// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <functional>
#include "VideoCommon/BPMemory.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/XFMemory.h"

namespace UberShader
{
#pragma pack(1)
struct pixel_ubershader_uid_data
{
  u32 num_texgens : 4;
  u32 early_depth : 1;
  u32 per_pixel_depth : 1;
  u32 per_pixel_lighting : 1;
  u32 uint_output : 1;
  u32 unused : 24;
  u32 NumValues() const { return sizeof(pixel_ubershader_uid_data); }
  u32 StartValue() const
  {
    return 0;
  }

  void ClearUnused()
  {
    uint_output = 0;
    unused = 0;
  }
};
#pragma pack()
#define PIXELUBERSHADERGEN_UID_VERSION 1

typedef ShaderUid<pixel_ubershader_uid_data> PixelUberShaderUid;

PixelUberShaderUid GetPixelUberShaderUid(u32 components, const XFMemory &xfr, const BPMemory &bpm);

void GenPixelShader(ShaderCode& out, API_TYPE ApiType, const ShaderHostConfig& host_config,
                          const pixel_ubershader_uid_data& uid_data);

void EnumeratePixelUberShaderUids(const std::function<void(const PixelUberShaderUid&, size_t)>& callback);
}
