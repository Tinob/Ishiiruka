// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <cstdarg>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "Common/CommonTypes.h"
#include "Common/FileUtil.h"
#include "Common/StringUtil.h"
#include "Common/Logging/Log.h"
#include "Common/Hash.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/XFMemory.h"

/**
* Shader UID class used to uniquely identify the ShaderCode output written in the shader generator.
* uid_data can be any struct of parameters that uniquely identify each shader code output.
* Unless performance is not an issue, uid_data should be tightly packed to reduce memory footprint.
* Shader generators will write to specific uid_data fields; ShaderUid methods will only read raw u32 values from a union.
*/
template<class uid_data>
class ShaderUid
{
public:
  ShaderUid() : HASH(0)
  {}

  inline void ClearUID()
  {
    data = {};
    HASH = 0;
  }

  inline void ClearHASH()
  {
    HASH = 0;
  }

  inline void CalculateUIDHash()
  {
    if (HASH == 0)
    {
      data.ClearUnused();
      if (data.NumValues() == sizeof(u32))
      {
        HASH = *reinterpret_cast<u32*>(reinterpret_cast<u8*>(&data) + data.StartValue());
      }
      else if (data.NumValues() == sizeof(u64))
      {
        HASH = *reinterpret_cast<u64*>(reinterpret_cast<u8*>(&data) + data.StartValue());
      }
      else
      {
        HASH = (std::size_t)GetMurmurHash3(reinterpret_cast<u8*>(&data) + data.StartValue(), data.NumValues(), 0);
      }
      HASH++;
    }
  }

  bool operator == (const ShaderUid& obj) const
  {
    return data.StartValue() == obj.data.StartValue()
      && data.NumValues() == obj.data.NumValues()
      && !memcmp(reinterpret_cast<const u8*>(&data) + data.StartValue(), reinterpret_cast<const u8*>(&obj.data) + data.StartValue(), data.NumValues());
  }

  bool operator != (const ShaderUid& obj) const
  {
    return data.StartValue() != obj.data.StartValue()
      || data.NumValues() != obj.data.NumValues()
      || !!memcmp(reinterpret_cast<const u8*>(&data) + data.StartValue(), reinterpret_cast<const u8*>(&obj.data) + data.StartValue(), data.NumValues());
  }

  // determines the storage order inside STL containers
  bool operator < (const ShaderUid& obj) const
  {
    return memcmp(reinterpret_cast<const u8*>(&data) + data.StartValue(), reinterpret_cast<const u8*>(&obj.data) + obj.data.StartValue(), std::max(obj.data.NumValues(), data.NumValues())) < 0;
  }

  template<class T>
  inline T& GetUidData()
  {
    return data;
  }

  const uid_data& GetUidData() const
  {
    return data;
  }
  size_t GetUidDataSize() const
  {
    return sizeof(uid_data);
  }
  struct ShaderUidHasher
  {
    std::size_t operator()(const ShaderUid<uid_data>& k) const
    {
      return k.HASH;
    }
  };
private:
  uid_data data;
  std::size_t HASH;
};

class ShaderCode
{
public:
  ShaderCode() { m_buffer.reserve(32768); }
  void Write(const char* fmt, ...)
#ifdef __GNUC__
    __attribute__((format(printf, 2, 3)))
#endif
  {
    va_list arglist;
    va_start(arglist, fmt);
    m_buffer += StringFromFormatV(fmt, arglist);
    va_end(arglist);
}
  void clear()
  {
    m_buffer.clear();
  }
  void copy(const ShaderCode& src)
  {
    m_buffer = src.m_buffer;
  }
  const char* data() const
  {
    return m_buffer.c_str();
  }
  ptrdiff_t size() const
  {
    return m_buffer.size();
  }

protected:
  std::string m_buffer;
};

// Host config contains the settings which can influence generated shaders.
union ShaderHostConfig
{
  u32 bits;

  struct
  {
    u32 msaa : 1;
    u32 ssaa : 1;
    u32 stereo : 1;
    u32 wireframe : 1;
    u32 fast_depth_calc : 1;
    u32 bounding_box : 1;
    u32 backend_dual_source_blend : 1;
    u32 backend_geometry_shaders : 1;
    u32 backend_early_z : 1;
    u32 backend_bbox : 1;
    u32 backend_gs_instancing : 1;
    u32 backend_clip_control : 1;
    u32 backend_ssaa : 1;
    u32 backend_atomics : 1;
    u32 backend_depth_clamp : 1;
    u32 backend_reversed_depth_range : 1;
    u32 backend_bitfield : 1;
    u32 backend_dynamic_sampler_indexing : 1;
    u32 pad : 13;
  };

  static ShaderHostConfig GetCurrent();
};

// Gets the filename of the specified type of cache object (e.g. vertex shader, pipeline).
std::string GetDiskShaderCacheFileName(API_TYPE api_type, const char* type, bool include_gameid,
  bool include_host_config, bool uid = false);

inline void WriteRegister(ShaderCode& object, API_TYPE api_type, const char *prefix, const u32 num)
{
  if (!(api_type & API_D3D9))
    return; // Nothing to do here

  object.Write(" : register(%s%d)", prefix, num);
}

inline void WriteLocation(ShaderCode& object, API_TYPE api_type)
{
  if (!(api_type & API_D3D9))
    return;

  object.Write("uniform ");
}

inline void DeclareUniform(ShaderCode& object, API_TYPE api_type, const u32 num, const char* type, const char* name)
{
  WriteLocation(object, api_type);
  object.Write("%s %s ", type, name);
  WriteRegister(object, api_type, "c", num);
  object.Write(";\n");
}

inline void DefineVSOutputStructMember(ShaderCode& object, API_TYPE api_type, const char* qualifier, const char* type, const char* name, int var_index, const char* semantic, int semantic_index = -1)
{
  if (qualifier != nullptr)
    object.Write("\t%s %s %s", qualifier, type, name);
  else
    object.Write("\t%s %s", type, name);

  if (var_index != -1)
    object.Write("%d", var_index);

  if (api_type == API_OPENGL || api_type == API_VULKAN)
    object.Write(";\n");
  else
  {
    if (semantic_index != -1)
      object.Write(" : %s%d;\n", semantic, semantic_index);
    else
      object.Write(" : %s;\n", semantic);
  }
}

inline void GenerateVSOutputMembers(ShaderCode& object, API_TYPE api_type, const bool enable_pl, const u32 numtexgens, const char* qualifier = nullptr)
{
  DefineVSOutputStructMember(object, api_type, qualifier, "float4", "pos", -1, api_type == API_D3D11 ? "SV_Position" : "POSITION");
  DefineVSOutputStructMember(object, api_type, qualifier, "float4", "colors_", 0, "COLOR", 0);
  DefineVSOutputStructMember(object, api_type, qualifier, "float4", "colors_", 1, "COLOR", 1);

  if (numtexgens < 7)
  {
    for (unsigned int i = 0; i < numtexgens; ++i)
      DefineVSOutputStructMember(object, api_type, qualifier, "float3", "tex", i, "TEXCOORD", i);
    DefineVSOutputStructMember(object, api_type, qualifier, "float4", "clipPos", -1, "TEXCOORD", numtexgens);

    if (enable_pl)
      DefineVSOutputStructMember(object, api_type, qualifier, "float4", "Normal", -1, "TEXCOORD", numtexgens + 1);
  }
  else
  {
    // Store clip position in the w component of first 4 texcoords
    int num_texcoords = enable_pl ? 8 : numtexgens;
    for (int i = 0; i < num_texcoords; ++i)
      DefineVSOutputStructMember(object, api_type, qualifier, (enable_pl || i < 4) ? "float4" : "float3", "tex", i, "TEXCOORD", i);
  }
  if (g_ActiveConfig.backend_info.bSupportsDepthClamp)
  {
    DefineVSOutputStructMember(object, api_type, qualifier, "float2", "clipDist", -1, "SV_ClipDistance", 0);
  }
}

inline void AssignVSOutputMembers(ShaderCode& object, API_TYPE api_type, const char* a, const char* b, bool enable_pl, u32 numtexgens)
{
  object.Write("\t%s.pos = %s.pos;\n", a, b);
  object.Write("\t%s.colors_0 = %s.colors_0;\n", a, b);
  object.Write("\t%s.colors_1 = %s.colors_1;\n", a, b);

  if (numtexgens < 7)
  {
    for (unsigned int i = 0; i < numtexgens; ++i)
      object.Write("\t%s.tex%d = %s.tex%d;\n", a, i, b, i);
    object.Write("\t%s.clipPos = %s.clipPos;\n", a, b);

    if (enable_pl)
      object.Write("\t%s.Normal = %s.Normal;\n", a, b);
  }
  else
  {
    // Store clip position in the w component of first 4 texcoords
    int num_texcoords = enable_pl ? 8 : numtexgens;
    for (int i = 0; i < num_texcoords; ++i)
      object.Write("\t%s.tex%d = %s.tex%d;\n", a, i, b, i);
  }
  if (g_ActiveConfig.backend_info.bSupportsDepthClamp)
  {
    object.Write("\t%s.clipDist.x = %s.clipDist.x;\n", a, b);
    object.Write("\t%s.clipDist.y = %s.clipDist.y;\n", a, b);
  }
}

// We use the flag "centroid" to fix some MSAA rendering bugs. With MSAA, the
// pixel shader will be executed for each pixel which has at least one passed sample.
// So there may be rendered pixels where the center of the pixel isn't in the primitive.
// As the pixel shader usually renders at the center of the pixel, this position may be
// outside the primitive. This will lead to sampling outside the texture, sign changes, ...
// As a workaround, we interpolate at the centroid of the coveraged pixel, which
// is always inside the primitive.
// Without MSAA, this flag is defined to have no effect.
inline const char* GetInterpolationQualifier(API_TYPE api_type, bool msaa, bool ssaa, bool in = true, bool in_out = false)
{
  if (!msaa || (api_type & API_D3D9))
    return "";

  if (!ssaa)
  {
    if (in_out && (api_type == API_OPENGL || api_type == API_VULKAN) && !g_ActiveConfig.backend_info.bSupportsBindingLayout)
      return in ? "centroid in" : "centroid out";
    return "centroid";
  }

  return "sample";
}
