// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
#include <algorithm>
#include <cstring>
#include <memory>
#include <string>
#include <utility>

#include "Common/Align.h"
#include "Common/FileUtil.h"
#include "Common/MemoryUtil.h"
#include "Common/StringUtil.h"

#include "Core/ConfigManager.h"
#include "Core/FifoPlayer/FifoPlayer.h"
#include "Core/FifoPlayer/FifoRecorder.h"
#include "Core/HW/Memmap.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/FramebufferManagerBase.h"
#include "VideoCommon/HiresTextures.h"
#include "VideoCommon/PostProcessing.h"
#include "VideoCommon/RenderBase.h"
#include "VideoCommon/SamplerCommon.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/TextureScalerCommon.h"
#include "VideoCommon/TextureUtil.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"

static const u64 MAX_TEXTURE_BINARY_SIZE =
    1024 * 1024 * 4;  // 1024 x 1024 texel times 8 nibbles per texel
std::unique_ptr<TextureCacheBase> g_texture_cache;

TextureCacheBase::TCacheEntry::TCacheEntry(std::unique_ptr<HostTexture> tex, bool material,
                                           bool luma)
{
  texture = std::move(tex);
  material_map = material;
  emissive = luma;
}

TextureCacheBase::TCacheEntry::~TCacheEntry()
{
  for (auto& reference : references)
    reference->references.erase(this);
}

void TextureCacheBase::CheckTempSize(size_t required_size)
{
  if (required_size <= temp_size)
    return;

  temp_size = required_size;
  Common::FreeAlignedMemory(temp);
  temp = static_cast<u8*>(Common::AllocateAlignedMemory(temp_size, 16));
}

TextureCacheBase::TextureCacheBase()
{
  SetBackupConfig(g_ActiveConfig);
  temp_size = 2048 * 2048 * 4;
  temp = static_cast<u8*>(Common::AllocateAlignedMemory(temp_size, 16));

  TexDecoder::SetTexFmtOverlayOptions(backup_config.texfmt_overlay,
                                      backup_config.texfmt_overlay_center);

  HiresTexture::Init();

  texture_pool_memory_usage = 0;
  InvalidateAllBindPoints();
  m_scaler = std::make_unique<TextureScaler>();
}

void TextureCacheBase::Invalidate()
{
  InvalidateAllBindPoints();
  bound_textures.fill(nullptr);
  auto iter = textures_by_address.begin();
  auto end = textures_by_address.end();
  while (iter != end)
  {
    iter = InvalidateTexture(iter);
  }
  textures_by_address.clear();
  textures_by_hash.clear();
}

TextureCacheBase::~TextureCacheBase()
{
  HiresTexture::Shutdown();
  Invalidate();
  texture_pool.clear();
  texture_pool_memory_usage = 0;
  if (TextureCacheBase::temp)
  {
    Common::FreeAlignedMemory(TextureCacheBase::temp);
    TextureCacheBase::temp = nullptr;
  }
  m_scaler.reset();
}

void TextureCacheBase::OnConfigChanged(VideoConfig& config)
{
  if (config.bHiresTextures != backup_config.hires_textures ||
      config.bCacheHiresTextures != backup_config.cache_hires_textures)
  {
    HiresTexture::Update();
  }

  // TODO: Invalidating texcache is really stupid in some of these cases
  if (config.iSafeTextureCache_ColorSamples != backup_config.colorsamples ||
      config.bTexFmtOverlayEnable != backup_config.texfmt_overlay ||
      config.bTexFmtOverlayCenter != backup_config.texfmt_overlay_center ||
      config.bHiresTextures != backup_config.hires_textures ||
      config.iTexScalingFactor != backup_config.scaling_factor ||
      config.iTexScalingType != backup_config.scaling_mode ||
      config.bTexDeposterize != backup_config.scaling_deposterize ||
      config.bEnableGPUTextureDecoding != backup_config.gpu_texture_decoding)
  {
    g_texture_cache->Invalidate();

    TexDecoder::SetTexFmtOverlayOptions(g_ActiveConfig.bTexFmtOverlayEnable,
                                        g_ActiveConfig.bTexFmtOverlayCenter);
  }

  if ((config.iStereoMode > 0) != backup_config.stereo_3d ||
      config.bStereoEFBMonoDepth != backup_config.efb_mono_depth)
  {
    g_texture_cache->DeleteShaders();
    if (!g_texture_cache->CompileShaders())
      PanicAlert("Failed to recompile one or more texture conversion shaders.");
  }

  SetBackupConfig(config);
}

void TextureCacheBase::SetBackupConfig(const VideoConfig& config)
{
  backup_config.colorsamples = config.iSafeTextureCache_ColorSamples;
  backup_config.texfmt_overlay = config.bTexFmtOverlayEnable;
  backup_config.texfmt_overlay_center = config.bTexFmtOverlayCenter;
  backup_config.hires_textures = config.bHiresTextures;
  backup_config.cache_hires_textures = config.bCacheHiresTextures;
  backup_config.stereo_3d = config.iStereoMode > 0;
  backup_config.efb_mono_depth = config.bStereoEFBMonoDepth;
  backup_config.scaling_factor = config.iTexScalingFactor;
  backup_config.scaling_mode = config.iTexScalingType;
  backup_config.scaling_deposterize = config.bTexDeposterize;
  backup_config.gpu_texture_decoding = config.bEnableGPUTextureDecoding;
}

void TextureCacheBase::Cleanup(s32 _frameCount)
{
  s32 texture_kill_threshold = TEXTURE_KILL_THRESHOLD;
  if (texture_pool_memory_usage < (TEXTURE_POOL_MEMORY_LIMIT / 2))
  {
    // if we are using less than the memory limit increase kill threshold
    texture_kill_threshold *= TEXTURE_KILL_MULTIPLIER;
  }
  auto iter = textures_by_address.begin();
  auto tcend = textures_by_address.end();
  while (iter != tcend)
  {
    if (iter->second->tmem_only)
    {
      iter = InvalidateTexture(iter);
    }
    else if (iter->second->frameCount == FRAMECOUNT_INVALID)
    {
      iter->second->frameCount = _frameCount;
    }
    if (iter == tcend)
    {
      break;
    }
    if (_frameCount > texture_kill_threshold + iter->second->frameCount)
    {
      if (iter->second->IsEfbCopy())
      {
        // Only remove EFB copies when they wouldn't be used anymore(changed hash), because EFB
        // copies living on the host GPU are unrecoverable. Perform this check only every
        // TEXTURE_KILL_THRESHOLD for performance reasons
        if ((_frameCount - iter->second->frameCount) % TEXTURE_KILL_THRESHOLD == 1 &&
            iter->second->hash != iter->second->CalculateHash())
        {
          iter = InvalidateTexture(iter);
        }
        else
        {
          ++iter;
        }
      }
      else
      {
        iter = InvalidateTexture(iter);
      }
    }
    else
    {
      ++iter;
    }
  }
  TexPool::iterator iter2 = texture_pool.begin();
  TexPool::iterator tcend2 = texture_pool.end();
  while (iter2 != tcend2)
  {
    if (iter2->second.frameCount == FRAMECOUNT_INVALID)
    {
      iter2->second.frameCount = _frameCount;
    }
    if (_frameCount > TEXTURE_POOL_KILL_THRESHOLD + iter2->second.frameCount)
    {
      texture_pool_memory_usage -= iter2->second.texture->GetConfig().GetSizeInBytes();
      iter2 = texture_pool.erase(iter2);
    }
    else
    {
      ++iter2;
    }
  }
}

bool TextureCacheBase::TCacheEntry::OverlapsMemoryRange(u32 range_address, u32 range_size) const
{
  if (addr + size_in_bytes <= range_address)
    return false;

  if (addr >= range_address + range_size)
    return false;

  return true;
}

TextureCacheBase::TCacheEntry* TextureCacheBase::ApplyPaletteToEntry(TCacheEntry* entry,
                                                                     u32 tlutaddr, u32 tlutfmt,
                                                                     u32 palette_size)
{
  TextureConfig newconfig = entry->GetConfig();
  newconfig.rendertarget = true;
  newconfig.pcformat = PC_TEX_FMT_RGBA32;
  newconfig.levels = 1;
  TCacheEntry* decoded_entry = AllocateCacheEntry(newconfig, false);

  if (decoded_entry)
  {
    decoded_entry->SetGeneralParameters(entry->addr, entry->size_in_bytes, entry->format);
    decoded_entry->SetDimensions(entry->native_width, entry->native_height, 1);
    decoded_entry->SetHashes(entry->base_hash, entry->hash);
    decoded_entry->frameCount = FRAMECOUNT_INVALID;
    decoded_entry->is_efb_copy = false;
    g_texture_cache->LoadLut(tlutfmt, &texMem[tlutaddr], palette_size);
    auto iter = textures_by_address.emplace(entry->addr, decoded_entry);
    if (g_texture_cache->Palettize(decoded_entry, entry))
    {
      return decoded_entry;
    }
    InvalidateTexture(iter);
  }
  return nullptr;
}

void TextureCacheBase::ScaleTextureCacheEntryTo(TextureCacheBase::TCacheEntry* entry, u32 new_width,
                                                u32 new_height)
{
  if (entry->GetConfig().width == new_width && entry->GetConfig().height == new_height)
  {
    return;
  }

  u32 max = g_ActiveConfig.backend_info.MaxTextureSize;
  if (max < new_width || max < new_height)
  {
    ERROR_LOG(VIDEO, "Texture too big, width = %d, height = %d", new_width, new_height);
    return;
  }

  TextureConfig newconfig;
  newconfig.width = new_width;
  newconfig.height = new_height;
  newconfig.layers = entry->GetConfig().layers;
  newconfig.rendertarget = true;
  newconfig.pcformat = PC_TEX_FMT_RGBA32;
  std::unique_ptr<HostTexture> new_texture = AllocateTexture(newconfig);
  if (new_texture)
  {
    new_texture->CopyRectangleFromTexture(entry->texture.get(), entry->GetConfig().GetRect(),
                                          new_texture->GetConfig().GetRect());

    entry->texture.swap(new_texture);

    auto config = new_texture->GetConfig();
    // At this point new_texture has the old texture in it,
    // we can potentially reuse this, so let's move it back to the pool
    texture_pool.emplace(config, TexPoolEntry(std::move(new_texture)));
  }
  else
  {
    ERROR_LOG(VIDEO, "Scaling failed");
  }
}

TextureCacheBase::TCacheEntry*
TextureCacheBase::DoPartialTextureUpdates(TCacheEntry* entry_to_update, u32 tlutaddr, u32 tlutfmt,
                                          u32 palette_size)
{
  // If the flag may_have_overlapping_textures is cleared, there are no overlapping EFB copies,
  // which aren't applied already. It is set for new textures, and for the affected range
  // on each EFB copy.
  if (!entry_to_update->may_have_overlapping_textures)
    return entry_to_update;
  entry_to_update->may_have_overlapping_textures = false;

  const bool isPaletteTexture =
      (entry_to_update->format == GX_TF_C4 || entry_to_update->format == GX_TF_C8 ||
       entry_to_update->format == GX_TF_C14X2 || entry_to_update->format >= 0x10000);

  // Efb copies and paletted textures are excluded from these updates, until there's an example
  // where a game would benefit from this. Both would require more work to be done.
  if (entry_to_update->IsEfbCopy())
    return entry_to_update;

  u32 block_width = TexDecoder::GetBlockWidthInTexels(entry_to_update->format & 0xf);
  u32 block_height = TexDecoder::GetBlockHeightInTexels(entry_to_update->format & 0xf);
  u32 block_size = block_width * block_height *
                   TexDecoder::GetTexelSizeInNibbles(entry_to_update->format & 0xf) / 2;

  u32 numBlocksX = (entry_to_update->native_width + block_width - 1) / block_width;

  auto iter = FindOverlappingTextures(entry_to_update->addr, entry_to_update->size_in_bytes);
  while (iter.first != iter.second)
  {
    TCacheEntry* entry = iter.first->second;
    if (entry != entry_to_update && entry->IsEfbCopy() && !entry->tmem_only &&
        entry->references.count(entry_to_update) == 0 &&
        entry->OverlapsMemoryRange(entry_to_update->addr, entry_to_update->size_in_bytes) &&
        entry->memory_stride == numBlocksX * block_size)
    {
      if (entry->hash == entry->CalculateHash())
      {
        if (isPaletteTexture)
        {
          TCacheEntry* decoded_entry = ApplyPaletteToEntry(entry, tlutaddr, tlutfmt, palette_size);
          if (decoded_entry)
          {
            // Link the efb copy with the partially updated texture, so we won't apply this partial
            // update again
            entry->CreateReference(entry_to_update);
            // Mark the texture update as used, as if it was loaded directly
            entry->frameCount = FRAMECOUNT_INVALID;
            entry = decoded_entry;
          }
          else
          {
            ++iter.first;
            continue;
          }
        }

        u32 src_x, src_y, dst_x, dst_y;
        // Note for understanding the math:
        // Normal textures can't be strided, so the 2 missing cases with src_x > 0 don't exist
        if (entry->addr >= entry_to_update->addr)
        {
          u32 block_offset = (entry->addr - entry_to_update->addr) / block_size;
          u32 block_x = block_offset % numBlocksX;
          u32 block_y = block_offset / numBlocksX;
          src_x = 0;
          src_y = 0;
          dst_x = block_x * block_width;
          dst_y = block_y * block_height;
        }
        else
        {
          u32 block_offset = (entry_to_update->addr - entry->addr) / block_size;
          u32 block_x = (~block_offset + 1) % numBlocksX;
          u32 block_y = (block_offset + block_x) / numBlocksX;
          src_x = 0;
          src_y = block_y * block_height;
          dst_x = block_x * block_width;
          dst_y = 0;
        }

        u32 copy_width =
            std::min(entry->native_width - src_x, entry_to_update->native_width - dst_x);
        u32 copy_height =
            std::min(entry->native_height - src_y, entry_to_update->native_height - dst_y);

        // If one of the textures is scaled, scale both with the current efb scaling factor
        if (entry_to_update->native_width != entry_to_update->GetConfig().width ||
            entry_to_update->native_height != entry_to_update->GetConfig().height ||
            entry->native_width != entry->GetConfig().width ||
            entry->native_height != entry->GetConfig().height)
        {
          ScaleTextureCacheEntryTo(entry_to_update,
                                   g_renderer->EFBToScaledX(entry_to_update->native_width),
                                   g_renderer->EFBToScaledY(entry_to_update->native_height));
          ScaleTextureCacheEntryTo(entry, g_renderer->EFBToScaledX(entry->native_width),
                                   g_renderer->EFBToScaledY(entry->native_height));

          src_x = g_renderer->EFBToScaledX(src_x);
          src_y = g_renderer->EFBToScaledY(src_y);
          dst_x = g_renderer->EFBToScaledX(dst_x);
          dst_y = g_renderer->EFBToScaledY(dst_y);
          copy_width = g_renderer->EFBToScaledX(copy_width);
          copy_height = g_renderer->EFBToScaledY(copy_height);
        }

        MathUtil::Rectangle<int> srcrect, dstrect;
        srcrect.left = src_x;
        srcrect.top = src_y;
        srcrect.right = (src_x + copy_width);
        srcrect.bottom = (src_y + copy_height);
        dstrect.left = dst_x;
        dstrect.top = dst_y;
        dstrect.right = (dst_x + copy_width);
        dstrect.bottom = (dst_y + copy_height);
        entry_to_update->texture->CopyRectangleFromTexture(entry->texture.get(), srcrect, dstrect);

        if (isPaletteTexture)
        {
          // Remove the temporary converted texture, it won't be used anywhere else
          // TODO: It would be nice to convert and copy in one step, but this code path isn't common
          InvalidateTexture(GetTexCacheIter(entry));
        }
        else
        {
          // Link the two textures together, so we won't apply this partial update again
          entry->CreateReference(entry_to_update);
          // Mark the texture update as used, as if it was loaded directly
          entry->frameCount = FRAMECOUNT_INVALID;
        }
      }
      else
      {
        // If the hash does not match, this EFB copy will not be used for anything, so remove it
        iter.first = InvalidateTexture(iter.first);
        continue;
      }
    }
    ++iter.first;
  }
  return entry_to_update;
}

void TextureCacheBase::DumpTexture(TCacheEntry* entry, std::string basename, u32 level)
{
  std::string szDir = File::GetUserPath(D_DUMPTEXTURES_IDX) + SConfig::GetInstance().GetGameID();

  // make sure that the directory exists
  if (!File::Exists(szDir) || !File::IsDirectory(szDir))
    File::CreateDir(szDir);

  if (level > 0)
  {
    basename += StringFromFormat("_mip%i", level);
  }

  std::string filename = szDir + "/" + basename +
                         (TexDecoder::IsCompressed(entry->GetConfig().pcformat) ? ".dds" : ".png");

  if (!File::Exists(filename))
    entry->texture->Save(filename, level);
}

// Used by TextureCacheBase::Load
TextureCacheBase::TCacheEntry* TextureCacheBase::ReturnEntry(u32 stage, TCacheEntry* entry)
{
  entry->frameCount = FRAMECOUNT_INVALID;
  bound_textures[stage] = entry;
  GFX_DEBUGGER_PAUSE_AT(NEXT_TEXTURE_CHANGE, true);
  // We need to keep track of invalided textures until they have actually been replaced or re-loaded
  valid_bind_points.set(stage);
  return entry;
}
void TextureCacheBase::BindTextures()
{
  for (u32 i = 0; i < bound_textures.size(); ++i)
  {
    if (IsValidBindPoint(static_cast<u32>(i)) && bound_textures[i])
    {
      bound_textures[i]->texture->Bind(i);
    }
  }
}

TextureCacheBase::TCacheEntry* TextureCacheBase::Load(const u32 stage)
{
  // if this stage was not invalidated by changes to texture registers, keep the current texture
  if (IsValidBindPoint(stage) && bound_textures[stage])
  {
    return ReturnEntry(stage, bound_textures[stage]);
  }

  const FourTexUnits& tex = bpmem.tex[stage >> 2];
  const u32 id = stage & 3;
  const u32 address = (tex.texImage3[id].image_base /* & 0x1FFFFF*/) << 5;
  u32 width = tex.texImage0[id].width + 1;
  u32 height = tex.texImage0[id].height + 1;
  const u32 texformat = tex.texImage0[id].format;
  const u32 tlutaddr = tex.texTlut[id].tmem_offset << 9;
  const u32 tlutfmt = tex.texTlut[id].tlut_format;
  const bool use_mipmaps = SamplerCommon::AreBpTexMode0MipmapsEnabled(tex.texMode0[id]);
  u32 tex_levels = use_mipmaps ? ((tex.texMode1[id].max_lod + 0xf) / 0x10 + 1) : 1;
  const bool from_tmem = tex.texImage1[id].image_type != 0;

  // TexelSizeInNibbles(format) * width * height / 16;
  const u32 bsw = TexDecoder::GetBlockWidthInTexels(texformat);
  const u32 bsh = TexDecoder::GetBlockHeightInTexels(texformat);

  u32 expandedWidth = Common::AlignUpSizePow2(width, bsw);
  u32 expandedHeight = Common::AlignUpSizePow2(height, bsh);
  const u32 nativeW = width;
  const u32 nativeH = height;

  // Hash assigned to texcache entry (also used to generate filenames used for texture dumping and
  // custom texture lookup)
  u64 tex_hash = TEXHASH_INVALID;
  u64 tlut_hash = TEXHASH_INVALID;
  u64 full_hash = TEXHASH_INVALID;
  u32 full_format = texformat;
  HostTextureFormat pcfmt = PC_TEX_FMT_NONE;

  const bool isPaletteTexture =
      (texformat == GX_TF_C4 || texformat == GX_TF_C8 || texformat == GX_TF_C14X2);

  if (isPaletteTexture)
  {
    // Reject invalid tlut format.
    if (tlutfmt > GX_TL_RGB5A3)
    {
      return nullptr;
    }
    full_format = texformat | (tlutfmt << 16);
  }

  const u32 texture_size =
      TexDecoder::GetTextureSizeInBytes(expandedWidth, expandedHeight, texformat);
  u32 bytes_per_block = (bsw * bsh * TexDecoder::GetTexelSizeInNibbles(texformat)) / 2;
  u32 additional_mips_size = 0;  // not including level 0, which is texture_size

  // GPUs don't like when the specified mipmap count would require more than one 1x1-sized LOD in
  // the mipmap chain e.g. 64x64 with 7 LODs would have the mipmap chain
  // 64x64,32x32,16x16,8x8,4x4,2x2,1x1,0x0, so we limit the mipmap count to 6 there
  tex_levels = std::min<u32>(IntLog2(std::max(width, height)) + 1, tex_levels);

  for (u32 level = 1; level != tex_levels; ++level)
  {
    // We still need to calculate the original size of the mips
    const u32 expanded_mip_width =
        Common::AlignUpSizePow2(TextureUtil::CalculateLevelSize(width, level), bsw);
    const u32 expanded_mip_height =
        Common::AlignUpSizePow2(TextureUtil::CalculateLevelSize(height, level), bsh);

    additional_mips_size +=
        TexDecoder::GetTextureSizeInBytes(expanded_mip_width, expanded_mip_height, texformat);
  }

  const u8* src_data;
  if (from_tmem)
    src_data = &texMem[bpmem.tex[stage / 4].texImage1[stage % 4].tmem_even * TMEM_LINE_SIZE];
  else
    src_data = Memory::GetPointer(address);

  if (src_data == nullptr)
  {
    ERROR_LOG(
        VIDEO,
        "TextureCacheBase::Load has an address in Wii memory (%8x) but not in real memory (NULL)!",
        address);
    return nullptr;
  }

  // If we are recording a FifoLog, keep track of what memory we read.
  // FifiRecorder does it's own memory modification tracking independant of the texture hashing
  // below.
  if (g_bRecordFifoData && !from_tmem)
    FifoRecorder::GetInstance().UseMemory(address, texture_size + additional_mips_size,
                                          MemoryUpdate::TEXTURE_MAP);

  // TODO: This doesn't hash GB tiles for preloaded RGBA8 textures (instead, it's hashing more data
  // from the low tmem bank than it should)
  tex_hash = GetHash64(src_data, texture_size, g_ActiveConfig.iSafeTextureCache_ColorSamples);
  u32 palette_size = std::min(TexDecoder::GetPaletteSize(texformat), TMEM_SIZE - tlutaddr);
  if (isPaletteTexture)
  {
    tlut_hash =
        GetHash64(&texMem[tlutaddr], palette_size, g_ActiveConfig.iSafeTextureCache_ColorSamples);
    full_hash = tex_hash ^ tlut_hash;
  }
  else
  {
    full_hash = tex_hash;
  }
  // Search the texture cache for textures by address
  //
  // Find all texture cache entries for the current texture address, and decide whether to use one
  // of them, or to create a new one
  //
  // In most cases, the fastest way is to use only one texture cache entry for the same address.
  // Usually, when a texture changes, the old version of the texture is unlikely to be used again.
  // If there were new cache entries created for normal texture updates, there would be a slowdown
  // due to a huge amount of unused cache entries. Also thanks to texture pooling, overwriting an
  // existing cache entry is faster than creating a new one from scratch.
  //
  // Some games use the same address for different textures though. If the same cache entry was used
  // in this case, it would be constantly overwritten, and effectively there wouldn't be any caching
  // for those textures. Examples for this are Metroid Prime and Castlevania 3. Metroid Prime has
  // multiple sets of fonts on each other stored in a single texture and uses the palette to make
  // different characters visible or invisible. In Castlevania 3 some textures are used for 2
  // different things or at least in 2 different ways(size 1024x1024 vs 1024x256).
  //
  // To determine whether to use multiple cache entries or a single entry, use the following
  // heuristic: If the same texture address is used several times during the same frame, assume the
  // address is used for different purposes and allow creating an additional cache entry. If there's
  // at least one entry that hasn't been used for the same frame, then overwrite it, in order to
  // keep the cache as small as possible. If the current texture is found in the cache, use that
  // entry.
  //
  // For efb copies, the entry created in CopyRenderTargetToTexture always has to be used, or else
  // it was done in vain.
  auto iter_range = textures_by_address.equal_range(address);
  TexAddrCache::iterator iter = iter_range.first;
  TexAddrCache::iterator oldest_entry = iter;
  s32 temp_frameCount = 0x7fffffff;
  TexAddrCache::iterator unconverted_copy = textures_by_address.end();

  while (iter != iter_range.second)
  {
    TCacheEntry* entry = iter->second;
    // Skip entries that are only left in our texture cache for the tmem cache emulation
    if (entry->tmem_only)
    {
      ++iter;
      continue;
    }
    // Do not load strided EFB copies, they are not meant to be used directly
    if (entry->IsEfbCopy() && entry->native_width >= nativeW && entry->native_height >= nativeH &&
        entry->memory_stride == entry->BytesPerRow())
    {
      // EFB copies have slightly different rules: the hash doesn't need to match
      // in EFB2Tex mode, and EFB copy formats have different meanings from texture
      // formats.
      if (g_ActiveConfig.bSkipEFBCopyToRam || (tex_hash == entry->hash) ||
          IsPlayingBackFifologWithBrokenEFBCopies)
      {
        // TODO: We should check format/width/height/levels for EFB copies. Checking
        // format is complicated because EFB copy formats don't exactly match
        // texture formats. I'm not sure what effect checking width/height/levels
        // would have.
        if (!isPaletteTexture)
          return ReturnEntry(stage, entry);
        // Note that we found an unconverted EFB copy, then continue. We'll
        // perform the conversion later. Currently, we only convert EFB copies to
        // palette textures; we could do other conversions if it proved to be
        // beneficial.
        unconverted_copy = iter;
      }
      else
      {
        // Aggressively prune EFB copies: if it isn't useful here, it will probably
        // never be useful again. It's theoretically possible for a game to do
        // something weird where the copy could become useful in the future, but in
        // practice it doesn't happen.
        iter = InvalidateTexture(iter);
        continue;
      }
    }
    else
    {
      // For normal textures, all texture parameters need to match
      if (entry->hash == (full_hash) && entry->format == full_format &&
          entry->native_levels >= tex_levels && entry->native_width == nativeW &&
          entry->native_height == nativeH)
      {
        entry = DoPartialTextureUpdates(iter->second, tlutaddr, tlutfmt, palette_size);
        return ReturnEntry(stage, entry);
      }
    }
    // Find the texture which hasn't been used for the longest time. Count paletted
    // textures as the same texture here, when the texture itself is the same. This
    // improves the performance a lot in some games that use paletted textures.
    // Example: Sonic the Fighters (inside Sonic Gems Collection)
    // Skip EFB copies here, so they can be used for partial texture updates
    if (entry->frameCount != FRAMECOUNT_INVALID && entry->frameCount < temp_frameCount &&
        !entry->IsEfbCopy() && !(isPaletteTexture && entry->base_hash == tex_hash))
    {
      temp_frameCount = entry->frameCount;
      oldest_entry = iter;
    }
    ++iter;
  }
  std::string basename;
  if (unconverted_copy != textures_by_address.end())
  {
    g_texture_cache->LoadLut(tlutfmt, &texMem[tlutaddr], palette_size);
    // Perform palette decoding.
    TCacheEntry* decoded_entry =
        ApplyPaletteToEntry(unconverted_copy->second, tlutaddr, tlutfmt, palette_size);

    if (decoded_entry)
    {
      return ReturnEntry(stage, decoded_entry);
    }
  }

  // Search the texture cache for normal textures by hash
  //
  // If the texture was fully hashed, the address does not need to match. Identical duplicate
  // textures cause unnecessary slowdowns Example: Tales of Symphonia (GC) uses over 500 small
  // textures in menus, but only around 70 different ones
  if (g_ActiveConfig.iSafeTextureCache_ColorSamples == 0 ||
      std::max(texture_size, palette_size) <=
          (u32)g_ActiveConfig.iSafeTextureCache_ColorSamples * 8)
  {
    auto hash_range = textures_by_hash.equal_range(full_hash);
    TexHashCache::iterator hash_iter = hash_range.first;
    while (hash_iter != hash_range.second)
    {
      TCacheEntry* entry = hash_iter->second;
      // All parameters, except the address, need to match here
      if (entry->format == full_format && entry->native_levels >= tex_levels &&
          entry->native_width == nativeW && entry->native_height == nativeH)
      {
        entry = DoPartialTextureUpdates(hash_iter->second, tlutaddr, tlutfmt, palette_size);
        return ReturnEntry(stage, entry);
      }
      ++hash_iter;
    }
  }

  // If at least one entry was not used for the same frame, overwrite the oldest one
  if (temp_frameCount != 0x7fffffff)
  {
    // pool this texture and make a new one later
    InvalidateTexture(oldest_entry);
  }

  std::shared_ptr<HiresTexture> hires_tex;
  if (g_ActiveConfig.bHiresTextures || g_ActiveConfig.bDumpTextures)
  {
    basename =
        HiresTexture::GenBaseName(src_data, texture_size, &texMem[tlutaddr], palette_size, width,
                                  height, texformat, use_mipmaps, g_ActiveConfig.bDumpTextures);
  }
  if (g_ActiveConfig.bHiresTextures)
  {
    hires_tex = HiresTexture::Search(basename, [this](size_t required_size) {
      this->CheckTempSize(required_size);
      return this->temp;
    });
    if (hires_tex)
    {
      if (hires_tex->m_width != width || hires_tex->m_height != height)
      {
        width = hires_tex->m_width;
        height = hires_tex->m_height;
      }
      expandedWidth = hires_tex->m_width;
      expandedHeight = hires_tex->m_height;
      pcfmt = hires_tex->m_format;
    }
    else if (palette_size > 0)
    {
      std::string tempname =
          HiresTexture::GenBaseName(src_data, texture_size, &texMem[tlutaddr], 0, width, height,
                                    texformat, use_mipmaps, g_ActiveConfig.bDumpTextures);

      hires_tex = HiresTexture::Search(tempname, [this](size_t required_size) {
        this->CheckTempSize(required_size);
        return this->temp;
      });
      if (hires_tex)
      {
        if (hires_tex->m_width != width || hires_tex->m_height != height)
        {
          width = hires_tex->m_width;
          height = hires_tex->m_height;
        }
        expandedWidth = hires_tex->m_width;
        expandedHeight = hires_tex->m_height;
        pcfmt = hires_tex->m_format;
      }
    }
  }
  if (isPaletteTexture && !hires_tex)
  {
    g_texture_cache->LoadLut(tlutfmt, &texMem[tlutaddr], palette_size);
  }
  if (pcfmt == PC_TEX_FMT_NONE)
  {
    pcfmt = g_texture_cache->GetHostTextureFormat(texformat, (TlutFormat)tlutfmt, width, height);
  }
  // how many levels the allocated texture shall have
  const u32 texLevels = hires_tex ? hires_tex->m_levels : tex_levels;
  const bool use_scaling =
      (g_ActiveConfig.iTexScalingType > 0) && !hires_tex && (width < 384) && (height < 384);
  // We can decode on the GPU if it is a supported format and the flag is enabled.
  // Currently we don't decode RGBA8 textures from Tmem, as that would require copying from both
  // banks, and if we're doing an copy we may as well just do the whole thing on the CPU, since
  // there's no conversion between formats. In the future this could be extended with a separate
  // shader, however.
  bool decode_on_gpu =
      !hires_tex && !use_scaling && g_ActiveConfig.UseGPUTextureDecoding() &&
      g_texture_cache->SupportsGPUTextureDecode(static_cast<TextureFormat>(texformat),
                                                static_cast<TlutFormat>(tlutfmt)) &&
      !(from_tmem && texformat == GX_TF_RGBA8);

  // create the entry/texture
  TextureConfig config;
  config.width = width;
  config.height = height;
  config.levels = texLevels;
  config.pcformat = pcfmt;
  const bool materialmap =
      hires_tex && hires_tex->m_nrm_levels && g_ActiveConfig.HiresMaterialMapsEnabled();
  const bool emissivematerial =
      hires_tex && hires_tex->m_lum_levels && g_ActiveConfig.HiresMaterialMapsEnabled();
  config.layers += materialmap ? 1 : 0;
  config.layers += emissivematerial ? 1 : 0;
  if (use_scaling)
  {
    config.width *= g_ActiveConfig.iTexScalingFactor;
    config.height *= g_ActiveConfig.iTexScalingFactor;
    config.pcformat = PC_TEX_FMT_RGBA32;
  }
  TCacheEntry* entry = AllocateCacheEntry(config, materialmap);
  GFX_DEBUGGER_PAUSE_AT(NEXT_NEW_TEXTURE, true);

  iter = textures_by_address.emplace(address, entry);
  if (g_ActiveConfig.iSafeTextureCache_ColorSamples == 0 ||
      std::max(texture_size, palette_size) <=
          (u32)g_ActiveConfig.iSafeTextureCache_ColorSamples * 8)
  {
    entry->textures_by_hash_iter = textures_by_hash.emplace(full_hash, entry);
  }

  entry->SetGeneralParameters(address, texture_size, full_format);
  entry->SetDimensions(nativeW, nativeH, tex_levels);
  entry->SetHiresParams(!!hires_tex, basename, use_scaling, emissivematerial,
                        !!hires_tex && hires_tex->has_arbitrary_mips);
  entry->SetHashes(full_hash, tex_hash);
  entry->is_efb_copy = false;

  // load texture
  if (hires_tex)
  {
    int currentlayer = 0;
    entry->texture->Load(TextureCacheBase::temp, width, height, expandedWidth, 0, currentlayer);
    u8* Bufferptr = TextureCacheBase::temp;
    Bufferptr += TextureUtil::GetTextureSizeInBytes(width, height, pcfmt);
    for (u32 level = 1; level != texLevels; ++level)
    {
      u32 mip_width = TextureUtil::CalculateLevelSize(width, level);
      u32 mip_height = TextureUtil::CalculateLevelSize(height, level);
      entry->texture->Load(Bufferptr, mip_width, mip_height, mip_width, level, currentlayer);
      Bufferptr += TextureUtil::GetTextureSizeInBytes(mip_width, mip_height, pcfmt);
    }
    if (materialmap)
    {
      currentlayer++;
      entry->texture->Load(Bufferptr, width, height, width, 0, currentlayer);
      Bufferptr += TextureUtil::GetTextureSizeInBytes(width, height, pcfmt);
      for (u32 level = 1; level != texLevels; ++level)
      {
        u32 mip_width = TextureUtil::CalculateLevelSize(width, level);
        u32 mip_height = TextureUtil::CalculateLevelSize(height, level);
        entry->texture->Load(Bufferptr, mip_width, mip_height, mip_width, level, currentlayer);
        Bufferptr += TextureUtil::GetTextureSizeInBytes(mip_width, mip_height, pcfmt);
      }
    }
    if (emissivematerial)
    {
      currentlayer++;
      entry->texture->Load(Bufferptr, width, height, width, 0, currentlayer);
      Bufferptr += TextureUtil::GetTextureSizeInBytes(width, height, pcfmt);
      for (u32 level = 1; level != texLevels; ++level)
      {
        u32 mip_width = TextureUtil::CalculateLevelSize(width, level);
        u32 mip_height = TextureUtil::CalculateLevelSize(height, level);
        entry->texture->Load(Bufferptr, mip_width, mip_height, mip_width, level, currentlayer);
        Bufferptr += TextureUtil::GetTextureSizeInBytes(mip_width, mip_height, pcfmt);
      }
    }
  }
  else
  {
    const u8* ptr_even = NULL;
    const u8* ptr_odd = NULL;
    if (from_tmem)
    {
      ptr_even = &texMem[bpmem.tex[stage / 4].texImage1[stage % 4].tmem_even * TMEM_LINE_SIZE +
                         texture_size];
      ptr_odd = &texMem[bpmem.tex[stage / 4].texImage2[stage % 4].tmem_odd * TMEM_LINE_SIZE];
    }

    if (decode_on_gpu)
    {
      u32 row_stride = bytes_per_block * (expandedWidth / bsw);
      decode_on_gpu = DecodeTextureOnGPU(entry->texture.get(), 0, src_data, texture_size,
                                         static_cast<TextureFormat>(texformat), width, height,
                                         expandedWidth, expandedHeight, row_stride,
                                         &texMem[tlutaddr], static_cast<TlutFormat>(tlutfmt));
    }

    if (!decode_on_gpu)
    {
      u8* texturedata = TextureCacheBase::temp;
      u32 twidth = width;
      u32 theight = height;
      u32 texpandedWidth = expandedWidth;
      if (texformat == GX_TF_RGBA8 && from_tmem)
      {
        TexDecoder::DecodeRGBA8FromTmem(reinterpret_cast<u32*>(texturedata), src_data, ptr_odd,
                                        expandedWidth, expandedHeight);
      }
      else
      {
        TexDecoder::Decode(texturedata, src_data, expandedWidth, expandedHeight, texformat,
                           tlutaddr, static_cast<TlutFormat>(tlutfmt),
                           PC_TEX_FMT_RGBA32 == config.pcformat,
                           config.pcformat >= PC_TEX_FMT_DXT1);
      }
      if (use_scaling)
      {
        texturedata =
            reinterpret_cast<u8*>(m_scaler->Scale((u32*)texturedata, expandedWidth, height));
        twidth *= g_ActiveConfig.iTexScalingFactor;
        theight *= g_ActiveConfig.iTexScalingFactor;
        texpandedWidth *= g_ActiveConfig.iTexScalingFactor;
      }
      entry->texture->Load(texturedata, twidth, theight, texpandedWidth, 0, 0);
    }
    if (g_ActiveConfig.bDumpTextures)
    {
      DumpTexture(entry, basename, 0);
    }
    src_data += texture_size;

    for (u32 level = 1; level != texLevels; ++level)
    {
      const u32 mip_width = TextureUtil::CalculateLevelSize(width, level);
      const u32 mip_height = TextureUtil::CalculateLevelSize(height, level);
      const u32 expanded_mip_width = Common::AlignUpSizePow2(mip_width, bsw);
      const u32 expanded_mip_height = Common::AlignUpSizePow2(mip_height, bsh);
      const u32 mip_size =
          TexDecoder::GetTextureSizeInBytes(expanded_mip_width, expanded_mip_height, texformat);
      const u8*& mip_src_data = from_tmem ? ((level % 2) ? ptr_odd : ptr_even) : src_data;
      if (decode_on_gpu)
      {
        u32 row_stride = bytes_per_block * (expanded_mip_width / bsw);
        decode_on_gpu = DecodeTextureOnGPU(
            entry->texture.get(), level, mip_src_data, mip_size,
            static_cast<TextureFormat>(texformat), mip_width, mip_height, expanded_mip_width,
            expanded_mip_height, row_stride, &texMem[tlutaddr], static_cast<TlutFormat>(tlutfmt));
      }
      if (!decode_on_gpu)
      {
        u8* texturedata = TextureCacheBase::temp;
        u32 twidth = mip_width;
        u32 theight = mip_height;
        u32 texpandedWidth = expanded_mip_width;
        TexDecoder::Decode(texturedata, mip_src_data, expanded_mip_width, expanded_mip_height,
                           texformat, tlutaddr, static_cast<TlutFormat>(tlutfmt),
                           PC_TEX_FMT_RGBA32 == config.pcformat,
                           config.pcformat >= PC_TEX_FMT_DXT1);
        if (use_scaling)
        {
          texturedata =
              reinterpret_cast<u8*>(m_scaler->Scale((u32*)texturedata, expandedWidth, height));
          twidth *= g_ActiveConfig.iTexScalingFactor;
          theight *= g_ActiveConfig.iTexScalingFactor;
          texpandedWidth *= g_ActiveConfig.iTexScalingFactor;
        }
        entry->texture->Load(texturedata, twidth, theight, texpandedWidth, level, 0);
      }
      mip_src_data +=
          TexDecoder::GetTextureSizeInBytes(expanded_mip_width, expanded_mip_height, texformat);

      if (g_ActiveConfig.bDumpTextures)
        DumpTexture(entry, basename, level);
    }
  }

  INCSTAT(stats.numTexturesCreated);
  SETSTAT(stats.numTexturesAlive, textures_by_address.size());
  entry = DoPartialTextureUpdates(iter->second, tlutaddr, tlutfmt, palette_size);
  return ReturnEntry(stage, entry);
}

void TextureCacheBase::CopyRenderTargetToTexture(u32 dstAddr, u32 dstFormat, u32 dstStride,
                                                 bool is_depth_copy, const EFBRectangle& srcRect,
                                                 bool isIntensity, bool scaleByHalf)
{
  // Emulation methods:
  //
  // - EFB to RAM:
  //		Encodes the requested EFB data at its native resolution to the emulated RAM using shaders.
  //		Load() decodes the data from there again (using TextureDecoder) if the EFB copy is being
  //used as a texture again. 		Advantage: CPU can read data from the EFB copy and we don't lose any
  //important updates to the texture 		Disadvantage: Encoding+decoding steps often are redundant
  //because only some games read or modify EFB copies before using them as textures.
  //
  // - EFB to texture:
  //		Copies the requested EFB data to a texture object in VRAM, performing any color conversion
  //using shaders. 		Advantage:	Works for many games, since in most cases EFB copies aren't read or
  //modified at all before being used as a texture again. 					Since we don't do any further encoding or
  //decoding here, this method is much faster. 					It also allows enhancing the visual quality by doing
  //scaled EFB copies.
  //
  // - Hybrid EFB copies:
  //		1a) Whenever this function gets called, encode the requested EFB data to RAM (like EFB to
  //RAM) 		1b) Set type to TCET_EC_DYNAMIC for all texture cache entries in the destination address
  //range. 			If EFB copy caching is enabled, further checks will (try to) prevent redundant EFB
  //copies. 		2) Check if a texture cache entry for the specified dstAddr already exists (i.e. if an
  //EFB copy was triggered to that address before): 		2a) Entry doesn't exist:
  //			- Also copy the requested EFB data to a texture object in VRAM (like EFB to texture)
  //			- Create a texture cache entry for the target (type = TCET_EC_VRAM)
  //			- Store a hash of the encoded RAM data in the texcache entry.
  //		2b) Entry exists AND type is TCET_EC_VRAM:
  //			- Like case 2a, but reuse the old texcache entry instead of creating a new one.
  //		2c) Entry exists AND type is TCET_EC_DYNAMIC:
  //			- Only encode the texture to RAM (like EFB to RAM) and store a hash of the encoded data in
  //the existing texcache entry.
  //			- Do NOT copy the requested EFB data to a VRAM object. Reason: the texture is dynamic,
  //i.e. the CPU is modifying it. Storing a VRAM copy is useless, because we'd always end up
  //deleting it and reloading the data from RAM anyway. 		3) If the EFB copy gets used as a texture,
  //compare the source RAM hash with the hash you stored when encoding the EFB data to RAM. 		3a) If
  //the two hashes match AND type is TCET_EC_VRAM, reuse the VRAM copy you created 		3b) If the two
  //hashes differ AND type is TCET_EC_VRAM, screw your existing VRAM copy. Set type to
  //TCET_EC_DYNAMIC. 			Redecode the source RAM data to a VRAM object. The entry basically behaves like
  //a normal texture now. 		3c) If type is TCET_EC_DYNAMIC, treat the EFB copy like a normal texture.
  //		Advantage: 	Non-dynamic EFB copies can be visually enhanced like with EFB to texture.
  //					Compatibility is as good as EFB to RAM.
  //		Disadvantage:	Slower than EFB to texture and often even slower than EFB to RAM.
  //						EFB copy cache depends on accurate texture hashing being enabled. However, with
  //accurate hashing you end up being as slow as without a copy cache anyway.
  //
  // Disadvantage of all methods: Calling this function requires the GPU to perform a pipeline flush
  // which stalls any further CPU processing.
  //
  // For historical reasons, Dolphin doesn't actually implement "pure" EFB to RAM emulation, but
  // only EFB to texture and hybrid EFB copies.

  float colmat[28] = {0};
  float* const fConstAdd = colmat + 16;
  float* const ColorMask = colmat + 20;
  ColorMask[0] = ColorMask[1] = ColorMask[2] = ColorMask[3] = 255.0f;
  ColorMask[4] = ColorMask[5] = ColorMask[6] = ColorMask[7] = 1.0f / 255.0f;
  u32 cbufid = -1;
  auto srcFormat = bpmem.zcontrol.pixel_format.Value();
  bool efbHasAlpha = srcFormat == PEControl::RGBA6_Z24;

  if (is_depth_copy)
  {
    switch (dstFormat)
    {
    case 0:  // Z4
      colmat[3] = colmat[7] = colmat[11] = colmat[15] = 1.0f;
      cbufid = 0;
      dstFormat |= _GX_TF_CTF;
      break;
    case 8:  // Z8H
      dstFormat |= _GX_TF_CTF;
    case 1:  // Z8
      colmat[0] = colmat[4] = colmat[8] = colmat[12] = 1.0f;
      cbufid = 1;
      break;

    case 3:  // Z16
      colmat[1] = colmat[5] = colmat[9] = colmat[12] = 1.0f;
      cbufid = 2;
      break;

    case 11:  // Z16 (reverse order)
      colmat[0] = colmat[4] = colmat[8] = colmat[13] = 1.0f;
      cbufid = 3;
      dstFormat |= _GX_TF_CTF;
      break;

    case 6:  // Z24X8
      colmat[0] = colmat[5] = colmat[10] = 1.0f;
      cbufid = 4;
      break;

    case 9:  // Z8M
      colmat[1] = colmat[5] = colmat[9] = colmat[13] = 1.0f;
      cbufid = 5;
      dstFormat |= _GX_TF_CTF;
      break;

    case 10:  // Z8L
      colmat[2] = colmat[6] = colmat[10] = colmat[14] = 1.0f;
      cbufid = 6;
      dstFormat |= _GX_TF_CTF;
      break;

    case 12:  // Z16L - copy lower 16 depth bits
              // expected to be used as an IA8 texture (upper 8 bits stored as intensity, lower 8
              // bits stored as alpha) Used e.g. in Zelda: Skyward Sword
      colmat[1] = colmat[5] = colmat[9] = colmat[14] = 1.0f;
      cbufid = 7;
      dstFormat |= _GX_TF_CTF;
      break;

    default:
      ERROR_LOG(VIDEO, "Unknown copy zbuf format: 0x%x", dstFormat);
      colmat[2] = colmat[5] = colmat[8] = 1.0f;
      cbufid = 8;
      break;
    }
    dstFormat |= _GX_TF_ZTF;
  }
  else if (isIntensity)
  {
    fConstAdd[0] = fConstAdd[1] = fConstAdd[2] = 16.0f / 255.0f;
    switch (dstFormat)
    {
    case 0:  // I4
    case 1:  // I8
    case 2:  // IA4
    case 3:  // IA8
    case 8:  // I8
             // TODO - verify these coefficients
      colmat[0] = 0.257f;
      colmat[1] = 0.504f;
      colmat[2] = 0.098f;
      colmat[4] = 0.257f;
      colmat[5] = 0.504f;
      colmat[6] = 0.098f;
      colmat[8] = 0.257f;
      colmat[9] = 0.504f;
      colmat[10] = 0.098f;

      if (dstFormat < 2 || dstFormat == 8)
      {
        colmat[12] = 0.257f;
        colmat[13] = 0.504f;
        colmat[14] = 0.098f;
        fConstAdd[3] = 16.0f / 255.0f;
        if (dstFormat == 0)
        {
          ColorMask[0] = ColorMask[1] = ColorMask[2] = 255.0f / 16.0f;
          ColorMask[4] = ColorMask[5] = ColorMask[6] = 1.0f / 15.0f;
          cbufid = 9;
        }
        else
        {
          cbufid = 10;
        }
      }
      else  // alpha
      {
        colmat[15] = 1;
        if (dstFormat == 2)
        {
          ColorMask[0] = ColorMask[1] = ColorMask[2] = ColorMask[3] = 255.0f / 16.0f;
          ColorMask[4] = ColorMask[5] = ColorMask[6] = ColorMask[7] = 1.0f / 15.0f;
          cbufid = 11;
          if (!efbHasAlpha)
          {
            ColorMask[3] = 0.0f;
            fConstAdd[3] = 1.0f;
            cbufid = 12;
          }
        }
        else
        {
          cbufid = 13;
          if (!efbHasAlpha)
          {
            ColorMask[3] = 0.0f;
            fConstAdd[3] = 1.0f;
            cbufid = 14;
          }
        }
      }
      break;

    default:
      ERROR_LOG(VIDEO, "Unknown copy intensity format: 0x%x", dstFormat);
      colmat[0] = colmat[5] = colmat[10] = colmat[15] = 1.0f;
      cbufid = 32;
      break;
    }
  }
  else
  {
    switch (dstFormat)
    {
    case 0:  // R4
      colmat[0] = colmat[4] = colmat[8] = colmat[12] = 1;
      ColorMask[0] = 255.0f / 16.0f;
      ColorMask[4] = 1.0f / 15.0f;
      cbufid = 15;
      dstFormat |= _GX_TF_CTF;
      break;
    case 1:  // R8
    case 8:  // R8
      colmat[0] = colmat[4] = colmat[8] = colmat[12] = 1;
      cbufid = 16;
      dstFormat = GX_CTF_R8;
      break;

    case 2:  // RA4
      colmat[0] = colmat[4] = colmat[8] = colmat[15] = 1.0f;
      ColorMask[0] = ColorMask[3] = 255.0f / 16.0f;
      ColorMask[4] = ColorMask[7] = 1.0f / 15.0f;
      cbufid = 17;
      if (!efbHasAlpha)
      {
        ColorMask[3] = 0.0f;
        fConstAdd[3] = 1.0f;
        cbufid = 18;
      }
      dstFormat |= _GX_TF_CTF;
      break;
    case 3:  // RA8
      colmat[0] = colmat[4] = colmat[8] = colmat[15] = 1.0f;
      cbufid = 19;
      if (!efbHasAlpha)
      {
        ColorMask[3] = 0.0f;
        fConstAdd[3] = 1.0f;
        cbufid = 20;
      }
      dstFormat |= _GX_TF_CTF;
      break;

    case 7:  // A8
      colmat[3] = colmat[7] = colmat[11] = colmat[15] = 1.0f;
      cbufid = 21;
      if (!efbHasAlpha)
      {
        ColorMask[3] = 0.0f;
        fConstAdd[0] = 1.0f;
        fConstAdd[1] = 1.0f;
        fConstAdd[2] = 1.0f;
        fConstAdd[3] = 1.0f;
        cbufid = 22;
      }
      dstFormat |= _GX_TF_CTF;
      break;

    case 9:  // G8
      colmat[1] = colmat[5] = colmat[9] = colmat[13] = 1.0f;
      cbufid = 23;
      dstFormat |= _GX_TF_CTF;
      break;
    case 10:  // B8
      colmat[2] = colmat[6] = colmat[10] = colmat[14] = 1.0f;
      cbufid = 24;
      dstFormat |= _GX_TF_CTF;
      break;

    case 11:  // RG8
      colmat[0] = colmat[4] = colmat[8] = colmat[13] = 1.0f;
      cbufid = 25;
      dstFormat |= _GX_TF_CTF;
      break;

    case 12:  // GB8
      colmat[1] = colmat[5] = colmat[9] = colmat[14] = 1.0f;
      cbufid = 26;
      dstFormat |= _GX_TF_CTF;
      break;

    case 4:  // RGB565
      colmat[0] = colmat[5] = colmat[10] = 1.0f;
      ColorMask[0] = ColorMask[2] = 255.0f / 8.0f;
      ColorMask[4] = ColorMask[6] = 1.0f / 31.0f;
      ColorMask[1] = 255.0f / 4.0f;
      ColorMask[5] = 1.0f / 63.0f;
      fConstAdd[3] = 1.0f;  // set alpha to 1
      cbufid = 27;
      break;

    case 5:  // RGB5A3
      colmat[0] = colmat[5] = colmat[10] = colmat[15] = 1.0f;
      ColorMask[0] = ColorMask[1] = ColorMask[2] = 255.0f / 8.0f;
      ColorMask[4] = ColorMask[5] = ColorMask[6] = 1.0f / 31.0f;
      ColorMask[3] = 255.0f / 32.0f;
      ColorMask[7] = 1.0f / 7.0f;
      cbufid = 28;
      if (!efbHasAlpha)
      {
        ColorMask[3] = 0.0f;
        fConstAdd[3] = 1.0f;
        cbufid = 29;
      }
      break;
    case 6:  // RGBA8
      colmat[0] = colmat[5] = colmat[10] = colmat[15] = 1.0f;
      cbufid = 30;
      if (!efbHasAlpha)
      {
        ColorMask[3] = 0.0f;
        fConstAdd[3] = 1.0f;
        cbufid = 31;
      }
      break;
    default:
      ERROR_LOG(VIDEO, "Unknown copy color format: 0x%x", dstFormat);
      colmat[0] = colmat[5] = colmat[10] = colmat[15] = 1.0f;
      cbufid = 32;
      break;
    }
  }

  u8* dst = Memory::GetPointer(dstAddr);
  if (dst == nullptr)
  {
    ERROR_LOG(VIDEO, "Trying to copy from EFB to invalid address 0x%8x", dstAddr);
    return;
  }

  EFBRectangle clampedRect;
  clampedRect.left = std::min(srcRect.left, (int)EFB_WIDTH);
  clampedRect.right = std::min(srcRect.right, (int)EFB_WIDTH);
  clampedRect.top = std::min(srcRect.top, (int)EFB_HEIGHT);
  clampedRect.bottom = std::min(srcRect.bottom, (int)EFB_HEIGHT);

  const u32 tex_w = srcRect.GetWidth() / (scaleByHalf ? 2 : 1);
  const u32 tex_h = srcRect.GetHeight() / (scaleByHalf ? 2 : 1);

  u32 c_tex_w = clampedRect.GetWidth() / (scaleByHalf ? 2 : 1);
  u32 c_tex_h = clampedRect.GetHeight() / (scaleByHalf ? 2 : 1);

  u32 scaled_tex_w = tex_w;
  u32 scaled_tex_h = tex_h;

  if (g_ActiveConfig.bCopyEFBScaled)
  {
    scaled_tex_w = g_renderer->EFBToScaledX(tex_w);
    scaled_tex_h = g_renderer->EFBToScaledY(tex_h);
    c_tex_w = g_renderer->EFBToScaledX(c_tex_w);
    c_tex_h = g_renderer->EFBToScaledY(c_tex_h);
  }

  // remove all texture cache entries at dstAddr
  {
    auto iter_range = textures_by_address.equal_range(dstAddr);
    TexAddrCache::iterator iter = iter_range.first;
    while (iter != iter_range.second)
    {
      iter = InvalidateTexture(iter);
    }
  }

  // Get the base (in memory) format of this efb copy.
  u32 baseFormat = TexDecoder::GetEfbCopyBaseFormat(dstFormat);

  u32 blockH = TexDecoder::GetBlockHeightInTexels(baseFormat);
  const u32 blockW = TexDecoder::GetBlockWidthInTexels(baseFormat);

  // Round up source height to multiple of block size
  u32 actualHeight = Common::AlignUpSizePow2(tex_h, blockH);
  const u32 actualWidth = Common::AlignUpSizePow2(tex_w, blockW);

  u32 num_blocks_y = actualHeight / blockH;
  const u32 num_blocks_x = actualWidth / blockW;

  // RGBA takes two cache lines per block; all others take one
  const u32 bytes_per_block = baseFormat == GX_TF_RGBA8 ? 64 : 32;

  const u32 bytes_per_row = num_blocks_x * bytes_per_block;
  const u32 covered_range = num_blocks_y * dstStride;

  // temporal fix for hangs when trying to copy larger buffers that the actual efb size
  bool copy_to_ram = !g_ActiveConfig.bSkipEFBCopyToRam && srcRect.GetWidth() <= EFB_WIDTH &&
                     srcRect.GetHeight() <= EFB_HEIGHT;
  if (g_ActiveConfig.bLastStoryEFBToRam)
  {
    // mimimi085181: Ugly speedhack for the Last Story
    copy_to_ram = copy_to_ram || ((tex_w == 64 || tex_w == 128 || tex_w == 256) && !isIntensity &&
                                  tex_h != 1 && (dstFormat == 6 || dstFormat == 32));
  }

  bool copy_to_vram = true;
  // Only apply triggered post-processing on specific formats, to avoid false positives.
  // Skip depth copies, single-channel textures (basically RGB565/RGB5A3/RGBA8 only)
  if (g_ActiveConfig.backend_info.bSupportsPostProcessing && g_renderer->GetPostProcessor())
  {
    if (!is_depth_copy && !isIntensity && dstFormat >= 4 && dstFormat <= 6)
    {
      const TargetRectangle targetSource = g_renderer->ConvertEFBRectangle(clampedRect);
      g_renderer->GetPostProcessor()->OnEFBCopy(&targetSource);
    }
  }
  if (copy_to_ram)
  {
    EFBCopyFormat format(srcFormat, static_cast<TextureFormat>(dstFormat));
    CopyEFB(dst, format, tex_w, bytes_per_row, num_blocks_y, dstStride, is_depth_copy, srcRect,
            scaleByHalf);
  }
  else
  {
    // Hack: Most games don't actually need the correct texture data in RAM
    //       and we can just keep a copy in VRAM. We zero the memory so we
    //       can check it hasn't changed before using our copy in VRAM.
    u8* ptr = dst;
    for (u32 i = 0; i < num_blocks_y; i++)
    {
      memset(ptr, 0, bytes_per_row);
      ptr += dstStride;
    }
  }

  if (g_bRecordFifoData)
  {
    // Mark the memory behind this efb copy as dynamicly generated for the Fifo log
    u32 address = dstAddr;
    for (u32 i = 0; i < num_blocks_y; i++)
    {
      FifoRecorder::GetInstance().UseMemory(address, bytes_per_row, MemoryUpdate::TEXTURE_MAP,
                                            true);
      address += dstStride;
    }
  }

  if (dstStride < bytes_per_row)
  {
    // This kind of efb copy results in a scrambled image.
    // I'm pretty sure no game actually wants to do this, it might be caused by a
    // programming bug in the game, or a CPU/Bounding box emulation issue with dolphin.
    // The copy_to_ram code path above handles this "correctly" and scrambles the image
    // but the copy_to_vram code path just saves and uses unscrambled texture instead.

    // To avoid a "incorrect" result, we simply skip doing the copy_to_vram code path
    // so if the game does try to use the scrambled texture, dolphin will grab the scrambled
    // texture (or black if copy_to_ram is also disabled) out of ram.
    ERROR_LOG(VIDEO, "Memory stride too small (%i < %i)", dstStride, bytes_per_row);
    copy_to_vram = false;
  }

  // Invalidate all textures that overlap the range of our efb copy.
  // Unless our efb copy has a weird stride, then we mark them to check for partial texture updates.
  // TODO: This also invalidates partial overlaps, which we currently don't have a better way
  //       of dealing with.
  bool invalidate_textures = dstStride == bytes_per_row || !copy_to_vram;
  auto iter = FindOverlappingTextures(dstAddr, covered_range);
  while (iter.first != iter.second)
  {
    TCacheEntry* entry = iter.first->second;
    if (entry->OverlapsMemoryRange(dstAddr, covered_range))
    {
      if (invalidate_textures)
      {
        iter.first = InvalidateTexture(iter.first);
        continue;
      }
      entry->may_have_overlapping_textures = true;
    }
    ++iter.first;
  }

  if (copy_to_vram)
  {
    // create the texture
    TextureConfig config;
    config.rendertarget = true;
    config.pcformat =
        g_ActiveConfig.UseHPFrameBuffer() ? PC_TEX_FMT_RGBA16_FLOAT : PC_TEX_FMT_RGBA32;
    config.width = scaled_tex_w;
    config.height = scaled_tex_h;
    config.layers = FramebufferManagerBase::GetEFBLayers();

    TCacheEntry* entry = AllocateCacheEntry(config, false);

    if (entry)
    {
      entry->SetGeneralParameters(dstAddr, 0, baseFormat);
      entry->SetDimensions(tex_w, tex_h, 1);

      entry->frameCount = FRAMECOUNT_INVALID;
      entry->SetEfbCopy(dstStride);
      entry->is_custom_tex = false;

      CopyEFBToCacheEntry(entry, is_depth_copy, clampedRect, scaleByHalf, cbufid, colmat, c_tex_w,
                          c_tex_h);

      u64 hash = entry->CalculateHash();
      entry->SetHashes(hash, hash);

      if (g_ActiveConfig.bDumpEFBTarget)
      {
        static int count = 0;
        entry->texture->Save(StringFromFormat("%sefb_frame_%i.png",
                                              File::GetUserPath(D_DUMPTEXTURES_IDX).c_str(),
                                              count++),
                             0);
      }

      textures_by_address.emplace(dstAddr, entry);
    }
  }
}

std::unique_ptr<HostTexture> TextureCacheBase::AllocateTexture(const TextureConfig& config)
{
  TexPool::iterator iter = FindMatchingTextureFromPool(config);
  std::unique_ptr<HostTexture> entry;
  if (iter != texture_pool.end())
  {
    entry = std::move(iter->second.texture);
    texture_pool.erase(iter);
  }
  else
  {
    entry = CreateTexture(config);
    texture_pool_memory_usage += config.GetSizeInBytes();
    if (!entry)
      return nullptr;

    INCSTAT(stats.numTexturesCreated);
  }

  return entry;
}

TextureCacheBase::TCacheEntry* TextureCacheBase::AllocateCacheEntry(const TextureConfig& config,
                                                                    bool materialmap, bool luma)
{
  std::unique_ptr<HostTexture> texture = AllocateTexture(config);
  if (!texture)
  {
    return nullptr;
  }
  TCacheEntry* cacheEntry = new TCacheEntry(std::move(texture), materialmap, luma);
  cacheEntry->textures_by_hash_iter = textures_by_hash.end();
  return cacheEntry;
}

void TextureCacheBase::DisposeTexture(std::unique_ptr<HostTexture>& texture)
{
  auto config = texture->GetConfig();
  texture_pool.emplace(config, TexPoolEntry(std::move(texture)));
}

void TextureCacheBase::DisposeCacheEntry(TCacheEntry* entry)
{
  if (entry->textures_by_hash_iter != textures_by_hash.end())
  {
    textures_by_hash.erase(entry->textures_by_hash_iter);
    entry->textures_by_hash_iter = textures_by_hash.end();
  }

  auto config = entry->texture->GetConfig();
  texture_pool.emplace(config, TexPoolEntry(std::move(entry->texture)));
  delete entry;
}

TextureCacheBase::TexPool::iterator
TextureCacheBase::FindMatchingTextureFromPool(const TextureConfig& config)
{
  // Find a texture from the pool that does not have a frameCount of FRAMECOUNT_INVALID.
  // This prevents a texture from being used twice in a single frame with different data,
  // which potentially means that a driver has to maintain two copies of the texture anyway.
  // Render-target textures are fine through, as they have to be generated in a seperated pass.
  // As non-render-target textures are usually static, this should not matter much.
  auto range = texture_pool.equal_range(config);
  auto matching_iter = std::find_if(range.first, range.second, [](const auto& iter) {
    return iter.first.rendertarget || iter.second.frameCount != FRAMECOUNT_INVALID;
  });
  return matching_iter != range.second ? matching_iter : texture_pool.end();
}

TextureCacheBase::TexAddrCache::iterator
TextureCacheBase::GetTexCacheIter(TextureCacheBase::TCacheEntry* entry)
{
  auto iter_range = textures_by_address.equal_range(entry->addr);
  TexAddrCache::iterator iter = iter_range.first;

  while (iter != iter_range.second)
  {
    if (iter->second == entry)
    {
      return iter;
    }
    ++iter;
  }
  return textures_by_address.end();
}

TextureCacheBase::TexAddrCache::iterator
TextureCacheBase::InvalidateTexture(TexAddrCache::iterator iter)
{
  if (iter == textures_by_address.end())
    return textures_by_address.end();
  for (size_t i = 0; i < bound_textures.size(); ++i)
  {
    // If the entry is currently bound and not invalidated, keep it, but mark it as invalidated.
    // This way it can still be used via tmem cache emulation, but nothing else.
    // Spyro: A Hero's Tail is known for using such overwritten textures.
    if (bound_textures[i] == iter->second && IsValidBindPoint(static_cast<u32>(i)))
    {
      bound_textures[i]->tmem_only = true;
      return ++iter;
    }
  }
  DisposeCacheEntry(iter->second);
  return textures_by_address.erase(iter);
}

std::pair<TextureCacheBase::TexAddrCache::iterator, TextureCacheBase::TexAddrCache::iterator>
TextureCacheBase::FindOverlappingTextures(u32 addr, u32 size_in_bytes)
{
  // We index by the starting address only, so there is no way to query all textures
  // which end after the given addr. But the GC textures have a limited size, so we
  // look for all textures which have a start address bigger than addr minus the maximal
  // texture size. But this yields false-positives which must be checked later on.

  // 1024 x 1024 texel times 8 nibbles per texel
  constexpr u32 max_texture_size = 1024 * 1024 * 4;
  u32 lower_addr = addr > max_texture_size ? addr - max_texture_size : 0;
  auto begin = textures_by_address.lower_bound(lower_addr);
  auto end = textures_by_address.upper_bound(addr + size_in_bytes);

  return std::make_pair(begin, end);
}

u32 TextureCacheBase::TCacheEntry::BytesPerRow() const
{
  const u32 blockW = TexDecoder::GetBlockWidthInTexels(format);

  // Round up source height to multiple of block size
  const u32 actualWidth = Common::AlignUpSizePow2(native_width, blockW);

  const u32 numBlocksX = actualWidth / blockW;

  // RGBA takes two cache lines per block; all others take one
  const u32 bytes_per_block = (format & 0xF) == GX_TF_RGBA8 ? 64 : 32;

  return numBlocksX * bytes_per_block;
}

u32 TextureCacheBase::TCacheEntry::NumBlocksY() const
{
  u32 blockH = TexDecoder::GetBlockHeightInTexels(format);
  // Round up source height to multiple of block size
  u32 actualHeight = Common::AlignUpSizePow2(native_height, blockH);

  return actualHeight / blockH;
}

void TextureCacheBase::TCacheEntry::SetEfbCopy(u32 stride)
{
  is_efb_copy = true;
  memory_stride = stride;

  ASSERT_MSG(VIDEO, memory_stride >= BytesPerRow(), "Memory stride is too small");

  size_in_bytes = memory_stride * NumBlocksY();
}

u64 TextureCacheBase::TCacheEntry::CalculateHash() const
{
  u8* ptr = Memory::GetPointer(addr);
  if (memory_stride == BytesPerRow())
  {
    return GetHash64(ptr, size_in_bytes, g_ActiveConfig.iSafeTextureCache_ColorSamples);
  }
  else
  {
    u32 blocks = NumBlocksY();
    u64 temp_hash = size_in_bytes;

    u32 samples_per_row = 0;
    if (g_ActiveConfig.iSafeTextureCache_ColorSamples != 0)
    {
      // Hash at least 4 samples per row to avoid hashing in a bad pattern, like just on the left
      // side of the efb copy
      samples_per_row = std::max(g_ActiveConfig.iSafeTextureCache_ColorSamples / blocks, 4u);
    }

    for (u32 i = 0; i < blocks; i++)
    {
      // Multiply by a prime number to mix the hash up a bit. This prevents identical blocks from
      // canceling each other out
      temp_hash = (temp_hash * 397) ^ GetHash64(ptr, BytesPerRow(), samples_per_row);
      ptr += memory_stride;
    }
    return temp_hash;
  }
}
