// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoadingSSE.h"

template <typename T>
__forceinline float PosScale(TPipelineState &pipelinestate, T val)
{
	return val * pipelinestate.posScale;
}

template <>
__forceinline float PosScale(TPipelineState &pipelinestate, float val)
{
	return val;
}

template <typename T>
__forceinline u8* IndexedDataPosition(TPipelineState &pipelinestate)
{
	auto const index = pipelinestate.Read<T>();
	return cached_arraybases[ARRAY_POSITION] + (index * arraystrides[ARRAY_POSITION]);
}

template <typename T, int N>
__forceinline void _Pos_ReadDirect(TPipelineState &pipelinestate)
{
	static_assert(N <= 3, "N > 3 is not sane!");

	for (int i = 0; i < 3; ++i)
		pipelinestate.Write(i<N ? PosScale(pipelinestate, pipelinestate.Read<T>()) : 0.f);
}

template <typename I, typename T, int N>
__forceinline void _Pos_ReadIndex(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	static_assert(N <= 3, "N > 3 is not sane!");

	auto const data = reinterpret_cast<const T*>(IndexedDataPosition<I>(pipelinestate));

	for (int i = 0; i < 3; ++i)
		pipelinestate.Write(i<N ? PosScale(pipelinestate, Common::FromBigEndian(data[i])) : 0.f);
}

#if _M_SSE >= 0x301
template <typename I, bool three>
__forceinline void _Pos_ReadIndex_Float_SSSE3(TPipelineState &pipelinestate)
{
	const __m128i* pData = (const __m128i*)IndexedDataPosition<I>(pipelinestate);
	if (three)
	{
		Float3ToFloat3sse3((__m128i*)pipelinestate.GetWritePosition(), pData);
	}
	else
	{
		Float2ToFloat3sse3((__m128i*)pipelinestate.GetWritePosition(), pData);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}

template <bool three>
__forceinline void _Pos_ReadDirect_Float_SSSE3(TPipelineState &pipelinestate)
{
	const __m128i* pData = (const __m128i*)pipelinestate.GetReadPosition();
	if (three)
	{
		pipelinestate.ReadSkip(sizeof(float) * 3);
		Float3ToFloat3sse3((__m128i*)pipelinestate.GetWritePosition(), pData);
	}
	else
	{
		pipelinestate.ReadSkip(sizeof(float) * 2);
		Float2ToFloat3sse3((__m128i*)pipelinestate.GetWritePosition(), pData);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}
#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
__forceinline void _Pos_ReadIndex_16x2_SSE4(TPipelineState &pipelinestate)
{
	const s32 Data = *((const s32*)IndexedDataPosition<I>(pipelinestate));
	if (Signed)
	{
		Short2ToFloat3sse4((float*)pipelinestate.GetWritePosition(), Data, &pipelinestate.posScale);
	}
	else
	{
		UShort2ToFloat3sse4((float*)pipelinestate.GetWritePosition(), Data, &pipelinestate.posScale);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}

template <typename I, bool Signed>
__forceinline void _Pos_ReadIndex_16x3_SSE4(TPipelineState &pipelinestate)
{
	const __m128i* pData = (const __m128i*)IndexedDataPosition<I>(pipelinestate);
	if (Signed)
	{
		Short3ToFloat3sse4((float*)pipelinestate.GetWritePosition(), pData, &pipelinestate.posScale);
	}
	else
	{
		UShort3ToFloat3sse4((float*)pipelinestate.GetWritePosition(), pData, &pipelinestate.posScale);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}

template <bool Signed>
__forceinline void _Pos_ReadDirect_16x2_SSE4(TPipelineState &pipelinestate)
{
	const s32 Data = *((const s32*)pipelinestate.GetReadPosition());
	pipelinestate.ReadSkip(sizeof(s16) * 2);
	if (Signed)
	{
		Short2ToFloat3sse4((float*)pipelinestate.GetWritePosition(), Data, &pipelinestate.posScale);
	}
	else
	{
		UShort2ToFloat3sse4((float*)pipelinestate.GetWritePosition(), Data, &pipelinestate.posScale);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}

template <bool Signed>
__forceinline void _Pos_ReadDirect_16x3_SSE4(TPipelineState &pipelinestate)
{
	const __m128i* pData = (const __m128i*)pipelinestate.GetReadPosition();
	pipelinestate.ReadSkip(sizeof(s16) * 3);
	if (Signed)
	{
		Short3ToFloat3sse4((float*)pipelinestate.GetWritePosition(), pData, &pipelinestate.posScale);
	}
	else
	{
		UShort3ToFloat3sse4((float*)pipelinestate.GetWritePosition(), pData, &pipelinestate.posScale);
	}
	pipelinestate.WriteSkip(sizeof(float) * 3);
}
#endif