// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <d3dx9.h>
#include <string>

#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/HLSLCompiler.h"
#include "D3DShader.h"

namespace DX9
{

namespace D3D
{

// bytecode->shader.
LPDIRECT3DVERTEXSHADER9 CreateVertexShaderFromByteCode(const u8 *bytecode, u32 len)
{
	LPDIRECT3DVERTEXSHADER9 v_shader;
	HRESULT hr = D3D::dev->CreateVertexShader((DWORD *)bytecode, &v_shader);
	if (FAILED(hr))
		return NULL;

	return v_shader;
}

// code->bytecode.
bool CompileVertexShader(const char *code, u32 len, u8 **bytecode, u32 *bytecodelen)
{
	ID3DBlob* shaderBuffer = NULL;
	ID3DBlob* errorBuffer = NULL;

	HRESULT hr = HLSLCompiler::getInstance().CompileShader(
		code,
		len,
		nullptr,
		nullptr,
		nullptr,
		"main",
		D3D::VertexShaderVersionString(),
		D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&shaderBuffer,
		&errorBuffer);

	if (FAILED(hr))
	{
		static u32 num_failures = 0;
		char szTemp[MAX_PATH];
		sprintf(szTemp, "%sbad_vs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
		std::ofstream file;
		OpenFStream(file, szTemp, std::ios_base::out);
		file << code;
		file.close();

		PanicAlert("Failed to compile vertex shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
						szTemp,
						D3D::VertexShaderVersionString(),
						(char*)errorBuffer->GetBufferPointer());

		*bytecode = NULL;
		*bytecodelen = 0;
	}
	else
	{
		*bytecodelen = (u32)shaderBuffer->GetBufferSize();
		*bytecode = new u8[*bytecodelen];
		memcpy(*bytecode, shaderBuffer->GetBufferPointer(), *bytecodelen);
	}

	//cleanup
	if (shaderBuffer)
		shaderBuffer->Release();
	if (errorBuffer)
		errorBuffer->Release();
	return SUCCEEDED(hr) ? true : false;
}

// bytecode->shader
LPDIRECT3DPIXELSHADER9 CreatePixelShaderFromByteCode(const u8 *bytecode, u32 len)
{
	LPDIRECT3DPIXELSHADER9 p_shader;
	HRESULT hr = D3D::dev->CreatePixelShader((DWORD *)bytecode, &p_shader);
	if (FAILED(hr))
		return NULL;

	return p_shader;
}

// code->bytecode
bool CompilePixelShader(const char *code, u32 len, u8 **bytecode, u32 *bytecodelen)
{
	ID3DBlob* shaderBuffer = 0;
	ID3DBlob* errorBuffer = 0;

	// Someone:
	// For some reason, I had this kind of errors : "Shader uses texture addressing operations
	// in a dependency chain that is too complex for the target shader model (ps_2_0) to handle."
	HRESULT hr = HLSLCompiler::getInstance().CompileShader(
		code,
		len,
		nullptr,
		nullptr,
		nullptr,
		"main",
		D3D::PixelShaderVersionString(),
		D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3,
		0,
		&shaderBuffer,
		&errorBuffer);;

	if (FAILED(hr))
	{
		static u32 num_failures = 0;
		char szTemp[MAX_PATH];
		sprintf(szTemp, "%sbad_ps_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
		std::ofstream file;
		OpenFStream(file, szTemp, std::ios_base::out);
		file << code;
		file.close();

		PanicAlert("Failed to compile pixel shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
						szTemp,
						D3D::PixelShaderVersionString(),
						(char*)errorBuffer->GetBufferPointer());

		*bytecode = NULL;
		*bytecodelen = 0;
	}
	else
	{
		*bytecodelen = (u32)shaderBuffer->GetBufferSize();
		*bytecode = new u8[*bytecodelen];
		memcpy(*bytecode, shaderBuffer->GetBufferPointer(), *bytecodelen);
	}

	//cleanup
	if (shaderBuffer)
		shaderBuffer->Release();
	if (errorBuffer)
		errorBuffer->Release();
	return SUCCEEDED(hr) ? true : false;
}

LPDIRECT3DVERTEXSHADER9 CompileAndCreateVertexShader(const char *code, u32 len)
{
	u8 *bytecode;
	u32 bytecodelen;
	if (CompileVertexShader(code, len, &bytecode, &bytecodelen))
	{
		LPDIRECT3DVERTEXSHADER9 v_shader = CreateVertexShaderFromByteCode(bytecode, len);
		delete [] bytecode;
		return v_shader;
	}
	return NULL;
}

LPDIRECT3DPIXELSHADER9 CompileAndCreatePixelShader(const char* code, u32 len)
{
	u8 *bytecode;
	u32 bytecodelen;
	if (CompilePixelShader(code, len, &bytecode, &bytecodelen))
	{
		LPDIRECT3DPIXELSHADER9 p_shader = CreatePixelShaderFromByteCode(bytecode, len);
		delete [] bytecode;
		return p_shader;
	}
	return NULL;
}

}  // namespace

}  // namespace DX9