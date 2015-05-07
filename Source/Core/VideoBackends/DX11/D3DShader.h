// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <string>

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DBlob.h"

struct ID3D11PixelShader;
struct ID3D11VertexShader;

namespace DX11
{

namespace D3D
{
enum class ShaderType : u32 {
	Vertex,
	Pixel,
	Geometry,
	Compute
};

VertexShaderPtr CreateVertexShaderFromByteCode(const void* bytecode, size_t len);
GeometryShaderPtr CreateGeometryShaderFromByteCode(const void* bytecode, size_t len);
PixelShaderPtr CreatePixelShaderFromByteCode(const void* bytecode, size_t len);
ComputeShaderPtr CreateComputeShaderFromByteCode(const void* bytecode, size_t len);

bool CompileShader(
	ShaderType type, 
	const std::string& code,
	D3DBlob& blob, 
	const D3D_SHADER_MACRO* pDefines = nullptr,
	const char* pEntry = nullptr);

// Utility functions
VertexShaderPtr CompileAndCreateVertexShader(
	const std::string& code,
	const D3D_SHADER_MACRO* pDefines = nullptr,
	const char* pEntry = nullptr);
GeometryShaderPtr CompileAndCreateGeometryShader(
	const std::string& code,
	const D3D_SHADER_MACRO* pDefines = nullptr,
	const char* pEntry = nullptr);
PixelShaderPtr CompileAndCreatePixelShader(
	const std::string& code,
	const D3D_SHADER_MACRO* pDefines = nullptr,
	const char* pEntry = nullptr);
ComputeShaderPtr CompileAndCreateComputeShader(
	const std::string& code,
	const D3D_SHADER_MACRO* pDefines = nullptr,
	const char* pEntry = nullptr);

inline VertexShaderPtr CreateVertexShaderFromByteCode(D3DBlob& bytecode)
{
	return CreateVertexShaderFromByteCode(bytecode.Data(), bytecode.Size());
}
inline GeometryShaderPtr CreateGeometryShaderFromByteCode(D3DBlob& bytecode)
{
	return CreateGeometryShaderFromByteCode(bytecode.Data(), bytecode.Size());
}
inline PixelShaderPtr CreatePixelShaderFromByteCode(D3DBlob& bytecode)
{
	return CreatePixelShaderFromByteCode(bytecode.Data(), bytecode.Size());
}
inline ComputeShaderPtr CreateComputeShaderFromByteCode(D3DBlob& bytecode)
{
	return CreateComputeShaderFromByteCode(bytecode.Data(), bytecode.Size());
}
}

}  // namespace DX11