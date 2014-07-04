// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoadingSSE.h"

extern float posScale;
extern TVtxAttr *pVtxAttr;

template <typename T>
float PosScale(T val)
{
	return val * posScale;
}

template <>
float PosScale(float val)
{
	return val;
}

template <typename T>
__forceinline u8* IndexedDataPosition()
{
	auto const index = DataRead<T>();
	return cached_arraybases[ARRAY_POSITION] + (index * arraystrides[ARRAY_POSITION]);
}

template <typename T, int N>
__forceinline void _Pos_ReadDirect()
{
	static_assert(N <= 3, "N > 3 is not sane!");

	for (int i = 0; i < 3; ++i)
		DataWrite(i<N ? PosScale(DataRead<T>()) : 0.f);
}

template <typename I, typename T, int N>
__forceinline void _Pos_ReadIndex()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	static_assert(N <= 3, "N > 3 is not sane!");

	auto const data = reinterpret_cast<const T*>(IndexedDataPosition<I>());

	for (int i = 0; i < 3; ++i)
		DataWrite(i<N ? PosScale(Common::FromBigEndian(data[i])) : 0.f);
}

#if _M_SSE >= 0x301
template <typename I, bool three>
__forceinline void _Pos_ReadIndex_Float_SSSE3()
{
	const __m128i* pData = (const __m128i*)IndexedDataPosition<I>();
	if (three)
	{
		Float3ToFloat3sse3((__m128i*)VertexManager::s_pCurBufferPointer, pData);
	}
	else
	{
		Float2ToFloat3sse3((__m128i*)VertexManager::s_pCurBufferPointer, pData);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}

template <bool three>
__forceinline void _Pos_ReadDirect_Float_SSSE3()
{
	const __m128i* pData = (const __m128i*)DataGetPosition();
	if (three)
	{
		DataSkip(sizeof(float) * 3);
		Float3ToFloat3sse3((__m128i*)VertexManager::s_pCurBufferPointer, pData);
	}
	else
	{
		DataSkip(sizeof(float) * 2);
		Float2ToFloat3sse3((__m128i*)VertexManager::s_pCurBufferPointer, pData);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}
#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
__forceinline void _Pos_ReadIndex_16x2_SSE4()
{
	const s32 Data = *((const s32*)IndexedDataPosition<I>());
	if (Signed)
	{
		Short2ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, Data, &posScale);
	}
	else
	{
		UShort2ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, Data, &posScale);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}

template <typename I, bool Signed>
__forceinline void _Pos_ReadIndex_16x3_SSE4()
{
	const __m128i* pData = (const __m128i*)IndexedDataPosition<I>();
	if (Signed)
	{
		Short3ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, pData, &posScale);
	}
	else
	{
		UShort3ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, pData, &posScale);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}

template <bool Signed>
__forceinline void _Pos_ReadDirect_16x2_SSE4()
{
	const s32 Data = *((const s32*)DataGetPosition());
	DataSkip(sizeof(s16) * 2);
	if (Signed)
	{
		Short2ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, Data, &posScale);
	}
	else
	{
		UShort2ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, Data, &posScale);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}

template <bool Signed>
__forceinline void _Pos_ReadDirect_16x3_SSE4()
{
	const __m128i* pData = (const __m128i*)DataGetPosition();
	DataSkip(sizeof(s16) * 3);
	if (Signed)
	{
		Short3ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, pData, &posScale);
	}
	else
	{
		UShort3ToFloat3sse4((float*)VertexManager::s_pCurBufferPointer, pData, &posScale);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 3;
}
#endif