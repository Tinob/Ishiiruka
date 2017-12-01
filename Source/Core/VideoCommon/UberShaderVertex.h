// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <functional>
#include "VideoCommon/PixelShaderGen.h"

namespace UberShader
{
#pragma pack(1)
struct vertex_ubershader_uid_data
{
  u32 num_texgens : 4;
  u32 per_pixel_lighting : 1;
  u32 unused : 27;
  u32 NumValues() const { return sizeof(vertex_ubershader_uid_data); }
  u32 StartValue() const
  {
    return 0;
  }
  void ClearUnused()
  {
    unused = 0;
  }
};
#pragma pack()
#define VERTEXUBERSHADERGEN_UID_VERSION 1
typedef ShaderUid<vertex_ubershader_uid_data> VertexUberShaderUid;

VertexUberShaderUid GetVertexUberShaderUid(u32 components, const XFMemory &xfr);

void GenVertexShader(ShaderCode& code, API_TYPE api_type, const ShaderHostConfig& host_config,
                           const vertex_ubershader_uid_data& uid_data);
void EnumerateVertexUberShaderUids(const std::function<void(const VertexUberShaderUid&, size_t)>& callback);
}
