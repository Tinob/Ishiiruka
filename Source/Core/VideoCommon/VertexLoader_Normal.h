// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka By Tino
#pragma once
#include "Common/Common.h"
#include "VideoCommon/VertexLoader.h"
class VertexLoader_Normal
{
public:

	// Init
	static void Init(void);

	// GetSize
	static u32 GetSize(u32 _type, u32 _format, u32 _elements, u32 _index3);

	// GetFunction
	static TPipelineFunction GetFunction(u32 _type, u32 _format, u32 _elements, u32 _index3);

	// Append FunctionString
	static void GetFunctionSTR(std::string *dest, u32 _type, u32 _format, u32 _elements, u32 _index3);

private:
	static bool Initialized;
};