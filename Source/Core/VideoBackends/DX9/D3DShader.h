// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "VideoBackends/DX9/D3DBase.h"

namespace DX9
{

namespace D3D
{
	LPDIRECT3DVERTEXSHADER9 CreateVertexShaderFromByteCode(const u8 *bytecode, u32 len);
	LPDIRECT3DPIXELSHADER9 CreatePixelShaderFromByteCode(const u8 *bytecode, u32 len);

	// The returned bytecode buffers should be delete[]-d.
	bool CompileVertexShader(const char *code, u32 len, u8 **bytecode, u32 *bytecodelen);
	bool CompilePixelShader(const char *code, u32 len, u8 **bytecode, u32 *bytecodelen);

	// Utility functions
	LPDIRECT3DVERTEXSHADER9 CompileAndCreateVertexShader(const char *code, u32 len);
	LPDIRECT3DPIXELSHADER9 CompileAndCreatePixelShader(const char *code, u32 len);
}

}  // namespace DX9