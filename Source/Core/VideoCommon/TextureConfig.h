// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <algorithm>
#include <memory>
#include <tuple>

#include "Common/CommonTypes.h"
#include "Common/MathUtil.h"

#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/TextureUtil.h"

struct TextureConfig
{
  constexpr TextureConfig() = default;

  u32 GetSizeInBytes() const
  {
    u32 result = TextureUtil::GetTextureSizeInBytes(width, height, pcformat);
    if (levels > 1 || rendertarget)
    {
      result += result * 2;
    }
    result = std::max(result, 4096u);
    return result;
  }

  bool operator == (const TextureConfig& o) const
  {
    return std::tie(width, height, levels, layers, rendertarget, pcformat) ==
      std::tie(o.width, o.height, o.levels, o.layers, o.rendertarget, o.pcformat);
  }

  MathUtil::Rectangle<int> GetRect() const
  {
    return { 0, 0, static_cast<int>(width), static_cast<int>(height) };
  }

  struct Hasher
  {
    size_t operator()(const TextureConfig& c) const
    {
      return (u64)c.rendertarget << 56	// 1 bit
        | (u64)c.pcformat << 48		// 8 bits
        | (u64)c.layers << 40		// 8 bits 
        | (u64)c.levels << 32		// 8 bits 
        | (u64)c.height << 16		// 16 bits 
        | (u64)c.width;				// 16 bits
    }
  };

  u32 width = 0, height = 0, levels = 1, layers = 1;
  bool rendertarget = false;
  HostTextureFormat pcformat = PC_TEX_FMT_NONE;
};
