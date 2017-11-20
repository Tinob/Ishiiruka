// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <functional>

#include <d3d11.h>
#include <unordered_map>

#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/ObjectUsageProfiler.h"

namespace DX11
{

class GeometryShaderCache
{
public:
  static void Init();
  static void Clear();
  static void Shutdown();
  static void PrepareShader(
    PrimitiveType primitive_type,
    const XFMemory &xfr,
    const u32 components);
  static bool TestShader();
  static void InsertByteCode(const GeometryShaderUid &uid, const void* bytecode, u32 bytecodelen);

  static ID3D11GeometryShader* GetClearGeometryShader();
  static ID3D11GeometryShader* GetCopyGeometryShader();

  static ID3D11GeometryShader* GetActiveShader()
  {
    return s_last_entry->shader.get();
  }
  static D3D::BufferDescriptor GetConstantBuffer();
  static void Reload();
private:
  static void LoadFromDisk();
  static void CompileShaders();
  struct GSCacheEntry
  {
    D3D::GeometryShaderPtr shader;
    bool compiled;
    std::atomic_flag initialized;

    GSCacheEntry() : shader(nullptr), compiled(false)
    {
      initialized.clear();
    }
    ~GSCacheEntry()
    {
      shader.reset();
    }
  };
  static inline void PushByteCode(const void* bytecode, unsigned int bytecodelen, GSCacheEntry* entry);
  typedef std::unordered_map<GeometryShaderUid, GSCacheEntry, GeometryShaderUid::ShaderUidHasher> GSCache;

  static GSCache s_geometry_shaders;
  static const GSCacheEntry* s_last_entry;
  static GeometryShaderUid s_last_uid;
  static GSCacheEntry s_pass_entry;
  static void CompileGShader(const GeometryShaderUid& uid, std::function<void()> oncompilationfinished);
};

}  // namespace DX11
