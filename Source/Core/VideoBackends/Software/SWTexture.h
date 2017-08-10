// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonTypes.h"

#include "VideoCommon/HostTexture.h"

namespace SW
{
class SWTexture final : public HostTexture
{
public:
  explicit SWTexture(const TextureConfig& tex_config);
  ~SWTexture() = default;

  void Bind(unsigned int stage) override;

  void CopyRectangleFromTexture(const HostTexture* source,
    const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect) override;
  void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) override;
  uintptr_t GetInternalObject() const override { return 0; };
};

}  // namespace SW
#pragma once
