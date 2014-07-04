// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
#pragma once
#include "Common/Common.h"
#include "VideoCommon/VertexLoader.h"

class VertexLoader_Position {
public:

	// Init
	static void Init(void);

	// GetSize
	static unsigned int GetSize(u32 _type, u32 _format, u32 _elements);

	// GetFunction
	static TPipelineFunction GetFunction(u32 _type, u32 _format, u32 _elements);
private:
	static bool Initialized;
};