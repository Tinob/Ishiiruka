// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonTypes.h"

#include "VideoCommon/HostTexture.h"

class D3DTexture2D;

namespace DX9
{
class DXTexture final : public HostTexture
{
public:
  explicit DXTexture(const TextureConfig& tex_config);
  ~DXTexture();

  void Bind(u32 stage) override;
  bool Save(const std::string& filename, u32 level) override;

  void CopyRectangleFromTexture(const HostTexture* source,
    const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect) override;
  void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) override;

  LPDIRECT3DTEXTURE9 GetRawTexIdentifier() const;
  static void Initialize();
  static void Dispose();
  uintptr_t GetInternalObject() const override { return reinterpret_cast<uintptr_t>(m_texture); };
private:  
  void ReplaceTexture(const u8* src, u32 width, u32 height,
    u32 expanded_width, u32 level, bool swap_r_b);
  LPDIRECT3DTEXTURE9 m_texture = nullptr;
  D3DFORMAT d3d_fmt = {};
  bool compressed = false;
};

}  // namespace DX11
