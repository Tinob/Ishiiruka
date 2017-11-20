// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include <functional>

#include "VideoCommon/TessellationShaderGen.h"
#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/UberShaderPixel.h"
#include "VideoCommon/UberShaderVertex.h"

namespace DX12
{

class D3DBlob;

class ShaderCache final
{
public:
  static void Init();
  static void Clear();
  static void Shutdown();

  static void PrepareShaders(
    PIXEL_SHADER_RENDER_MODE render_mode,
    PrimitiveType gs_primitive_type,
    u32 components,
    const XFMemory &xfr,
    const BPMemory &bpm);

  static bool TestShaders();
  static bool UsePixelUberShader();
  static bool UseVertexUberShader();

  static void InsertVSByteCode(const VertexShaderUid& uid, D3DBlob* bytecode_blob);
  static void InsertPSByteCode(const PixelShaderUid& uid, D3DBlob* bytecode_blob);
  static void InsertVUSByteCode(const UberShader::VertexUberShaderUid& uid, D3DBlob* bytecode_blob);
  static void InsertPUSByteCode(const UberShader::PixelUberShaderUid& uid, D3DBlob* bytecode_blob);
  static void InsertGSByteCode(const GeometryShaderUid& uid, D3DBlob * bytecode_blob);
  static void InsertDSByteCode(const TessellationShaderUid& uid, D3DBlob* bytecode_blob);
  static void InsertHSByteCode(const TessellationShaderUid& uid, D3DBlob* bytecode_blob);


  static D3D12_SHADER_BYTECODE GetActiveDomainShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActiveGeometryShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActiveHullShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActivePixelShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActiveVertexShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActivePixelUberShaderBytecode();
  static D3D12_SHADER_BYTECODE GetActiveVertexUberShaderBytecode();

  // The various uid flavors inherit from ShaderGeneratorInterface.
  static const GeometryShaderUid& GetActiveGeometryShaderUid();
  static const PixelShaderUid&    GetActivePixelShaderUid();
  static const VertexShaderUid&   GetActiveVertexShaderUid();
  static const TessellationShaderUid& GetActiveTessellationShaderUid();
  static const UberShader::PixelUberShaderUid& GetActivePixelUberShaderUid();
  static const UberShader::VertexUberShaderUid& GetActiveVertexUberShaderUid();

  static D3D12_SHADER_BYTECODE GetDomainShaderFromUid(const TessellationShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetGeometryShaderFromUid(const GeometryShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetHullShaderFromUid(const TessellationShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetPixelShaderFromUid(const PixelShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetVertexShaderFromUid(const VertexShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetPixelUberShaderFromUid(const UberShader::PixelUberShaderUid& uid);
  static D3D12_SHADER_BYTECODE GetVertexUberShaderFromUid(const UberShader::VertexUberShaderUid& uid);


  static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetCurrentPrimitiveTopology();
  static void Reload();
private:
  static void CompileHostBasedShaders();
  static void CompileShaders();
  static void CompileUberShaders();
  static void LoadFromDisk();
  static void LoadHostBasedFromDisk();
  static void SetCurrentPrimitiveTopology(PrimitiveType gs_primitive_type);

  static void HandleGSUIDChange(const GeometryShaderUid& gs_uid, std::function<void()> oncompilationfinished);

  static void HandlePSUIDChange(const PixelShaderUid& ps_uid, bool forcecompile, std::function<void()> oncompilationfinished);

  static void HandleVSUIDChange(const VertexShaderUid& vs_uid, bool forcecompile, std::function<void()> oncompilationfinished);

  static void HandleTSUIDChange(const TessellationShaderUid& ts_uid, std::function<void()> oncompilationfinished);

  static void HandlePUSUIDChange(const UberShader::PixelUberShaderUid& ps_uid, std::function<void()> oncompilationfinished);

  static void HandleVUSUIDChange(const UberShader::VertexUberShaderUid& vs_uid, std::function<void()> oncompilationfinished);
};

}
