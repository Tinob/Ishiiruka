// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include <atomic>
#include <functional>
#include <unordered_map>

#include <d3d11_2.h>

#include "VideoBackends/DX11/D3DPtr.h"

#include "VideoCommon/ObjectUsageProfiler.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/UberShaderPixel.h"


namespace DX11
{

class PixelShaderCache
{
public:
  static void Init();
  static void Clear();
  static void Shutdown();
  static void PrepareShader(
    PIXEL_SHADER_RENDER_MODE render_mode,
    u32 componets,
    const XFMemory &xfr,
    const BPMemory &bpm);
  static bool TestShader();  

  static void SetActiveShader();

  static D3D::BufferDescriptor GetConstantBuffer();

  static ID3D11PixelShader* GetColorMatrixProgram(bool multisampled);
  static ID3D11PixelShader* GetColorCopyProgram(bool multisampled, bool ssaa = false);
  static ID3D11PixelShader* GetDepthMatrixProgram(bool multisampled);
  static ID3D11PixelShader* GetClearProgram();
  static ID3D11PixelShader* ReinterpRGBA6ToRGB8(bool multisampled);
  static ID3D11PixelShader* ReinterpRGB8ToRGBA6(bool multisampled);
  static ID3D11PixelShader* GetDepthResolveProgram();
  static void InvalidateMSAAShaders();
  static void Reload();
  static void InsertByteCode(const PixelShaderUid &uid, const void* bytecode, u32 bytecodelen);
  static void InsertByteCode(const UberShader::PixelUberShaderUid &uid, const void* bytecode, u32 bytecodelen);
private:
  static void LoadFromDisk();
  static void CompileShaders();
  static void CompileUberShaders();
  static void CompileUberShader(const UberShader::PixelUberShaderUid& uid, const ShaderHostConfig& hostconfig, std::function<void()> oncompilationfinished);
  static void CompilePShader(const PixelShaderUid& uid, const ShaderHostConfig& hostconfig, bool forcecompile, std::function<void()> oncompilationfinished);
  struct PSCacheEntry
  {
    std::string code;
    D3D::PixelShaderPtr shader;
    bool compiled;
    std::atomic_flag initialized;

    PSCacheEntry() : compiled(false)
    {
      initialized.clear();
    }
    void Destroy()
    {
      shader.reset();
    }
  };
  static inline void PushByteCode(const void* bytecode, u32 bytecodelen, PSCacheEntry* entry);
  typedef ObjectUsageProfiler<PixelShaderUid, pKey_t, PSCacheEntry, PixelShaderUid::ShaderUidHasher> PSCache;
  typedef std::unordered_map<UberShader::PixelUberShaderUid, PSCacheEntry, UberShader::PixelUberShaderUid::ShaderUidHasher> PUSCache;
  static PSCache* s_pixel_shaders;
  static PUSCache s_pixel_uber_shaders;
  static const PSCacheEntry* s_last_entry;
  static const PSCacheEntry* s_last_uber_entry;
  static PixelShaderUid s_last_uid;
  static UberShader::PixelUberShaderUid s_last_uber_uid;
};

}  // namespace DX11
