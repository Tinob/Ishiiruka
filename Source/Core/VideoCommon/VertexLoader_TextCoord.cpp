// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "VideoCommon/VertexLoader_TextCoord.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"

bool VertexLoader_TextCoord::Initialized = false;

void LOADERDECL TexCoord_Read_Dummy()
{
	tcIndex++;
}

template <typename T, s32 N>
void LOADERDECL TexCoord_ReadDirect()
{
	_TexCoord_ReadDirect<T, N>();
}

template <typename I, typename T, s32 N>
void LOADERDECL TexCoord_ReadIndex()
{
	_TexCoord_ReadIndex<I, T, N>();
}

#if _M_SSE >= 0x301
template <typename I>
void LOADERDECL TexCoord_ReadIndex_Float2_SSSE3()
{
	_TexCoord_ReadIndex_Float2_SSSE3<I>();
}

void LOADERDECL TexCoord_ReadDirect_Float2_SSSE3()
{
	_TexCoord_ReadDirect_Float2_SSSE3();
}

#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
void LOADERDECL TexCoord_ReadIndex_16x2_SSE4()
{
	_TexCoord_ReadIndex_16x2_SSE4<I, Signed>();
}

template <bool Signed>
void LOADERDECL TexCoord_ReadDirect_16x2_SSE4()
{
	_TexCoord_ReadDirect_16x2_SSE4<Signed>();
}
#endif

static TPipelineFunction tableReadTexCoord[4][8][2] = {
	{
		{NULL, NULL,},
		{NULL, NULL,},
		{NULL, NULL,},
		{NULL, NULL,},
		{NULL, NULL,},
	},
	{
		{TexCoord_ReadDirect<u8, 1>,  TexCoord_ReadDirect<u8, 2>,},
		{TexCoord_ReadDirect<s8, 1>,   TexCoord_ReadDirect<s8, 2>,},
		{TexCoord_ReadDirect<u16, 1>, TexCoord_ReadDirect<u16, 2>,},
		{TexCoord_ReadDirect<s16, 1>,  TexCoord_ReadDirect<s16, 2>,},
		{TexCoord_ReadDirect<float, 1>,  TexCoord_ReadDirect<float, 2>,},
	},
	{
		{TexCoord_ReadIndex<u8, u8, 1>,  TexCoord_ReadIndex<u8, u8, 2>,},
		{TexCoord_ReadIndex<u8, s8, 1>,   TexCoord_ReadIndex<u8, s8, 2>,},
		{TexCoord_ReadIndex<u8, u16, 1>, TexCoord_ReadIndex<u8, u16, 2>,},
		{TexCoord_ReadIndex<u8, s16, 1>,  TexCoord_ReadIndex<u8, s16, 2>,},
		{TexCoord_ReadIndex<u8, float, 1>,  TexCoord_ReadIndex<u8, float, 2>,},
	},
	{
		{TexCoord_ReadIndex<u16, u8, 1>,  TexCoord_ReadIndex<u16, u8, 2>,},
		{TexCoord_ReadIndex<u16, s8, 1>,   TexCoord_ReadIndex<u16, s8, 2>,},
		{TexCoord_ReadIndex<u16, u16, 1>, TexCoord_ReadIndex<u16, u16, 2>,},
		{TexCoord_ReadIndex<u16, s16, 1>,  TexCoord_ReadIndex<u16, s16, 2>,},
		{TexCoord_ReadIndex<u16, float, 1>,  TexCoord_ReadIndex<u16, float, 2>,},
	},
};

static s32 tableReadTexCoordVertexSize[4][8][2] = {
	{
		{0, 0,}, {0, 0,}, {0, 0,}, {0, 0,}, {0, 0,},
	},
	{
		{1, 2,}, {1, 2,}, {2, 4,}, {2, 4,}, {4, 8,},
	},
	{
		{1, 1,}, {1, 1,}, {1, 1,}, {1, 1,}, {1, 1,},
	},
	{
		{2, 2,}, {2, 2,}, {2, 2,}, {2, 2,}, {2, 2,},
	},
};

void VertexLoader_TextCoord::Init(void)
{
	if (Initialized)
	{
		return;
	}
	Initialized = true;
#if _M_SSE >= 0x301

	if (cpu_info.bSSSE3)
	{
		tableReadTexCoord[1][4][1] = TexCoord_ReadDirect_Float2_SSSE3;
		tableReadTexCoord[2][4][1] = TexCoord_ReadIndex_Float2_SSSE3<u8>;
		tableReadTexCoord[3][4][1] = TexCoord_ReadIndex_Float2_SSSE3<u16>;
	}

#endif

#if _M_SSE >= 0x401

	if (cpu_info.bSSE4_1)
	{
		tableReadTexCoord[1][2][1] = TexCoord_ReadDirect_16x2_SSE4<false>;
		tableReadTexCoord[1][3][1] = TexCoord_ReadDirect_16x2_SSE4<true>;
		tableReadTexCoord[2][2][1] = TexCoord_ReadIndex_16x2_SSE4<u8, false>;
		tableReadTexCoord[3][2][1] = TexCoord_ReadIndex_16x2_SSE4<u16, false>;
		tableReadTexCoord[2][3][1] = TexCoord_ReadIndex_16x2_SSE4<u8, true>;
		tableReadTexCoord[3][3][1] = TexCoord_ReadIndex_16x2_SSE4<u16, true>;
	}

#endif

}

u32 VertexLoader_TextCoord::GetSize(u32 _type, u32 _format, u32 _elements)
{
	return tableReadTexCoordVertexSize[_type][_format][_elements];
}

TPipelineFunction VertexLoader_TextCoord::GetFunction(u32 _type, u32 _format, u32 _elements)
{
	return tableReadTexCoord[_type][_format][_elements];
}

TPipelineFunction VertexLoader_TextCoord::GetDummyFunction()
{
	return TexCoord_Read_Dummy;
}
