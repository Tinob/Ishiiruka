// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include "VideoBackends/DX11/D3DTexture.h"
#include "VideoBackends/DX11/TextureEncoder.h"
#include "VideoCommon/TextureCacheBase.h"

namespace DX11
{

class TextureCache : public ::TextureCacheBase
{
public:
  TextureCache();
  ~TextureCache();

private:

  HostTextureFormat GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height) override;

  void CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
    bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height) override;

  bool DecodeTextureOnGPU(HostTexture* dst, u32 dst_level, const u8* data,
    u32 data_size, TextureFormat format, u32 width, u32 height,
    u32 aligned_width, u32 aligned_height, u32 row_stride,
    const u8* palette, TlutFormat palette_format) override;

  std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) override;

  void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
    u32 num_blocks_y, u32 memory_stride, bool is_depth_copy,
    const EFBRectangle& src_rect, bool scale_by_half) override;
  bool Palettize(TCacheEntry* entry, const TCacheEntry* base_entry) override;
  void LoadLut(u32 lutFmt, void* addr, u32 size) override;
  bool SupportsGPUTextureDecode(TextureFormat format, TlutFormat palette_format) override;
  bool CompileShaders() override
  {
    return true;
  }
  void DeleteShaders() override
  {}
};

}
