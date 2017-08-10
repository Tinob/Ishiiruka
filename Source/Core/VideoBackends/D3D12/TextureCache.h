// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include <memory>
#include "VideoBackends/D3D12/D3DTexture.h"
#include "VideoCommon/TextureCacheBase.h"

namespace DX12
{

class D3DStreamBuffer;

class TextureCache : public TextureCacheBase
{
public:
  TextureCache();
  ~TextureCache();
  static D3D12_GPU_DESCRIPTOR_HANDLE GetTextureGroupHandle();
  virtual void BindTextures();
private: 

  HostTextureFormat GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height) override;

  void CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
    bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height) override;

  virtual std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) override;
  bool Palettize(TextureCacheBase::TCacheEntry* entry, const TextureCacheBase::TCacheEntry* base_entry) override;
  void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
    u32 num_blocks_y, u32 memory_stride, bool is_depth_copy,
    const EFBRectangle& src_rect, bool scale_by_half) override;
  void LoadLut(u32 lutFmt, void* addr, u32 size) override;
  bool CompileShaders() override
  {
    return true;
  }
  void DeleteShaders() override
  {}

  TlutFormat m_lut_format = {};
  u32 m_lut_size = {};
  void* m_addr = {};
  u64 m_hash = {};
  std::unique_ptr<D3DStreamBuffer> m_palette_stream_buffer;

  ID3D12Resource* m_palette_uniform_buffer = nullptr;
  D3D12_SHADER_BYTECODE m_palette_pixel_shaders[3] = {};
};

}
