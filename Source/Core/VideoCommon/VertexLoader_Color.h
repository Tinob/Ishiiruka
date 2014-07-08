// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
#include "VideoCommon/VertexLoader.h"
#pragma once

void LOADERDECL Color_ReadDirect_24b_888(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadDirect_32b_888x(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadDirect_16b_565(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadDirect_16b_4444(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadDirect_24b_6666(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadDirect_32b_8888(TPipelineState &pipelinestate);

void LOADERDECL Color_ReadIndex8_16b_565(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex8_24b_888(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex8_32b_888x(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex8_16b_4444(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex8_24b_6666(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex8_32b_8888(TPipelineState &pipelinestate);

void LOADERDECL Color_ReadIndex16_16b_565(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex16_24b_888(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex16_32b_888x(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex16_16b_4444(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex16_24b_6666(TPipelineState &pipelinestate);
void LOADERDECL Color_ReadIndex16_32b_8888(TPipelineState &pipelinestate);