// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "VideoCommon/VertexLoader_Color.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"

void LOADERDECL Color_ReadDirect_24b_888(TPipelineState &pipelinestate) { _Color_ReadDirect_24b_888(pipelinestate); }
void LOADERDECL Color_ReadDirect_32b_888x(TPipelineState &pipelinestate) { _Color_ReadDirect_32b_888x(pipelinestate); }
void LOADERDECL Color_ReadDirect_16b_565(TPipelineState &pipelinestate) { _Color_ReadDirect_16b_565(pipelinestate); }
void LOADERDECL Color_ReadDirect_16b_4444(TPipelineState &pipelinestate) { _Color_ReadDirect_16b_4444(pipelinestate); }
void LOADERDECL Color_ReadDirect_24b_6666(TPipelineState &pipelinestate) { _Color_ReadDirect_24b_6666(pipelinestate); }
void LOADERDECL Color_ReadDirect_32b_8888(TPipelineState &pipelinestate) { _Color_ReadDirect_32b_8888(pipelinestate); }

void LOADERDECL Color_ReadIndex8_16b_565(TPipelineState &pipelinestate) { Color_ReadIndex_16b_565<u8>(pipelinestate); }
void LOADERDECL Color_ReadIndex8_24b_888(TPipelineState &pipelinestate) { Color_ReadIndex_24b_888<u8>(pipelinestate); }
void LOADERDECL Color_ReadIndex8_32b_888x(TPipelineState &pipelinestate) { Color_ReadIndex_32b_888x<u8>(pipelinestate); }
void LOADERDECL Color_ReadIndex8_16b_4444(TPipelineState &pipelinestate) { Color_ReadIndex_16b_4444<u8>(pipelinestate); }
void LOADERDECL Color_ReadIndex8_24b_6666(TPipelineState &pipelinestate) { Color_ReadIndex_24b_6666<u8>(pipelinestate); }
void LOADERDECL Color_ReadIndex8_32b_8888(TPipelineState &pipelinestate) { Color_ReadIndex_32b_8888<u8>(pipelinestate); }

void LOADERDECL Color_ReadIndex16_16b_565(TPipelineState &pipelinestate) { Color_ReadIndex_16b_565<u16>(pipelinestate); }
void LOADERDECL Color_ReadIndex16_24b_888(TPipelineState &pipelinestate) { Color_ReadIndex_24b_888<u16>(pipelinestate); }
void LOADERDECL Color_ReadIndex16_32b_888x(TPipelineState &pipelinestate) { Color_ReadIndex_32b_888x<u16>(pipelinestate); }
void LOADERDECL Color_ReadIndex16_16b_4444(TPipelineState &pipelinestate) { Color_ReadIndex_16b_4444<u16>(pipelinestate); }
void LOADERDECL Color_ReadIndex16_24b_6666(TPipelineState &pipelinestate) { Color_ReadIndex_24b_6666<u16>(pipelinestate); }
void LOADERDECL Color_ReadIndex16_32b_8888(TPipelineState &pipelinestate) { Color_ReadIndex_32b_8888<u16>(pipelinestate); }
