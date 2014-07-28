// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"

void LOADERDECL Color_ReadDirect_24b_888();
void LOADERDECL Color_ReadDirect_32b_888x();
void LOADERDECL Color_ReadDirect_16b_565();
void LOADERDECL Color_ReadDirect_16b_4444();
void LOADERDECL Color_ReadDirect_24b_6666();
void LOADERDECL Color_ReadDirect_32b_8888();

void LOADERDECL Color_ReadIndex8_16b_565();
void LOADERDECL Color_ReadIndex8_24b_888();
void LOADERDECL Color_ReadIndex8_32b_888x();
void LOADERDECL Color_ReadIndex8_16b_4444();
void LOADERDECL Color_ReadIndex8_24b_6666();
void LOADERDECL Color_ReadIndex8_32b_8888();

void LOADERDECL Color_ReadIndex16_16b_565();
void LOADERDECL Color_ReadIndex16_24b_888();
void LOADERDECL Color_ReadIndex16_32b_888x();
void LOADERDECL Color_ReadIndex16_16b_4444();
void LOADERDECL Color_ReadIndex16_24b_6666();
void LOADERDECL Color_ReadIndex16_32b_8888();


void  Color_ReadDirect_24b_888STR(std::string *dest);
void  Color_ReadDirect_32b_888xSTR(std::string *dest);
void  Color_ReadDirect_16b_565STR(std::string *dest);
void  Color_ReadDirect_16b_4444STR(std::string *dest);
void  Color_ReadDirect_24b_6666STR(std::string *dest);
void  Color_ReadDirect_32b_8888STR(std::string *dest);

void  Color_ReadIndex8_16b_565STR(std::string *dest);
void  Color_ReadIndex8_24b_888STR(std::string *dest);
void  Color_ReadIndex8_32b_888xSTR(std::string *dest);
void  Color_ReadIndex8_16b_4444STR(std::string *dest);
void  Color_ReadIndex8_24b_6666STR(std::string *dest);
void  Color_ReadIndex8_32b_8888STR(std::string *dest);

void  Color_ReadIndex16_16b_565STR(std::string *dest);
void  Color_ReadIndex16_24b_888STR(std::string *dest);
void  Color_ReadIndex16_32b_888xSTR(std::string *dest);
void  Color_ReadIndex16_16b_4444STR(std::string *dest);
void  Color_ReadIndex16_24b_6666STR(std::string *dest);
void  Color_ReadIndex16_32b_8888STR(std::string *dest);