#include "Common/Common.h"
#if _M_SSE >= 0x401
#include <smmintrin.h>
#include <emmintrin.h>
#elif _M_SSE >= 0x301 && !(defined __GNUC__ && !defined __SSSE3__)
#include <tmmintrin.h>
#endif

#if _M_SSE >= 0x301
static const __m128i kMaskSwap32_2 = _mm_set_epi32(0xFFFFFFFFL, 0xFFFFFFFFL, 0x04050607L, 0x00010203L);
static const __m128i kMaskSwap32_3 = _mm_set_epi32(0xFFFFFFFFL, 0x08090A0BL, 0x04050607L, 0x00010203L);
static const __m128i kMaskSwap32_4 = _mm_set_epi32(0x0C0D0E0FL, 0x08090A0BL, 0x04050607L, 0x00010203L);

__forceinline void Float2ToFloat2sse3(__m128i *dst, const __m128i *src)
{
	__m128i a = _mm_loadl_epi64(src);
	a = _mm_shuffle_epi8(a, kMaskSwap32_2);
	_mm_storel_epi64(dst, a);
}

__forceinline void Float2ToFloat3sse3(__m128i* dst, const __m128i* src)
{
	__m128i a = _mm_loadu_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap32_2);
	_mm_storeu_si128(dst, a);
}

__forceinline void Float3ToFloat3sse3(__m128i* dst, const __m128i* src)
{
	__m128i a = _mm_loadu_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap32_3);
	_mm_storeu_si128(dst, a);
}

__forceinline void Float4ToFloat4sse3(__m128i* dst, const __m128i* src)
{
	__m128i a = _mm_loadu_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap32_4);
	_mm_storeu_si128(dst, a);
}

#endif

#if _M_SSE >= 0x401
static const __m128i kMaskSwap16_2 = _mm_set_epi32(0xFFFFFFFFL, 0xFFFFFFFFL, 0xFFFFFFFFL, 0x02030001L);
static const __m128i kMaskSwap16_3 = _mm_set_epi32(0xFFFFFFFFL, 0xFFFFFFFFL, 0xFFFF0405L, 0x02030001L);

__forceinline void Short2ToFloat2sse4(__m64* dst, const s32 src, const float* scale)
{
	__m128i a = _mm_cvtsi32_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_2);
	a = _mm_cvtepi16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storel_pi(dst, b);
}

__forceinline void Short2ToFloat3sse4(float* dst, const s32 src, const float* scale)
{
	__m128i a = _mm_cvtsi32_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_2);
	a = _mm_cvtepi16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storeu_ps(dst, b);
}

__forceinline void Short3ToFloat3sse4(float* dst, const __m128i* src, const float* scale)
{
	__m128i a = _mm_loadl_epi64(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_3);
	a = _mm_cvtepi16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storeu_ps(dst, b);
}

__forceinline void UShort2ToFloat2sse4(__m64* dst, const s32 src, const float* scale)
{
	__m128i a = _mm_cvtsi32_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_2);
	a = _mm_cvtepu16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storel_pi(dst, b);
}

__forceinline void UShort2ToFloat3sse4(float* dst, const s32 src, const float* scale)
{
	__m128i a = _mm_cvtsi32_si128(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_2);
	a = _mm_cvtepu16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storeu_ps(dst, b);
}

__forceinline void UShort3ToFloat3sse4(float* dst, const __m128i* src, const float* scale)
{
	__m128i a = _mm_loadl_epi64(src);
	a = _mm_shuffle_epi8(a, kMaskSwap16_3);
	a = _mm_cvtepu16_epi32(a);
	__m128 b = _mm_cvtepi32_ps(a);
	const __m128 c = _mm_load1_ps(scale);
	b = _mm_mul_ps(b, c);
	_mm_storeu_ps(dst, b);
}
#endif