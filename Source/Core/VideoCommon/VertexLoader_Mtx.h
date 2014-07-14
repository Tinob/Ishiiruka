// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#pragma once
#include "VideoCommon/VertexLoader.h"

class Vertexloader_Mtx
{
public:
	static void LOADERDECL PosMtx_ReadDirect_UByte();
	static void LOADERDECL PosMtx_Write();
	static void LOADERDECL PosMtxDisabled_Write();
	static void LOADERDECL TexMtx_ReadDirect_UByte();
	static void LOADERDECL TexMtx_Write_Float();
	static void LOADERDECL TexMtx_Write_Float2();
	static void LOADERDECL TexMtx_Write_Float4();
	static void PosMtx_ReadDirect_UByteSTR(std::string *dest);
	static void PosMtx_WriteSTR(std::string *dest);
	static void PosMtxDisabled_WriteSTR(std::string *dest);
	static void TexMtx_ReadDirect_UByteSTR(std::string *dest);
	static void TexMtx_Write_FloatSTR(std::string *dest);
	static void TexMtx_Write_Float2STR(std::string *dest);
	static void TexMtx_Write_Float4STR(std::string *dest);
};