// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"

class Vertexloader_Mtx
{
public:
  static void LOADERDECL PosMtx_ReadDirect_UByte();
  static void LOADERDECL PosMtx_Write();
  static void LOADERDECL TexMtx_ReadDirect_UByte();
  static void LOADERDECL TexMtx_Write_Float();
  static void LOADERDECL TexMtx_Write_Float2();
  static void LOADERDECL TexMtx_Write_Float3();
};