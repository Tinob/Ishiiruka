// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include <atomic>
#include <unordered_map>
#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DBlob.h"
#include "VideoBackends/DX11/VertexManager.h"

#include "VideoCommon/ObjectUsageProfiler.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/UberShaderVertex.h"

namespace DX11 {

class VertexShaderCache
{
public:
  static void Init();  
  static void Shutdown();
  static void PrepareShader(u32 components,
    const XFMemory &xfr,
    const BPMemory &bpm);
  static bool TestShader();
  static void SetActiveShader(D3DVertexFormat* current_vertex_format);
  static D3D::BufferDescriptor GetConstantBuffer();

  static ID3D11VertexShader* GetSimpleVertexShader();
  static ID3D11VertexShader* GetClearVertexShader();
  static ID3D11InputLayout* GetSimpleInputLayout();
  static ID3D11InputLayout* GetClearInputLayout();
  static void Reload();
  static void InsertByteCode(const VertexShaderUid &uid, D3DBlob&& bcodeblob);
  static void InsertByteCode(const UberShader::VertexUberShaderUid &uid, D3DBlob&& bcodeblob);
private:
  static void Clear();
  static void LoadFromDisk();
  static void CompileShaders();
  static void CompileUberShaders();
  static void CompileUberShader(const UberShader::VertexUberShaderUid& uid, const ShaderHostConfig& hostconfig, std::function<void()> oncompilationfinished);
  static void CompileVShader(const VertexShaderUid& uid, const ShaderHostConfig& hostconfig, bool forcecompile, std::function<void()> oncompilationfinished);
  struct VSCacheEntry
  {
    D3D::VertexShaderPtr shader;
    D3DBlob bytecode; // needed to initialize the input layout

    std::string code;
    bool compiled;
    std::atomic_flag initialized;
    VSCacheEntry() : compiled(false)
    {
      initialized.clear();
    }
    void SetByteCode(D3DBlob&& blob)
    {
      bytecode = std::move(blob);

    }
    void Destroy()
    {
      shader.reset();
      bytecode = nullptr;
    }
  };
  static inline void PushByteCode(D3DBlob&& bcodeblob, VSCacheEntry* entry);
  typedef ObjectUsageProfiler<VertexShaderUid, pKey_t, VSCacheEntry, VertexShaderUid::ShaderUidHasher> VSCache;
  typedef std::unordered_map<UberShader::VertexUberShaderUid, VSCacheEntry, UberShader::VertexUberShaderUid::ShaderUidHasher> VUSCache;
  static VSCache* s_vshaders;
  static VUSCache s_vuber_shaders;
  static const VSCacheEntry* s_last_entry;
  static const VSCacheEntry* s_last_uber_entry;
  static VertexShaderUid s_last_uid;
  static UberShader::VertexUberShaderUid s_last_uber_uid;
};

}  // namespace DX11
