// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <array>
#include <cmath>
#include <memory>

#include "Common/CommonTypes.h"
#include "Core/ConfigManager.h"

#include "VideoCommon/BPStructs.h"
#include "VideoCommon/Debugger.h"
#include "VideoCommon/GeometryShaderManager.h"
#include "VideoCommon/TessellationShaderManager.h"
#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/OpcodeDecoding.h"
#include "VideoCommon/PerfQueryBase.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/RenderBase.h"
#include "VideoCommon/SamplerCommon.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/XFMemory.h"

std::unique_ptr<VertexManagerBase> g_vertex_manager;

static const PrimitiveType primitive_from_gx[8] = {
  PrimitiveType::Triangles, // GX_DRAW_QUADS
  PrimitiveType::Triangles, // GX_DRAW_QUADS_2
  PrimitiveType::Triangles, // GX_DRAW_TRIANGLES
  PrimitiveType::Triangles, // GX_DRAW_TRIANGLE_STRIP
  PrimitiveType::Triangles, // GX_DRAW_TRIANGLE_FAN
  PrimitiveType::Lines,     // GX_DRAW_LINES
  PrimitiveType::Lines,     // GX_DRAW_LINE_STRIP
  PrimitiveType::Points,    // GX_DRAW_POINTS
};

// Due to the BT.601 standard which the GameCube is based on being a compromise
// between PAL and NTSC, neither standard gets square pixels. They are each off
// by ~9% in opposite directions.
// Just in case any game decides to take this into account, we do both these
// tests with a large amount of slop.
static bool AspectIs4_3(float width, float height)
{
  float aspect = fabsf(width / height);
  return fabsf(aspect - 4.0f / 3.0f) < 4.0f / 3.0f * 0.11;  // within 11% of 4:3
}

static bool AspectIs16_9(float width, float height)
{
  float aspect = fabsf(width / height);
  return fabsf(aspect - 16.0f / 9.0f) < 16.0f / 9.0f * 0.11;  // within 11% of 16:9
}

PrimitiveType VertexManagerBase::GetPrimitiveType(int primitive)
{
  return primitive_from_gx[primitive & 7];
}

VertexManagerBase::VertexManagerBase() {}

VertexManagerBase::~VertexManagerBase() {}



inline u32 GetRemainingIndices(int prim)
{
  OpcodeDecoder::GxDrawMode primitive = static_cast<OpcodeDecoder::GxDrawMode>(prim);
  u32 index_len = VertexManagerBase::MAXIBUFFERSIZE - IndexGenerator::GetIndexLen();
  if (primitive == OpcodeDecoder::GX_DRAW_TRIANGLE_STRIP || primitive == OpcodeDecoder::GX_DRAW_TRIANGLE_FAN)
  {
    return index_len / 3 + 2;
  }
  if (primitive < OpcodeDecoder::GX_DRAW_TRIANGLES)
  {
    return index_len / 6 * 4;
  }
  else if (primitive == OpcodeDecoder::GX_DRAW_LINE_STRIP)
  {
    return index_len / 2 + 1;
  }
  else if (primitive <= OpcodeDecoder::GX_DRAW_POINTS)
  {
    return index_len;
  }
  return 0;
}

void VertexManagerBase::PrepareForAdditionalData(int primitive, u32 count, u32 stride)
{
  // The SSE vertex loader can write up to 4 bytes past the end
  u32 const needed_vertex_bytes = count * stride + 4;
  u32 max_index_size = std::min(IndexGenerator::GetRemainingIndices(), GetRemainingIndices(primitive));

  // We can't merge different kinds of primitives, so we have to flush here
  // Check for size in buffer, if the buffer gets full, call Flush()
  PrimitiveType new_primitive_type = primitive_from_gx[primitive];
  if (m_current_primitive_type != new_primitive_type)
  {
    RasterizationState raster_state = {};
    raster_state.Generate(bpmem, new_primitive_type);
    g_renderer->SetRasterizationState(raster_state);
  }
  if (count > max_index_size
    || needed_vertex_bytes > GetRemainingSize()
    || m_current_primitive_type != primitive_from_gx[primitive])
  {
#if defined(_DEBUG) || defined(DEBUGFAST)
    if (count > IndexGenerator::GetRemainingIndices())
      ERROR_LOG(VIDEO, "Too little remaining index values. Use 32-bit or reset them on flush.");
    if (count > GetRemainingIndices(primitive))
      ERROR_LOG(VIDEO, "VertexManagerBase: Buffer not large enough for all indices! "
        "Increase MAXIBUFFERSIZE or we need primitive breaking after all.");
    if (needed_vertex_bytes > GetRemainingSize())
      ERROR_LOG(VIDEO, "VertexManagerBase: Buffer not large enough for all vertices! "
        "Increase MAXVBUFFERSIZE or we need primitive breaking after all.");
#endif
    Flush();
  }
  m_current_primitive_type = primitive_from_gx[primitive];
  m_cull_all = bpmem.genMode.cullmode == GenMode::CULL_ALL && primitive < 5;
  // need to alloc new buffer
  if (m_is_flushed)
  {
    g_vertex_manager->ResetBuffer(stride);
    m_is_flushed = false;
  }
}

std::pair<size_t, size_t> VertexManagerBase::ResetFlushAspectRatioCount()
{
  std::pair<size_t, size_t> val = std::make_pair(m_flush_count_4_3, m_flush_count_anamorphic);
  m_flush_count_4_3 = 0;
  m_flush_count_anamorphic = 0;
  return val;
}

static void SetSamplerState(u32 index, bool custom_tex, bool has_arbitrary_mips, float custom_tex_scale)
{
  const FourTexUnits& tex = bpmem.tex[index / 4];
  const TexMode0& tm0 = tex.texMode0[index % 4];

  SamplerState state = {};
  state.Generate(bpmem, index);
  bool mip_maps_enabled = SamplerCommon::AreBpTexMode0MipmapsEnabled(tm0);
  bool acurate_filtering = (!custom_tex || has_arbitrary_mips) && g_ActiveConfig.eFilteringMode == FilteringMode::Accurate && mip_maps_enabled && state.lod_bias.Value() != 0;
  // Force texture filtering config option.
  if (g_ActiveConfig.eFilteringMode == FilteringMode::Forced)
  {
    state.min_filter = SamplerState::Filter::Linear;
    state.mag_filter = SamplerState::Filter::Linear;
    state.mipmap_filter =
        mip_maps_enabled ?
      SamplerState::Filter::Linear :
      SamplerState::Filter::Point;
  }
  else if (g_ActiveConfig.eFilteringMode == FilteringMode::Disabled)
  {
    state.min_filter = SamplerState::Filter::Point;
    state.mag_filter = SamplerState::Filter::Point;
    state.mipmap_filter = SamplerState::Filter::Point;
  }

  // Custom textures may have a greater number of mips
  if (custom_tex)
    state.max_lod = 255;

  // Anisotropic filtering option.
  if (!acurate_filtering && g_ActiveConfig.iMaxAnisotropy != 0 && !SamplerCommon::IsBpTexMode0PointFilteringEnabled(tm0))
  {
    // https://www.opengl.org/registry/specs/EXT/texture_filter_anisotropic.txt
    // For predictable results on all hardware/drivers, only use one of:
    //	GL_LINEAR + GL_LINEAR (No Mipmaps [Bilinear])
    //	GL_LINEAR + GL_LINEAR_MIPMAP_LINEAR (w/ Mipmaps [Trilinear])
    // Letting the game set other combinations will have varying arbitrary results;
    // possibly being interpreted as equal to bilinear/trilinear, implicitly
    // disabling anisotropy, or changing the anisotropic algorithm employed.
    if (g_ActiveConfig.eFilteringMode != FilteringMode::Disabled)
    {
      state.min_filter = SamplerState::Filter::Linear;
      state.mag_filter = SamplerState::Filter::Linear;
      if (mip_maps_enabled)
        state.mipmap_filter = SamplerState::Filter::Linear;
    }
    state.anisotropic_filtering = 1;
  }
  else
  {
    state.anisotropic_filtering = 0;
  }

  if (acurate_filtering)
  {
    // Apply a secondary bias calculated from the IR scale to pull inwards mipmaps
    // that have arbitrary contents, eg. are used for fog effects where the
    // distance they kick in at is important to preserve at any resolution.
    // Correct this with the upscaling factor of custom textures.
    s64 lod_offset = std::log2(g_renderer->GetEFBScale() / custom_tex_scale) * 256.f;
    state.lod_bias = MathUtil::Clamp<s64>(state.lod_bias + lod_offset, -32768, 32767);
  }

  g_renderer->SetSamplerState(index, state);
}

void VertexManagerBase::DoFlush()
{
  // loading a state will invalidate BP, so check for it
  NativeVertexFormat* current_vertex_format = VertexLoaderManager::GetCurrentVertexFormat();
  g_video_backend->CheckInvalidState();
  g_vertex_manager->PrepareShaders(m_current_primitive_type, VertexLoaderManager::g_current_components, xfmem, bpmem);
#if defined(_DEBUG) || defined(DEBUGFAST)
  PRIM_LOG("frame%d:\n texgen=%d, numchan=%d, dualtex=%d, ztex=%d, cole=%d, alpe=%d, ze=%d", g_ActiveConfig.iSaveTargetId, xfmem.numTexGen.numTexGens,
    xfmem.numChan.numColorChans, xfmem.dualTexTrans.enabled, bpmem.ztex2.op,
    (int)bpmem.blendmode.colorupdate, (int)bpmem.blendmode.alphaupdate, (int)bpmem.zmode.updateenable);

  for (unsigned int i = 0; i < xfmem.numChan.numColorChans; ++i)
  {
    LitChannel* ch = &xfmem.color[i];
    PRIM_LOG("colchan%d: matsrc=%d, light=0x%x, ambsrc=%d, diffunc=%d, attfunc=%d", i, ch->matsource, ch->GetFullLightMask(), ch->ambsource, ch->diffusefunc, ch->attnfunc);
    ch = &xfmem.alpha[i];
    PRIM_LOG("alpchan%d: matsrc=%d, light=0x%x, ambsrc=%d, diffunc=%d, attfunc=%d", i, ch->matsource, ch->GetFullLightMask(), ch->ambsource, ch->diffusefunc, ch->attnfunc);
  }

  for (unsigned int i = 0; i < xfmem.numTexGen.numTexGens; ++i)
  {
    TexMtxInfo tinfo = xfmem.texMtxInfo[i];
    if (tinfo.texgentype != XF_TEXGEN_EMBOSS_MAP) tinfo.hex &= 0x7ff;
    if (tinfo.texgentype != XF_TEXGEN_REGULAR) tinfo.projection = 0;

    PRIM_LOG("txgen%d: proj=%d, input=%d, gentype=%d, srcrow=%d, embsrc=%d, emblght=%d, postmtx=%d, postnorm=%d",
      i, tinfo.projection, tinfo.inputform, tinfo.texgentype, tinfo.sourcerow, tinfo.embosssourceshift, tinfo.embosslightshift,
      xfmem.postMtxInfo[i].index, xfmem.postMtxInfo[i].normalize);
  }

  PRIM_LOG("pixel: tev=%d, ind=%d, texgen=%d, dstalpha=%d, alphatest=0x%x", (int)bpmem.genMode.numtevstages + 1, (int)bpmem.genMode.numindstages.Value(),
    (int)bpmem.genMode.numtexgens, (u32)bpmem.dstalpha.enable, (bpmem.alpha_test.hex >> 16) & 0xff);
#endif
  if (!m_cull_all)
  {
    u32 usedtextures = 0;
    for (u32 i = 0; i < bpmem.genMode.numtevstages + 1u; ++i)
      if (bpmem.tevorders[i / 2].getEnable(i & 1))
        usedtextures |= 1 << bpmem.tevorders[i / 2].getTexMap(i & 1);

    if (bpmem.genMode.numindstages.Value() > 0)
      for (u32 i = 0; i < bpmem.genMode.numtevstages + 1u; ++i)
        if (bpmem.tevind[i].IsActive() && bpmem.tevind[i].bt < bpmem.genMode.numindstages.Value())
          usedtextures |= 1 << bpmem.tevindref.getTexMap(bpmem.tevind[i].bt);

    s32 material_mask = 0;
    s32 emissive_mask = 0;
    for (unsigned int i = 0; i < 8; i++)
    {
      if (usedtextures & (1 << i))
      {
        const TextureCacheBase::TCacheEntry* tentry = g_texture_cache->Load(i);
        if (tentry)
        {
          int materiallayer = 0;
          int emissivelayer = 0;
          if (g_ActiveConfig.HiresMaterialMapsEnabled())
          {
            if (tentry->material_map)
            {
              materiallayer = 1;
            }
            if (tentry->emissive)
            {
              emissivelayer = materiallayer + 1;
            }
            material_mask |= ((int)tentry->material_map) << i;
            emissive_mask |= ((int)tentry->emissive) << i;
          }
          float custom_tex_scale = tentry->GetConfig().width / float(tentry->native_width);
          SetSamplerState(i, tentry->is_custom_tex || tentry->is_scaled, tentry->has_arbitrary_mips, custom_tex_scale);
          PixelShaderManager::SetTexDims(
            i,
            tentry->native_width,
            tentry->native_height,
            tentry->IsEfbCopy() ? tentry->GetConfig().layers : 0 ,
            materiallayer, emissivelayer);
        }
        else
          ERROR_LOG(VIDEO, "error loading texture");
      }
    }
    if (g_ActiveConfig.bForcePhongShading)
    {
      SamplerState state = {};
      state.min_filter = SamplerState::Filter::Linear;
      state.mag_filter = SamplerState::Filter::Linear;
      state.mipmap_filter = SamplerState::Filter::Linear;
      state.max_lod = 255;
      g_renderer->SetSamplerState(8, state);
    }
    if (g_ActiveConfig.HiresMaterialMapsEnabled())
    {
      PixelShaderManager::SetFlags(0, ~0, material_mask);
      PixelShaderManager::SetFlags(1, ~0, emissive_mask);
    }
    g_texture_cache->BindTextures();
  }
  // set global constants
  VertexShaderManager::SetConstants();

  // Track some stats used elsewhere by the anamorphic widescreen heuristic.
  if (!SConfig::GetInstance().bWii && xfmem.projection.type == GX_PERSPECTIVE)
  {
    float* rawProjection = xfmem.projection.rawProjection;
    bool viewport_is_4_3 = AspectIs4_3(xfmem.viewport.wd, xfmem.viewport.ht);
    if (AspectIs16_9(rawProjection[2], rawProjection[0]) && viewport_is_4_3)
    {
      // Projection is 16:9 and viewport is 4:3, we are rendering an anamorphic
      // widescreen picture.
      m_flush_count_anamorphic++;
    }
    else if (AspectIs4_3(rawProjection[2], rawProjection[0]) && viewport_is_4_3)
    {
      // Projection and viewports are both 4:3, we are rendering a normal image.
      m_flush_count_4_3++;
    }
  }

  if (m_current_primitive_type == PrimitiveType::Triangles)
  {
    const PortableVertexDeclaration &vtx_dcl = current_vertex_format->GetVertexDeclaration();
    if (bpmem.genMode.zfreeze)
    {
      if (m_zslope_refresh_required)
      {
        PixelShaderManager::SetZSlope(m_zslope.dfdx, m_zslope.dfdy, m_zslope.f0);
        m_zslope_refresh_required = false;
      }
    }
    else if (IndexGenerator::GetIndexLen() >= 3)
    {
      CalculateZSlope(vtx_dcl, g_vertex_manager->GetIndexBuffer() + IndexGenerator::GetIndexLen() - 3);
    }

    // if cull mode is CULL_ALL, ignore triangles and quads
    if (m_cull_all)
    {
      m_is_flushed = true;
      m_cull_all = false;
      return;
    }
  }
  GeometryShaderManager::SetConstants();
  TessellationShaderManager::SetConstants();
  PixelShaderManager::SetConstants();
  const bool useDstAlpha = bpmem.dstalpha.enable &&
    bpmem.blendmode.alphaupdate &&
    bpmem.zcontrol.pixel_format == PEControl::RGBA6_Z24;

  if (PerfQueryBase::ShouldEmulate())
    g_perf_query->EnableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);
  g_vertex_manager->vFlush(useDstAlpha);
  if (PerfQueryBase::ShouldEmulate())
    g_perf_query->DisableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);

  GFX_DEBUGGER_PAUSE_AT(NEXT_FLUSH, true);

  if (xfmem.numTexGen.numTexGens != bpmem.genMode.numtexgens)
    ERROR_LOG(VIDEO, "xf.numtexgens (%d) does not match bp.numtexgens (%d). Error in command stream.", xfmem.numTexGen.numTexGens, bpmem.genMode.numtexgens.Value());

  m_is_flushed = true;
  m_cull_all = false;
}

void VertexManagerBase::DoState(PointerWrap& p)
{
  g_vertex_manager->vDoState(p);
}

void VertexManagerBase::CalculateZSlope(const PortableVertexDeclaration &vert_decl, const u16* indices)
{
  float out[12];
  float viewOffset[2] = {
      xfmem.viewport.xOrig - bpmem.scissorOffset.x * 2,
      xfmem.viewport.yOrig - bpmem.scissorOffset.y * 2
  };

  // Lookup vertices of the last rendered triangle and software-transform them
  // This allows us to determine the depth slope, which will be used if zfreeze
  // is enabled in the following flush.
  float *vout = out;
  for (u32 i = 0; i < 3; ++i, vout += 4)
  {
    u8* vtx_ptr = m_pBaseBufferPointer + vert_decl.stride * indices[i];
    VertexShaderManager::TransformToClipSpace(vtx_ptr, vert_decl, vout);
    float w = 1.0f / vout[3];
    // Transform to Screenspace
    vout[0] = vout[0] * w * xfmem.viewport.wd + viewOffset[0];
    vout[1] = vout[1] * w * xfmem.viewport.ht + viewOffset[1];
    vout[2] = vout[2] * w * xfmem.viewport.zRange + xfmem.viewport.farZ;
  }

  float dx31 = out[8] - out[0];
  float dx12 = out[0] - out[4];
  float dy12 = out[1] - out[5];
  float dy31 = out[9] - out[1];
  float c = -dx12 * dy31 - dx31 * -dy12;

  if (c == 0)
    return;
  c = 1.0f / c;
  float DF31 = out[10] - out[2];
  float DF21 = out[6] - out[2];
  float a = DF31 * -dy12 - DF21 * dy31;
  float b = dx31 * DF21 + dx12 * DF31;
  m_zslope.dfdx = -a * c;
  m_zslope.dfdy = -b * c;
  m_zslope.f0 = out[2] - (out[0] * m_zslope.dfdx + out[1] * m_zslope.dfdy);
  m_zslope_refresh_required = true;
}
