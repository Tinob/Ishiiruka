// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Common.h"
#include "Common/CPUDetect.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoader_Normal.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VertexLoadingSSE.h"

// warning: mapping buffer should be disabled to use this
#define LOG_NORM()  // PRIM_LOG("norm: %f %f %f, ", ((float*)VertexManager::s_pCurBufferPointer)[-3], ((float*)VertexManager::s_pCurBufferPointer)[-2], ((float*)VertexManager::s_pCurBufferPointer)[-1]);

VertexLoader_Normal::Set VertexLoader_Normal::m_Table[NUM_NRM_TYPE][NUM_NRM_INDICES][NUM_NRM_ELEMENTS][NUM_NRM_FORMAT];
static bool VertexLoader_Normal_Initialized = false;
namespace
{

__forceinline float FracAdjust(s8 val)
{
	return val * fractionTable[6];
}

__forceinline float FracAdjust(u8 val)
{
	return val * fractionTable[7];
}

__forceinline float FracAdjust(s16 val)
{
	return val * fractionTable[14];
}

__forceinline float FracAdjust(u16 val)
{
	return val * fractionTable[15];
}

__forceinline float FracAdjust(float val)
{
	return val;
}

template <typename T, int N>
__forceinline void ReadIndirect(const T* data)
{
	static_assert(3 == N || 9 == N, "N is only sane as 3 or 9!");

	for (int i = 0; i != N; ++i)
	{
		DataWrite(FracAdjust(Common::FromBigEndian(data[i])));
	}

	LOG_NORM();
}

template <typename T, int N>
struct Normal_Direct
{
	static void LOADERDECL function()
	{
		auto const source = reinterpret_cast<const T*>(DataGetPosition());
		ReadIndirect<T, N * 3>(source);
		DataSkip<N * 3 * sizeof(T)>();
	}

	static const int size = sizeof(T) * N * 3;
};

template <typename I, typename T, int Offset>
__forceinline u8* IndexedDataPosition()
{
	auto const index = DataRead<I>();
	return cached_arraybases[ARRAY_NORMAL]
		+ (index * arraystrides[ARRAY_NORMAL]) + sizeof(T) * 3 * Offset;
}

template <typename I, typename T, int N, int Offset>
__forceinline void Normal_Index_Offset()
{
	static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
	
	auto const data = reinterpret_cast<const T*>(IndexedDataPosition<I, T, Offset>());
	ReadIndirect<T, N * 3>(data);
}

template <typename I, typename T, int N>
struct Normal_Index
{
	static void LOADERDECL function()
	{
		Normal_Index_Offset<I, T, N, 0>();
	}

	static const int size = sizeof(I);
};

template <typename I, typename T>
struct Normal_Index_Indices3
{
	static void LOADERDECL function()
	{
		Normal_Index_Offset<I, T, 1, 0>();
		Normal_Index_Offset<I, T, 1, 1>();
		Normal_Index_Offset<I, T, 1, 2>();
	}

	static const int size = sizeof(I) * 3;
};

#if _M_SSE >= 0x301
template <int N>
struct Normal_Direct_FLOAT_SSSE3
{
	static void LOADERDECL function()
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

	static const int size = sizeof(float) * N * 3;
};

template <typename I, int N>
struct Normal_Index_FLOAT_SSSE3
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

		const float* src = reinterpret_cast<const float*>(IndexedDataPosition<I, float, 0>());
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

	static const int size = sizeof(I);
};

template <typename I>
struct Normal_Index3_FLOAT_SSSE3
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
		float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
		const float* src = reinterpret_cast<const float*>(IndexedDataPosition<I, float, 0>());
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
		dst += 3;
		src = reinterpret_cast<const float*>(IndexedDataPosition<I, float, 1>());
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
		dst += 3;
		src = reinterpret_cast<const float*>(IndexedDataPosition<I, float, 2>());
		Float3ToFloat3sse3((__m128i*)dst, (const __m128i*)src);
		dst += 3;
		VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);		
	}

	static const int size = sizeof(I) * 3;
};
#endif

#if _M_SSE >= 0x401
template <int N>
struct Normal_Direct_S16_SSSE4
{
	static void LOADERDECL function()
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

	static const int size = sizeof(s16) * N * 3;
};

template <int N>
struct Normal_Direct_U16_SSSE4
{
	static void LOADERDECL function()
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

	static const int size = sizeof(u16) * N * 3;
};

template <typename I, int N>
struct Normal_Index_S16_SSE4
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

		const s16* src = reinterpret_cast<const s16*>(IndexedDataPosition<I, s16, 0>());
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

	static const int size = sizeof(I);
};

template <typename I, int N>
struct Normal_Index_U16_SSE4
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

		const u16* src = reinterpret_cast<const u16*>(IndexedDataPosition<I, u16, 0>());
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

	static const int size = sizeof(I);
};

template <typename I>
struct Normal_Index3_S16_SSE4
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
		float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
		const s16* src = reinterpret_cast<const s16*>(IndexedDataPosition<I, s16, 0>());
		float scale = fractionTable[14];
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		src = reinterpret_cast<const s16*>(IndexedDataPosition<I, s16, 0>());
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		src = reinterpret_cast<const s16*>(IndexedDataPosition<I, s16, 0>());
		Short3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
	}

	static const int size = sizeof(I) * 3;
};

template <typename I>
struct Normal_Index3_U16_SSE4
{
	static void LOADERDECL function()
	{
		static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");
		float* dst = reinterpret_cast<float*>(VertexManager::s_pCurBufferPointer);
		const u16* src = reinterpret_cast<const u16*>(IndexedDataPosition<I, u16, 0>());
		float scale = fractionTable[15];
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		src = reinterpret_cast<const u16*>(IndexedDataPosition<I, u16, 0>());
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		src = reinterpret_cast<const u16*>(IndexedDataPosition<I, u16, 0>());
		UShort3ToFloat3sse4(dst, (const __m128i*)src, &scale);
		dst += 3;
		VertexManager::s_pCurBufferPointer = reinterpret_cast<u8*>(dst);
	}

	static const int size = sizeof(I) * 3;
};
#endif


}

void VertexLoader_Normal::Init(void)
{
	if (VertexLoader_Normal_Initialized)
	{
		return;
	}
	VertexLoader_Normal_Initialized = true;
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Direct<u8, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT] [FORMAT_BYTE]   = Normal_Direct<s8, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT] [FORMAT_USHORT]	= Normal_Direct<u16, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT] [FORMAT_SHORT] 	= Normal_Direct<s16, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Direct<float, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Direct<u8, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT3][FORMAT_BYTE]  	= Normal_Direct<s8, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT3][FORMAT_USHORT]	= Normal_Direct<u16, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] 	= Normal_Direct<s16, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct<float, 3>();

	// Same as above
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Direct<u8, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT] [FORMAT_BYTE]  	= Normal_Direct<s8, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT] [FORMAT_USHORT]	= Normal_Direct<u16, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT] [FORMAT_SHORT] 	= Normal_Direct<s16, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Direct<float, 1>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Direct<u8, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT3][FORMAT_BYTE]  	= Normal_Direct<s8, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT3][FORMAT_USHORT]	= Normal_Direct<u16, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] 	= Normal_Direct<s16, 3>();
	m_Table[NRM_DIRECT] [NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] 	= Normal_Direct<float, 3>();

	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Index<u8, u8, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT] [FORMAT_BYTE]  	= Normal_Index<u8, s8, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT] [FORMAT_USHORT]	= Normal_Index<u8, u16, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT] [FORMAT_SHORT] 	= Normal_Index<u8, s16, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Index<u8, float, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Index<u8, u8, 3>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT3][FORMAT_BYTE]  	= Normal_Index<u8, s8, 3>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT3][FORMAT_USHORT]	= Normal_Index<u8, u16, 3>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] 	= Normal_Index<u8, s16, 3>();
	m_Table[NRM_INDEX8] [NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] 	= Normal_Index<u8, float, 3>();

	// Same as above for NRM_NBT
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Index<u8, u8, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT] [FORMAT_BYTE]  	= Normal_Index<u8, s8, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT] [FORMAT_USHORT]	= Normal_Index<u8, u16, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT] [FORMAT_SHORT] 	= Normal_Index<u8, s16, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Index<u8, float, 1>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Index_Indices3<u8, u8>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT3][FORMAT_BYTE]  	= Normal_Index_Indices3<u8, s8>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT3][FORMAT_USHORT]	= Normal_Index_Indices3<u8, u16>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] 	= Normal_Index_Indices3<u8, s16>();
	m_Table[NRM_INDEX8] [NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] 	= Normal_Index_Indices3<u8, float>();

	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Index<u16, u8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT] [FORMAT_BYTE]  	= Normal_Index<u16, s8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT] [FORMAT_USHORT]	= Normal_Index<u16, u16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT] [FORMAT_SHORT] 	= Normal_Index<u16, s16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Index<u16, float, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Index<u16, u8, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_BYTE]  	= Normal_Index<u16, s8, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT]	= Normal_Index<u16, u16, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] 	= Normal_Index<u16, s16, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] 	= Normal_Index<u16, float, 3>();

	// Same as above for NRM_NBT
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT] [FORMAT_UBYTE] 	= Normal_Index<u16, u8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT] [FORMAT_BYTE]  	= Normal_Index<u16, s8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT] [FORMAT_USHORT]	= Normal_Index<u16, u16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT] [FORMAT_SHORT] 	= Normal_Index<u16, s16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT] [FORMAT_FLOAT] 	= Normal_Index<u16, float, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] 	= Normal_Index_Indices3<u16, u8>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_BYTE]  	= Normal_Index_Indices3<u16, s8>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT]	= Normal_Index_Indices3<u16, u16>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] 	= Normal_Index_Indices3<u16, s16>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] 	= Normal_Index_Indices3<u16, float>();


#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<1>();		
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<3>();
		
		// Same as above
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<3>();

		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index3_FLOAT_SSSE3<u8>();

		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index3_FLOAT_SSSE3<u16>();
	}
#endif
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<3>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<3>();

		// Same as above
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<3>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<3>();

		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 3>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index3_U16_SSE4<u8>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index3_S16_SSE4<u8>();

		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 3>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index3_U16_SSE4<u16>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index3_S16_SSE4<u16>();
	}
#endif

}

unsigned int VertexLoader_Normal::GetSize(unsigned int _type,
	unsigned int _format, unsigned int _elements, unsigned int _index3)
{
	return m_Table[_type][_index3][_elements][_format].gc_size;
}

TPipelineFunction VertexLoader_Normal::GetFunction(unsigned int _type,
	unsigned int _format, unsigned int _elements, unsigned int _index3)
{
	TPipelineFunction pFunc = m_Table[_type][_index3][_elements][_format].function;
	return pFunc;
}
