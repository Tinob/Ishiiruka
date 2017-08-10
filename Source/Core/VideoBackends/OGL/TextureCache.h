// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>

#include "Common/GL/GLUtil.h"
#include "VideoCommon/BPStructs.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/VideoCommon.h"

namespace OGL
{

class TextureCache : public ::TextureCacheBase
{
public:
  TextureCache();
  ~TextureCache();
  SHADER GetColorCopyProgram() const;
  GLuint GetColorCopyPositionUniform() const;
private:

  HostTextureFormat GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height) override;

  std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) override;

  bool DecodeTextureOnGPU(HostTexture* dst, u32 dst_level, const u8* data,
    u32 data_size, TextureFormat format, u32 width, u32 height,
    u32 aligned_width, u32 aligned_height, u32 row_stride,
    const u8* palette, TlutFormat palette_format) override;

  void CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
    bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height) override;

  void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
    u32 num_blocks_y, u32 memory_stride, bool is_depth_copy,
    const EFBRectangle& src_rect, bool scale_by_half) override;

  bool Palettize(TCacheEntry* entry, const TCacheEntry* base_entry) override;
  void LoadLut(u32 lutFmt, void* addr, u32 size) override;
  bool CompileShaders() override;
  void DeleteShaders() override;
  bool SupportsGPUTextureDecode(TextureFormat format, TlutFormat palette_format) override;
  void* m_last_addr = {};
  u32 m_last_size = {};
  u64 m_last_hash = {};
  u32 m_last_lutFmt = {};
};
}
