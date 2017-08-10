// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <bitset>
#include <map>
#include <memory>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "Common/CommonTypes.h"
#include "Common/Thread.h"

#include "VideoCommon/BPMemory.h"
#include "VideoCommon/HostTexture.h"
#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/TextureConfig.h"
#include "VideoCommon/VideoCommon.h"

struct VideoConfig;
class TextureScaler;

enum TextureCacheParams
{
  // ugly
  TEXHASH_INVALID = 0,
  FRAMECOUNT_INVALID = 0,
  TEXTURE_KILL_MULTIPLIER = 2,
  TEXTURE_KILL_THRESHOLD = 120,
  TEXTURE_POOL_KILL_THRESHOLD = 3,
  TEXTURE_POOL_MEMORY_LIMIT = 64 * 1024 * 1024
};

class TextureCacheBase
{
public: 
  enum TCacheEntryGroupIndex : u32
  {
    Color = 0,
    Material = 1
  };

  struct TCacheEntry
  {
    std::vector<std::unique_ptr<HostTexture>> textures;
    HostTexture* GetColor() const
    {
      return textures[TCacheEntryGroupIndex::Color].get();
    }
    HostTexture* GetMaterial() const
    {
      if (material_map)
      {
        return textures[TCacheEntryGroupIndex::Material].get();
      }
      return nullptr;
    }
    // common members    
    u32 addr = {};
    u32 size_in_bytes = {};
    u32 format = {}; // bits 0-3 will contain the in-memory format.
    u32 memory_stride = {};
    u32 native_width = {}, native_height = {}; // Texture dimensions from the GameCube's point of view
    u32 native_levels = {};
    // used to delete textures which haven't been used for TEXTURE_KILL_THRESHOLD frames
    s32 frameCount = FRAMECOUNT_INVALID;
    u64 hash = {};
    u64 base_hash = {};
    bool is_efb_copy = false;
    bool is_custom_tex = false;
    bool material_map = false;
    bool is_scaled = false;
    bool emissive_in_alpha = false;
    bool may_have_overlapping_textures = true;
    bool tmem_only = false;  // indicates that this texture only exists in the tmem cache

    // Keep an iterator to the entry in textures_by_hash, so it does not need to be searched when
    // removing the cache entry
    std::multimap<u64, TCacheEntry*>::iterator textures_by_hash_iter;

    // This is used to keep track of both:
    //   * efb copies used by this partially updated texture
    //   * partially updated textures which refer to this efb copy
    std::unordered_set<TCacheEntry*> references;

    std::string basename;

    explicit TCacheEntry(std::unique_ptr<HostTexture> tex);
    explicit TCacheEntry(std::unique_ptr<HostTexture> tex, std::unique_ptr<HostTexture> material);

    ~TCacheEntry();

    void SetGeneralParameters(u32 _addr, u32 _size, u32 _format)
    {
      addr = _addr;
      size_in_bytes = _size;
      format = _format;
    }

    void SetDimensions(u32 _native_width, u32 _native_height, u32 _native_levels)
    {
      native_width = _native_width;
      native_height = _native_height;
      native_levels = _native_levels;
      memory_stride = _native_width;
    }

    void SetHiresParams(bool _is_custom_tex, const std::string & _basename, bool _is_scaled, bool _emissive_in_alpha)
    {
      is_custom_tex = _is_custom_tex;
      basename = _basename;
      is_scaled = _is_scaled;
      emissive_in_alpha = _emissive_in_alpha;
    }

    void SetHashes(u64 _hash, u64 _base_hash)
    {
      hash = _hash;
      base_hash = _base_hash;
    }

    // This texture entry is used by the other entry as a sub-texture
    void CreateReference(TCacheEntry* other_entry)
    {
      // References are two-way, so they can easily be destroyed later
      this->references.emplace(other_entry);
      other_entry->references.emplace(this);
    }

    void DestroyAllReferences()
    {
      for (auto& reference : references)
        reference->references.erase(this);

      references.clear();
    }

    void SetEfbCopy(u32 stride);

    bool OverlapsMemoryRange(u32 range_address, u32 range_size) const;

    bool IsEfbCopy() const
    {
      return is_efb_copy;
    }

    u32 NumBlocksY() const;
    u32 BytesPerRow() const;

    u64 CalculateHash() const;
    const TextureConfig GetConfig() const { return textures[TCacheEntryGroupIndex::Color]->GetConfig(); }
  };

  virtual ~TextureCacheBase(); // needs virtual for DX11 dtor

  void OnConfigChanged(VideoConfig& config);
  // Removes textures which aren't used for more than TEXTURE_KILL_THRESHOLD frames,
  // frameCount is the current frame number.
  void Cleanup(s32 _frameCount);
  void Invalidate();

  virtual HostTextureFormat GetHostTextureFormat(const s32 texformat,
    const TlutFormat tlutfmt, u32 width, u32 height) = 0;

  virtual bool Palettize(TCacheEntry* entry, const TCacheEntry* base_entry) = 0;
  virtual void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
    u32 num_blocks_y, u32 memory_stride,
    bool is_depth_copy, const EFBRectangle& src_rect, bool scale_by_half) = 0;  

  virtual bool CompileShaders() = 0; // currently only implemented by OGL
  virtual void DeleteShaders() = 0; // currently only implemented by OGL

  virtual void LoadLut(u32 lutFmt, void* addr, u32 size) = 0;

  TCacheEntry* Load(const u32 stage);
  void InvalidateAllBindPoints() { valid_bind_points.reset(); }
  bool IsValidBindPoint(u32 i) const { return valid_bind_points.test(i); }
  virtual void BindTextures();
  void CopyRenderTargetToTexture(u32 dstAddr, u32 dstFormat, u32 dstStride,
    bool is_depth_copy, const EFBRectangle& srcRect, bool isIntensity, bool scaleByHalf);
  u8* GetTemporalBuffer()
  {
    return temp;
  }
  // Returns true if the texture data and palette formats are supported by the GPU decoder.
  virtual bool SupportsGPUTextureDecode(TextureFormat format, TlutFormat palette_format)
  {
    return false;
  }
  // Decodes the specified data to the GPU texture specified by entry.
  // width, height are the size of the image in pixels.
  // aligned_width, aligned_height are the size of the image in pixels, aligned to the block size.
  // row_stride is the number of bytes for a row of blocks, not pixels.
  virtual bool DecodeTextureOnGPU(HostTexture* dst, u32 dst_level, const u8* data,
    u32 data_size, TextureFormat tformat, u32 width, u32 height,
    u32 aligned_width, u32 aligned_height, u32 row_stride,
    const u8* palette, TlutFormat palette_format)
  {
    return false;
  }
  std::unique_ptr<HostTexture> AllocateTexture(const TextureConfig& config);
  void DisposeTexture(std::unique_ptr<HostTexture>& texture);
protected:
  alignas(16) u8 *temp = {};
  size_t temp_size = {};
  std::array<TCacheEntry*, 8> bound_textures{};
  std::bitset<8> valid_bind_points;
  TextureCacheBase();
private:
  virtual std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) = 0; 
  virtual void CopyEFBToCacheEntry(TCacheEntry* entry, bool is_depth_copy,
    const EFBRectangle& src_rect, bool scale_by_half,
    unsigned int cbuf_id, const float* colmat, u32 width, u32 height) = 0;
  // Minimal version of TCacheEntry just for TexPool
  struct TexPoolEntry
  {
    std::unique_ptr<HostTexture> texture;
    s32 frameCount = FRAMECOUNT_INVALID;
    TexPoolEntry(std::unique_ptr<HostTexture> tex) : texture(std::move(tex)) {}
  };
  typedef std::multimap<u32, TCacheEntry*> TexAddrCache;
  typedef std::multimap<u64, TCacheEntry*> TexHashCache;
  typedef std::unordered_multimap<TextureConfig, TexPoolEntry, TextureConfig::Hasher> TexPool;  

  void SetBackupConfig(const VideoConfig& config);
  void ScaleTextureCacheEntryTo(TCacheEntry* entry, u32 new_width, u32 new_height);
  void CheckTempSize(size_t required_size);

  
  TCacheEntry* DoPartialTextureUpdates(TCacheEntry* entry_to_update, u32 tlutaddr, u32 tlutfmt, u32 palette_size);
  TCacheEntry* ApplyPaletteToEntry(TCacheEntry* entry, u32 tlutaddr, u32 tlutfmt, u32 palette_size);
  void DumpTexture(TCacheEntry* entry, std::string basename, u32 level);

  TCacheEntry* AllocateCacheEntry(const TextureConfig& config, bool materialmap);
  void DisposeCacheEntry(TCacheEntry* texture);

  TexPool::iterator FindMatchingTextureFromPool(const TextureConfig& config);
  TexAddrCache::iterator GetTexCacheIter(TCacheEntry* entry);
  TexAddrCache::iterator InvalidateTexture(TexAddrCache::iterator t_iter);
  TCacheEntry* ReturnEntry(u32 stage, TCacheEntry* entry);

  // Return all possible overlapping textures. As addr+size of the textures is not
  // indexed, this may return false positives.
  std::pair<TexAddrCache::iterator, TexAddrCache::iterator>
    FindOverlappingTextures(u32 addr, u32 size_in_bytes);

  TexAddrCache textures_by_address;
  TexHashCache textures_by_hash;
  TexPool texture_pool;
  size_t texture_pool_memory_usage = {};

  // Backup configuration values
  struct BackupConfig
  {
    s32 colorsamples;
    bool texfmt_overlay;
    bool texfmt_overlay_center;
    bool hires_textures;
    bool cache_hires_textures;
    bool stereo_3d;
    bool efb_mono_depth;
    s32 scaling_mode;
    s32 scaling_factor;
    bool scaling_deposterize;
    bool gpu_texture_decoding;
  };
  BackupConfig backup_config = {};
  std::unique_ptr<TextureScaler> m_scaler;
};

extern std::unique_ptr<TextureCacheBase> g_texture_cache;