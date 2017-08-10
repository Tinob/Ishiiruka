// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>

#include "VideoBackends/DX9/D3DBase.h"

#include "VideoCommon/BPMemory.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/VideoCommon.h"

namespace DX9
{

class TextureCache : public ::TextureCacheBase
{
public:
  TextureCache();
  ~TextureCache();
private:
  HostTextureFormat GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height) override;

  virtual void CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
    bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height) override;

  virtual std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) override;
  bool Palettize(TextureCacheBase::TCacheEntry* entry, const TextureCacheBase::TCacheEntry* base_entry) override;
  void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
    u32 num_blocks_y, u32 memory_stride, bool is_depth_copy,
    const EFBRectangle& src_rect, bool scale_by_half) override;  
  void LoadLut(u32 lutFmt, void* addr, u32 size);
  bool CompileShaders() override
  {
    return true;
  }
  void DeleteShaders() override
  {}
};

}