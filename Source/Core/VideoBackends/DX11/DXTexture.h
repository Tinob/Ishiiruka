// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonTypes.h"

#include "VideoCommon/HostTexture.h"

class D3DTexture2D;

namespace DX11
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
  static void CopyTexture(D3DTexture2D* source, D3DTexture2D* destination,
    u32 srcwidth, u32 srcheight,
    u32 dstwidth, u32 dstheight);
  static void CopyRectangle(D3DTexture2D* source, D3DTexture2D* destination,
    const MathUtil::Rectangle<int>& srcrect, u32 srcwidth, u32 srcheight,
    const MathUtil::Rectangle<int>& dstrect, u32 dstwidth, u32 dstheight);
  void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) override;

  D3DTexture2D* GetRawTexIdentifier() const;
  uintptr_t GetInternalObject() const override { return reinterpret_cast<uintptr_t>(m_texture); };
private:
  D3DTexture2D* m_texture = nullptr;
  D3D11_USAGE usage = {};
  bool compressed = false;
  bool swap_rg = false;
  bool convertrgb565 = false;
};

}  // namespace DX11
