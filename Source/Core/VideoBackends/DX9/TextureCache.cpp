// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <d3dx9.h>
#include <memory>

#include "Common/CommonPaths.h"
#include "Common/MemoryUtil.h"
#include "Common/Hash.h"
#include "Common/FileUtil.h"

#include "Core/HW/Memmap.h"

#include "VideoBackends/DX9/D3DBase.h"
#include "VideoBackends/DX9/D3DTexture.h"
#include "VideoBackends/DX9/D3DUtil.h"
#include "VideoBackends/DX9/DXTexture.h"
#include "VideoBackends/DX9/FramebufferManager.h"
#include "VideoBackends/DX9/PixelShaderCache.h"
#include "VideoBackends/DX9/Render.h"
#include "VideoBackends/DX9/TextureCache.h"
#include "VideoBackends/DX9/TextureConverter.h"
#include "VideoBackends/DX9/VertexShaderCache.h"
#include "VideoBackends/DX9/Depalettizer.h"

#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/Debugger.h"
#include "VideoCommon/HiresTextures.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/TextureScalerCommon.h"
#include "VideoCommon/VertexShaderManager.h"


extern s32 frameCount;

namespace DX9
{

static std::unique_ptr<Depalettizer> s_depaletizer;

void TextureCache::CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
  bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height)
{
  auto config = entry->GetConfig();
  const DXTexture* hosttexture = static_cast<const DXTexture*>(entry->GetColor());
  g_renderer->ResetAPIState(); // reset any game specific settings

  const LPDIRECT3DTEXTURE9 read_texture = is_depth_copy ?
    FramebufferManager::GetEFBDepthTexture() :
    FramebufferManager::GetEFBColorTexture();

  LPDIRECT3DSURFACE9 Rendersurf = NULL;
  hosttexture->GetRawTexIdentifier()->GetSurfaceLevel(0, &Rendersurf);
  D3D::dev->SetDepthStencilSurface(NULL);
  D3D::dev->SetRenderTarget(0, Rendersurf);

  D3DVIEWPORT9 vp;
  TargetRectangle targetSource = g_renderer->ConvertEFBRectangle(src_rect);
  // Stretch picture with increased internal resolution
  vp.X = 0;
  vp.Y = 0;
  vp.Width = width;
  vp.Height = height;
  vp.MinZ = 0.0f;
  vp.MaxZ = 1.0f;
  D3D::dev->SetViewport(&vp);
  RECT destrect;
  destrect.bottom = vp.Height;
  destrect.left = 0;
  destrect.right = vp.Width;
  destrect.top = 0;

  PixelShaderManager::SetColorMatrix(colmat); // set transformation
  D3D::dev->SetPixelShaderConstantF(C_COLORMATRIX, PixelShaderManager::GetBuffer(), 7);

  RECT sourcerect;
  sourcerect.bottom = targetSource.bottom;
  sourcerect.left = targetSource.left;
  sourcerect.right = targetSource.right;
  sourcerect.top = targetSource.top;

  if (is_depth_copy)
  {
    if (scale_by_half || g_ActiveConfig.iMultisamples > 1)
    {
      D3D::ChangeSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
      D3D::ChangeSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    }
    else
    {
      D3D::ChangeSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      D3D::ChangeSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    }
  }
  else
  {
    D3D::ChangeSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    D3D::ChangeSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  }

  D3DFORMAT bformat = FramebufferManager::GetEFBDepthRTSurfaceFormat();
  s32 SSAAMode = g_ActiveConfig.iMultisamples - 1;

  D3D::drawShadedTexQuad(read_texture, &sourcerect,
    g_renderer->GetTargetWidth(), g_renderer->GetTargetHeight(),
    config.width, config.height,
    PixelShaderCache::GetDepthMatrixProgram(SSAAMode, is_depth_copy && bformat != FOURCC_RAWZ),
    VertexShaderCache::GetSimpleVertexShader(SSAAMode));

  Rendersurf->Release();

  D3D::RefreshSamplerState(0, D3DSAMP_MINFILTER);
  D3D::RefreshSamplerState(0, D3DSAMP_MAGFILTER);
  D3D::SetTexture(0, NULL);
  D3D::dev->SetRenderTarget(0, FramebufferManager::GetEFBColorRTSurface());
  D3D::dev->SetDepthStencilSurface(FramebufferManager::GetEFBDepthRTSurface());

  g_renderer->RestoreAPIState();
}

void TextureCache::CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
  u32 num_blocks_y, u32 memory_stride, bool is_depth_copy,
  const EFBRectangle& src_rect, bool scale_by_half)
{
  TextureConverter::EncodeToRamFromTexture(dst, format, native_width, bytes_per_row, num_blocks_y,
    memory_stride, is_depth_copy, src_rect, scale_by_half);
}

bool TextureCache::Palettize(TCacheEntry* entry, const TCacheEntry* base_entry)
{
  LPDIRECT3DTEXTURE9 texture = static_cast<DXTexture*>(entry->GetColor())->GetRawTexIdentifier();
  u32 texformat = entry->format & 0xf;
  Depalettizer::BaseType baseType = Depalettizer::Unorm8;
  if (texformat == GX_TF_C4 || texformat == GX_TF_I4)
    baseType = Depalettizer::Unorm4;
  else if (texformat == GX_TF_C8 || texformat == GX_TF_I8)
    baseType = Depalettizer::Unorm8;
  else
    return false;
  return s_depaletizer->Depalettize(texture, static_cast<DXTexture*>(base_entry->GetColor())->GetRawTexIdentifier(), baseType);
}

HostTextureFormat TextureCache::GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height)
{
  const bool compressed_supported = ((width & 3) == 0) && ((height & 3) == 0);
  HostTextureFormat pcfmt = TexDecoder::GetHostTextureFormat(texformat, tlutfmt, compressed_supported);
  pcfmt = !g_ActiveConfig.backend_info.bSupportedFormats[pcfmt] ? PC_TEX_FMT_RGBA32 : pcfmt;
  return pcfmt;
}

std::unique_ptr<HostTexture> TextureCache::CreateTexture(const TextureConfig& config)
{
  return std::make_unique<DXTexture>(config);
}

void TextureCache::LoadLut(u32 lutFmt, void* addr, u32 size)
{
  s_depaletizer->UploadPalette(lutFmt, addr, size);
}

TextureCache::TextureCache()
{
  DXTexture::Initialize();
  s_depaletizer = std::make_unique<Depalettizer>();
}

TextureCache::~TextureCache()
{
  DXTexture::Dispose();
  s_depaletizer.reset();
}

}
