// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified For Ishiiruka By Tino
#pragma once

#include "VideoCommon/VertexManagerBase.h"

#if _M_SSE >= 0x301 && !(defined __GNUC__ && !defined __SSSE3__)
#include <tmmintrin.h>
#endif

class DataReader
{
public:
	inline DataReader(const u8 *source) : Rbuffer(source) {}
	inline DataReader() : Rbuffer(nullptr) {}	
	__forceinline void ReadSkip(u32 skip)
	{
		Rbuffer += skip;
	}

	template <typename T> __forceinline T Peek(s32 _uOffset)
	{
		auto const result = Common::FromBigEndian(*reinterpret_cast<const T*>(Rbuffer + _uOffset));
		return result;
	}
	
	template <typename T> __forceinline T Peek()
	{
		auto const result = Common::FromBigEndian(*reinterpret_cast<const T*>(Rbuffer));
		return result;
	}

	template <typename T> __forceinline T PeekUnswapped(s32 _uOffset)
	{
		auto const result = *reinterpret_cast<const T*>(Rbuffer + _uOffset);
		return result;
	}

	template <typename T> __forceinline T PeekUnswapped()
	{
		auto const result = *reinterpret_cast<const T*>(Rbuffer);
		return result;
	}

	template <typename T> __forceinline T Read()
	{
		auto const result = Peek<T>();
		ReadSkip(sizeof(T));
		return result;
	}

	template <typename T> __forceinline T ReadUnswapped()
	{
		auto const result = PeekUnswapped<T>();
		ReadSkip(sizeof(T));
		return result;
	}

	__forceinline const u8* GetReadPosition()
	{
		return Rbuffer;
	}
	
	__forceinline void SetReadPosition(const u8 *source)
	{ 
		Rbuffer = source; 
	}

	#if _M_SSE >= 0x301
	const __m128i bs_mask = _mm_set_epi32(0x0C0D0E0FL, 0x08090A0BL, 0x04050607L, 0x00010203L);
	
	template<unsigned int N>
	void ReadU32xN_SSSE3(u32 *bufx16)
	{
		memcpy(bufx16, Rbuffer, sizeof(u32) * N);
		__m128i* buf = (__m128i *)bufx16;
		if (N>12) { _mm_store_si128(buf, _mm_shuffle_epi8(_mm_load_si128(buf), bs_mask)); buf++; }
		if (N>8)  { _mm_store_si128(buf, _mm_shuffle_epi8(_mm_load_si128(buf), bs_mask)); buf++; }
		if (N>4)  { _mm_store_si128(buf, _mm_shuffle_epi8(_mm_load_si128(buf), bs_mask)); buf++; }
		_mm_store_si128(buf, _mm_shuffle_epi8(_mm_load_si128(buf), bs_mask));
		Rbuffer += (sizeof(u32) * N);
	}
	
	#endif
	
	template<unsigned int N>
	void ReadU32xN(u32 *bufx16)
	{
		memcpy(bufx16, Rbuffer, sizeof(u32) * N);
		if (N >= 1) bufx16[0] = Common::swap32(bufx16[0]);
		if (N >= 2) bufx16[1] = Common::swap32(bufx16[1]);
		if (N >= 3) bufx16[2] = Common::swap32(bufx16[2]);
		if (N >= 4) bufx16[3] = Common::swap32(bufx16[3]);
		if (N >= 5) bufx16[4] = Common::swap32(bufx16[4]);
		if (N >= 6) bufx16[5] = Common::swap32(bufx16[5]);
		if (N >= 7) bufx16[6] = Common::swap32(bufx16[6]);
		if (N >= 8) bufx16[7] = Common::swap32(bufx16[7]);
		if (N >= 9) bufx16[8] = Common::swap32(bufx16[8]);
		if (N >= 10) bufx16[9] = Common::swap32(bufx16[9]);
		if (N >= 11) bufx16[10] = Common::swap32(bufx16[10]);
		if (N >= 12) bufx16[11] = Common::swap32(bufx16[11]);
		if (N >= 13) bufx16[12] = Common::swap32(bufx16[12]);
		if (N >= 14) bufx16[13] = Common::swap32(bufx16[13]);
		if (N >= 15) bufx16[14] = Common::swap32(bufx16[14]);
		if (N >= 16) bufx16[15] = Common::swap32(bufx16[15]);
		Rbuffer += (sizeof(u32) * N);
	}
private:
	const u8 *Rbuffer;
};

class DataWriter
{
public:
	inline DataWriter(u8 *destination) : Wbuffer(destination) {}
	inline DataWriter() : Wbuffer(nullptr) {}
	__forceinline void WriteSkip(u32 skip)
	{
		Wbuffer += skip;
	}

	template <typename T> __forceinline void Write(T data)
	{
		*((T*)Wbuffer) = data;
		WriteSkip(sizeof(T));
	}

	__forceinline u8* GetWritePosition()
	{
		return Wbuffer;
	}
	
	__forceinline void SetWritePosition(u8 *destination)
	{ 
		Wbuffer = destination; 
	}
private:
	u8 *Wbuffer;
};

extern DataReader g_VideoData;
typedef void(*DataReadU32xNfunc)(u32 *buf);
extern DataReadU32xNfunc DataReadU32xFuncs[16];