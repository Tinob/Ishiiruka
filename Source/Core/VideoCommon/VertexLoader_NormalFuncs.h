#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoadingSSE.h"

__forceinline float nrmFracAdjust(s8 val)
{
	return val * fractionTable[6];
}

__forceinline float nrmFracAdjust(u8 val)
{
	return val * fractionTable[7];
}

__forceinline float nrmFracAdjust(s16 val)
{
	return val * fractionTable[14];
}

__forceinline float nrmFracAdjust(u16 val)
{
	return val * fractionTable[15];
}

__forceinline float nrmFracAdjust(float val)
{
	return val;
}

template <typename T, int N>
__forceinline void ReadIndirect(const T* data)
{
	static_assert(3 == N || 9 == N, "N is only sane as 3 or 9!");

	for (int i = 0; i != N; ++i)
	{
		DataWrite(nrmFracAdjust(Common::FromBigEndian(data[i])));
	}
}

template <typename T, int N>
__forceinline void _Normal_Direct()
{
	auto const source = reinterpret_cast<const T*>(DataGetPosition());
	ReadIndirect<T, N * 3>(source);
	DataSkip<N * 3 * sizeof(T)>();
}

template <typename I, typename T, int Offset>
__forceinline u8* IndexedNormalPosition()
{
	auto const index = DataRead<I>();
	return cached_arraybases[ARRAY_NORMAL]
		+ (index * arraystrides[ARRAY_NORMAL]) + sizeof(T) * 3 * Offset;
}

template <typename I, typename T, int N, int Offset>
__forceinline void _Normal_Index_Offset()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	auto const data = reinterpret_cast<const T*>(IndexedNormalPosition<I, T, Offset>());
	ReadIndirect<T, N * 3>(data);
}

#if _M_SSE >= 0x301

template <int N>
__forceinline void _Normal_Direct_FLOAT_SSSE3()
{
	const float* src = reinterpret_cast<const float*>(DataGetPosition());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
	DataSkip<N * 3 * sizeof(float)>();
}

template <typename I, int N>
__forceinline void _Normal_Index_FLOAT_SSSE3()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const float* src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 0>());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}

template <typename I>
__forceinline void _Normal_Index3_FLOAT_SSSE3()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	const float* src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 0>());
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 1>());
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	src = reinterpret_cast<const float*>(IndexedNormalPosition<I, float, 2>());
	Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
	dst += 3;
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}

#endif

#if _M_SSE >= 0x401

template <int N>
__forceinline void _Normal_Direct_S16_SSSE4()
{
	const s16* src = reinterpret_cast<const s16*>(DataGetPosition());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	float scale = fractionTable[14];
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
	DataSkip<N * 3 * sizeof(s16)>();
}

template <int N>
__forceinline void _Normal_Direct_U16_SSSE4()
{
	const u16* src = reinterpret_cast<const u16*>(DataGetPosition());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	float scale = fractionTable[15];
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
	DataSkip<N * 3 * sizeof(u16)>();
}

template <typename I, int N>
__forceinline void _Normal_Index_S16_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const s16* src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	float scale = fractionTable[14];
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}

template <typename I, int N>
__forceinline void _Normal_Index_U16_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

	const u16* src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>());
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	float scale = fractionTable[15];
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
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}

template <typename I>
__forceinline void _Normal_Index3_S16_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	const s16* src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>());
	float scale = fractionTable[14];
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>());
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const s16*>(IndexedNormalPosition<I, s16, 0>());
	Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}

template <typename I>
__forceinline void _Normal_Index3_U16_SSE4()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
	const u16* src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>());
	float scale = fractionTable[15];
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>());
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	src = reinterpret_cast<const u16*>(IndexedNormalPosition<I, u16, 0>());
	UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
	dst += 3;
	VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
}
#endif