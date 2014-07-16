// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#pragma once
#include "VideoCommon/VertexLoader.h"

class VertexLoader_BBox
{
public:
	static void LOADERDECL UpdateBoundingBoxPrepare();
	static void LOADERDECL UpdateBoundingBox();
	static void UpdateBoundingBoxPrepare(TPipelineState &pipelinestate);
	static void UpdateBoundingBox(TPipelineState &pipelinestate);
	static void SetPrimitive(s32 primitive);
};