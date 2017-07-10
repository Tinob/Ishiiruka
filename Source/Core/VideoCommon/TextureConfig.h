// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <algorithm>
#include <memory>
#include <tuple>

#include "Common/CommonTypes.h"

#include "VideoCommon/TextureDecoder.h"

struct TextureConfig
{
  constexpr TextureConfig() = default;

  u32 GetSizeInBytes() const
  {
    u32 result = 0;
    switch (pcformat)
    {
    case PC_TEX_FMT_RGBA16_FLOAT:
      result = ((width + 3) & (~3)) * ((height + 3) & (~3)) * 8;
      break;
    case PC_TEX_FMT_RGBA_FLOAT:
      result = ((width + 3) & (~3)) * ((height + 3) & (~3)) * 16;
      break;
      break;
    case PC_TEX_FMT_DEPTH_FLOAT:
    case PC_TEX_FMT_R_FLOAT:
    case PC_TEX_FMT_BGRA32:
    case PC_TEX_FMT_RGBA32:
      result = ((width + 3) & (~3)) * ((height + 3) & (~3)) * 4;
      break;
    case PC_TEX_FMT_IA4_AS_IA8:
    case PC_TEX_FMT_IA8:
    case PC_TEX_FMT_RGB565:
      result = ((width + 3) & (~3)) * ((height + 3) & (~3)) * 2;
      break;
    case PC_TEX_FMT_DXT1:
      result = ((width + 3) >> 2)*((height + 3) >> 2) * 8;
      break;
    case PC_TEX_FMT_DXT3:
    case PC_TEX_FMT_DXT5:
      result = ((width + 3) >> 2)*((height + 3) >> 2) * 16;
      break;
    default:
      result = ((width + 3) >> 2)*((height + 3) >> 2);
      break;
    }
    if (levels > 1 || rendertarget)
    {
      result += result * 2;
    }
    if (materialmap)
    {
      result *= 2;
    }
    result = std::max(result, 4096u);
    return result;
  }

  bool operator == (const TextureConfig& o) const
  {
    return std::tie(width, height, levels, layers, rendertarget, pcformat, materialmap) ==
      std::tie(o.width, o.height, o.levels, o.layers, o.rendertarget, o.pcformat, o.materialmap);
  }

  struct Hasher
  {
    size_t operator()(const TextureConfig& c) const
    {
      return (u64)c.materialmap << 57	// 1 bit
        | (u64)c.rendertarget << 56	// 1 bit
        | (u64)c.pcformat << 48		// 8 bits
        | (u64)c.layers << 40		// 8 bits 
        | (u64)c.levels << 32		// 8 bits 
        | (u64)c.height << 16		// 16 bits 
        | (u64)c.width;				// 16 bits
    }
  };

  u32 width = 0, height = 0, levels = 1, layers = 1;
  bool rendertarget = false;
  bool materialmap = false;
  PC_TexFormat pcformat = PC_TEX_FMT_NONE;
};
