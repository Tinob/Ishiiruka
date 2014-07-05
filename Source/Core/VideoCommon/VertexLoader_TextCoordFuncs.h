// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoadingSSE.h"

extern s32 tcIndex;
extern float tcScale[8];

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

template <typename T, s32 N>
__forceinline void _TexCoord_ReadDirect()
{
	for (s32 i = 0; i != N; ++i)
		DataWrite(TCScale(DataRead<T>()));
	++tcIndex;
}

template <typename T>
__forceinline u8* IndexedTCoordPosition()
{
	auto const index = DataRead<T>();
	return cached_arraybases[ARRAY_TEXCOORD0 + tcIndex] + (index * arraystrides[ARRAY_TEXCOORD0 + tcIndex]);
}

template <typename I, typename T, s32 N>
__forceinline void _TexCoord_ReadIndex()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	auto const data = reinterpret_cast<const T*>(IndexedTCoordPosition<I>());

	for (s32 i = 0; i != N; ++i)
		DataWrite(TCScale(Common::FromBigEndian(data[i])));

	++tcIndex;
}

#if _M_SSE >= 0x301
template <typename I>
__forceinline void _TexCoord_ReadIndex_Float2_SSSE3()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const __m128i *pData = (const __m128i *)IndexedTCoordPosition<I>();
	Float2ToFloat2sse3((__m128i *)VertexManager::s_pCurBufferPointer, pData);
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	++tcIndex;
}

__forceinline void _TexCoord_ReadDirect_Float2_SSSE3()
{
	const __m128i *pData = (const __m128i *)DataGetPosition();
	Float2ToFloat2sse3((__m128i *)VertexManager::s_pCurBufferPointer, pData);
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	DataSkip(sizeof(float) * 2);
	++tcIndex;
}

#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
__forceinline void _TexCoord_ReadIndex_16x2_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	// Heavy in ZWW
	const s32 Data = *((const s32*)IndexedTCoordPosition<I>());
	if (Signed)
	{
		Short2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	else
	{
		UShort2ToFloat2sse4((__m64*)VertexManager::s_pCurBufferPointer, Data, &tcScale[tcIndex]);
	}
	VertexManager::s_pCurBufferPointer += sizeof(float) * 2;
	++tcIndex;
}

template <bool Signed>
__forceinline void _TexCoord_ReadDirect_16x2_SSE4()
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
	++tcIndex;
}
#endif