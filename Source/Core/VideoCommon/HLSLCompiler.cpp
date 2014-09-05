// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka By Tino

#include "VideoCommon/HLSLCompiler.h"

HLSLCompiler& HLSLCompiler::getInstance()
{
	static HLSLCompiler instance;
	// Instantiated on first use.
	return instance;
}

HLSLCompiler::HLSLCompiler() :
hD3DCompilerDll(nullptr), 
PD3DCompile(nullptr), 
PD3DReflect(nullptr),
d3dcompiler_dll_ref(0)
{
	LoadCompiler();	
}

HLSLCompiler::~HLSLCompiler()
{
	UnloadCompiler();	
}

HRESULT HLSLCompiler::CompileShader(LPCVOID pSrcData,
	SIZE_T SrcDataSize,
	LPCSTR pSourceName,
	const D3D_SHADER_MACRO *pDefines,
	ID3DInclude *pInclude,
	LPCSTR pEntrypoint,
	LPCSTR pTarget,
	UINT Flags1,
	UINT Flags2,
	ID3DBlob **ppCode,
	ID3DBlob **ppErrorMsgs)
{
	return PD3DCompile(pSrcData,
		SrcDataSize,
		pSourceName,
		pDefines,
		pInclude,
		pEntrypoint,
		pTarget,
		Flags1, Flags2,
		ppCode, 
		ppErrorMsgs);
}

HRESULT HLSLCompiler::Reflect(
	LPCVOID bytecode, 
	SIZE_T bytecodesize, 
	REFIID interfaceid, 
	void** interfaceobject)
{
	return PD3DReflect(bytecode, bytecodesize, interfaceid, interfaceobject);
}

HRESULT HLSLCompiler::LoadCompiler()
{
	if (d3dcompiler_dll_ref++ > 0) return S_OK;
	if (hD3DCompilerDll) return S_OK;

	// try to load D3DCompiler
	HRESULT hr = E_FAIL;
	for (unsigned int num = 49; num >= 42; --num)
	{
		std::string compilerfile = StringFromFormat("D3DCompiler_%d.dll", num);
		hD3DCompilerDll = LoadLibraryA(compilerfile.c_str());
		if (hD3DCompilerDll != nullptr)
		{
			NOTICE_LOG(VIDEO, 
				"Successfully loaded %s.", 
				compilerfile.c_str());
			hr = S_OK;
			break;
		}
	}
	if (FAILED(hr))
	{
		MessageBoxA(
			nullptr, 
			"Failed to load any D3DCompiler dll", 
			"Critical error", 
			MB_OK | MB_ICONERROR);
		return hr;
	}
	PD3DCompile = (pD3DCompile)GetProcAddress(hD3DCompilerDll, "D3DCompile");
	if (PD3DCompile == nullptr) 
		MessageBoxA(nullptr, 
		"GetProcAddress failed for D3DCompile!", 
		"Critical error",
		MB_OK | MB_ICONERROR);
	PD3DReflect = (D3DREFLECT)GetProcAddress(hD3DCompilerDll, "D3DReflect");
	if (PD3DReflect == NULL) 
		MessageBoxA(NULL, 
		"GetProcAddress failed for D3DReflect!", 
		"Critical error", MB_OK | MB_ICONERROR);
	return S_OK;
}

void HLSLCompiler::UnloadCompiler()
{
	if (!d3dcompiler_dll_ref) return;
	if (--d3dcompiler_dll_ref != 0) return;

	if (hD3DCompilerDll) FreeLibrary(hD3DCompilerDll);
	hD3DCompilerDll = nullptr;
	PD3DCompile = nullptr;
}
