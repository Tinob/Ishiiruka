// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoader_TextCoord.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VertexLoadingSSE.h"

template <int N>
void LOG_TEX();

template <>
__forceinline void LOG_TEX<1>()
{
	// warning: mapping buffer should be disabled to use this
	// PRIM_LOG("tex: %f, ", ((float*)VertexManager::s_pCurBufferPointer)[-1]);
}

template <>
__forceinline void LOG_TEX<2>()
{
	// warning: mapping buffer should be disabled to use this
	// PRIM_LOG("tex: %f %f, ", ((float*)VertexManager::s_pCurBufferPointer)[-2], ((float*)VertexManager::s_pCurBufferPointer)[-1]);
}

extern int tcIndex;
extern float tcScale[8];

void LOADERDECL TexCoord_Read_Dummy()
{
	tcIndex++;
}

template <typename T>
float TCScale(T val)
{
	return val * tcScale[tcIndex];
}

template <>
float TCScale(float val)
{
	return val;
}

template <typename T, int N>
void LOADERDECL TexCoord_ReadDirect()
{
	for (int i = 0; i != N; ++i)
		DataWrite(TCScale(DataRead<T>()));

	LOG_TEX<N>();

	++tcIndex;
}

template <typename T>
__forceinline u8* IndexedDataPosition()
{
	auto const index = DataRead<T>();
	return cached_arraybases[ARRAY_TEXCOORD0 + tcIndex] + (index * arraystrides[ARRAY_TEXCOORD0 + tcIndex]);
}

template <typename I, typename T, int N>
void LOADERDECL TexCoord_ReadIndex()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	
	auto const data = reinterpret_cast<const T*>(IndexedDataPosition<I>());

	for (int i = 0; i != N; ++i)
		DataWrite(TCScale(Common::FromBigEndian(data[i])));

	LOG_TEX<N>();
	++tcIndex;
}

#if _M_SSE >= 0x301
template <typename I>
void LOADERDECL TexCoord_ReadIndex_Float2_SSSE3()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const __m128i *pData = (const __m128i *)IndexedDataPosition<I>();
	Float2ToFloat2sse3((__m128i *)VertexManager::s_pCurBufferPointer, pData);
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	LOG_TEX<2>();
	++tcIndex;
}

void LOADERDECL TexCoord_ReadDirect_Float2_SSSE3()
{
	const __m128i *pData = (const __m128i *)DataGetPosition();
	Float2ToFloat2sse3((__m128i *)VertexManager::s_pCurBufferPointer, pData);
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	DataSkip(sizeof(float) * 2);
	LOG_TEX<2>();
	++tcIndex;
}

#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
void LOADERDECL TexCoord_ReadIndex_16x2_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	// Heavy in ZWW
	const s32 Data = *((const s32*)IndexedDataPosition<I>());
	if (Signed)
	{
		Short2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	else
	{
		UShort2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	LOG_TEX<2>();
	++tcIndex;
}

template <bool Signed>
void LOADERDECL TexCoord_ReadDirect_16x2_SSE4()
{
	const s32 Data = *((const s32*)DataGetPosition());
	if (Signed)
	{
		Short2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	else
	{
		UShort2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	DataSkip(sizeof(s32));
	LOG_TEX<2>();
	++tcIndex;
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

static int tableReadTexCoordVertexSize[4][8][2] = {
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

unsigned int VertexLoader_TextCoord::GetSize(unsigned int _type, unsigned int _format, unsigned int _elements)
{
	return tableReadTexCoordVertexSize[_type][_format][_elements];
}

TPipelineFunction VertexLoader_TextCoord::GetFunction(unsigned int _type, unsigned int _format, unsigned int _elements)
{
	return tableReadTexCoord[_type][_format][_elements];
}

TPipelineFunction VertexLoader_TextCoord::GetDummyFunction()
{
	return TexCoord_Read_Dummy;
}
