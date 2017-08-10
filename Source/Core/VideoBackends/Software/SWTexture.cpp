// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoBackends/Software/SWTexture.h"

namespace SW
{
SWTexture::SWTexture(const TextureConfig& tex_config) : HostTexture(tex_config)
{
}

void SWTexture::Bind(unsigned int stage)
{
}

void SWTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
}

void SWTexture::Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level)
{
}

}  // namespace SW
