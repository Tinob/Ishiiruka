// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/VertexLoadingSSE.h"

#define NUM_NRM_FORMAT 5
#define NUM_NRM_TYPE 4

enum ENormalElements
{
	NRM_NBT = 0,
	NRM_NBT3 = 1,
	NUM_NRM_ELEMENTS
};

enum ENormalIndices
{
	NRM_INDICES1 = 0,
	NRM_INDICES3 = 1,
	NUM_NRM_INDICES
};

__forceinline float nrmFracAdjust(s8 val)
{
	const float scale = (1.0f / (1U << 6));
	return val * scale;
}

__forceinline float nrmFracAdjust(u8 val)
{
	const float scale = (1.0f / (1U << 7));
	return val * scale;
}

__forceinline float nrmFracAdjust(s16 val)
{
	const float scale = (1.0f / (1U << 14));
	return val * scale;
}

__forceinline float nrmFracAdjust(u16 val)
{
	const float scale = (1.0f / (1U << 15));
	return val * scale;
}

__forceinline float nrmFracAdjust(float val)
{
	return val;
}

template <typename T, int N>
__forceinline void ReadIndirect(TPipelineState &pipelinestate, const T* data)
{
	static_assert(3 == N || 9 == N, "N is only sane as 3 or 9!");

	for (int i = 0; i != N; ++i)
	{
		pipelinestate.Write(nrmFracAdjust(Common::FromBigEndian(data[i])));
	}
}

template <typename T, int N>
__forceinline void _Normal_Direct(TPipelineState &pipelinestate)
{
	auto const source = reinterpret_cast<const T*>(pipelinestate.GetReadPosition());
	ReadIndirect<T, N * 3>(pipelinestate, source);
	pipelinestate.ReadSkip(N * 3 * sizeof(T));
}

template <typename I, typename T, int Offset>
__forceinline u8* IndexedNormalPosition(TPipelineState &pipelinestate)
{
	auto const index = pipelinestate.Read<I>();
	return cached_arraybases[ARRAY_NORMAL]
		+ (index * arraystrides[ARRAY_NORMAL]) + sizeof(T) * 3 * Offset;
}

template <typename I, typename T, int N, int Offset>
__forceinline void _Normal_Index_Offset(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	auto const data = reinterpret_cast<const T*>(IndexedNormalPosition<I, T, Offset>(pipelinestate));
	ReadIndirect<T, N * 3>(pipelinestate, data);
}

template <typename I, typename T>
__forceinline void _Normal_Index_Offset3(TPipelineState &pipelinestate)
{
	_Normal_Index_Offset<I, T, 1, 0>(pipelinestate);
	_Normal_Index_Offset<I, T, 1, 1>(pipelinestate);
	_Normal_Index_Offset<I, T, 1, 2>(pipelinestate);
}

#if _M_SSE >= 0x301

template <int N>
__forceinline void _Normal_Direct_FLOAT_SSSE3(TPipelineState &pipelinestate)
{
	const float* src = reinterpret_cast<const float*>(pipelinestate.GetReadPosition());
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
		src += 3;
		dst += 3;
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
	pipelinestate.ReadSkip(N * 3 * sizeof(float));
}

template <typename I, int N>
__forceinline void _Normal_Index_FLOAT_SSSE3(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const float* src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 0>(pipelinestate));
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
		src += 3;
		dst += 3;
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}

template <typename I>
__forceinline void _Normal_Index3_FLOAT_SSSE3(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const float* src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 0>(pipelinestate));
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 1>(pipelinestate));
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 2>(pipelinestate));
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}

#endif

#if _M_SSE >= 0x401

template <int N>
__forceinline void _Normal_Direct_S16_SSSE4(TPipelineState &pipelinestate)
{
	const s16* src = reinterpret_cast<const s16*>(pipelinestate.GetReadPosition());
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const float scale = (1.0f / (1U << 14));
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		src += 3;
		dst += 3;
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
	pipelinestate.ReadSkip(N * 3 * sizeof(s16));
}

template <int N>
__forceinline void _Normal_Direct_U16_SSSE4(TPipelineState &pipelinestate)
{
	const u16* src = reinterpret_cast<const u16*>(pipelinestate.GetReadPosition());
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const float scale = (1.0f / (1U << 15));
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		src += 3;
		dst += 3;
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
	pipelinestate.ReadSkip(N * 3 * sizeof(u16));
}

template <typename I, int N>
__forceinline void _Normal_Index_S16_SSE4(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const s16* src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>(pipelinestate));
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const float scale = (1.0f / (1U << 14));
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		src += 3;
		dst += 3;
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}

template <typename I, int N>
__forceinline void _Normal_Index_U16_SSE4(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const u16* src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>(pipelinestate));
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const float scale = (1.0f / (1U << 15));
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	if (N > 1)
	{
		src += 3;
		dst += 3;
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		src += 3;
		dst += 3;
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	}
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}

template <typename I>
__forceinline void _Normal_Index3_S16_SSE4(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const s16* src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>(pipelinestate));
	const float scale = (1.0f / (1U << 14));
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>(pipelinestate));
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>(pipelinestate));
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}

template <typename I>
__forceinline void _Normal_Index3_U16_SSE4(TPipelineState &pipelinestate)
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(pipelinestate.GetWritePosition());
	const u16* src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>(pipelinestate));
	const float scale = (1.0f / (1U << 15));
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>(pipelinestate));
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>(pipelinestate));
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	pipelinestate.SetWritePosition(reinterpret_cast<u8*>(dst));
}
#endif