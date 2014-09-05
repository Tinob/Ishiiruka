// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka By Tino

#pragma once

#include <vector>
#include <D3Dcompiler.h>
#include "VideoCommon/ShaderGenCommon.h"
class HLSLCompiler
{
public:
	static HLSLCompiler& getInstance();
	HRESULT CompileShader(LPCVOID pSrcData,
		SIZE_T SrcDataSize,
		LPCSTR pSourceName,
		const D3D_SHADER_MACRO *pDefines,
		ID3DInclude *pInclude,
		LPCSTR pEntrypoint,
		LPCSTR pTarget,
		UINT Flags1,
		UINT Flags2,
		ID3DBlob **ppCode,
		ID3DBlob **ppErrorMsgs
		);
	HRESULT Reflect(LPCVOID, SIZE_T, REFIID, void**);	
	virtual ~HLSLCompiler();
private:
	HLSLCompiler();
	typedef HRESULT(WINAPI *D3DREFLECT)(LPCVOID, SIZE_T, REFIID, void**);
	HINSTANCE hD3DCompilerDll;
	pD3DCompile PD3DCompile;
	D3DREFLECT PD3DReflect;
	s32 d3dcompiler_dll_ref;
	HRESULT LoadCompiler();
	void UnloadCompiler();
	HLSLCompiler(HLSLCompiler const&);
	void operator=(HLSLCompiler const&);
};
