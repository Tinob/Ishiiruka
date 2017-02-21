// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Modified For Ishiiruka By Tino
#pragma once
#include <cstring>
#include "Common/CommonFuncs.h"
#include "Common/CommonTypes.h"

class DataReader
{
public:
	inline DataReader()
		: Rbuffer(nullptr), end(nullptr)
	{}

	inline DataReader(u8* src, u8* _end)
		: Rbuffer(src), end(_end)
	{}

	inline const u8* GetPointer() const
	{
		return Rbuffer;
	}

	inline u8* operator=(u8* src)
	{
		Rbuffer = src;
		return src;
	}

	inline size_t size() const
	{
		return end - Rbuffer;
	}
	inline void ReadSkip(u32 skip)
	{
		Rbuffer += skip;
	}

	template <typename T> inline T Peek(s32 _uOffset) const
	{
		auto const result = Common::FromBigEndian(*reinterpret_cast<const T*>(Rbuffer + _uOffset));
		return result;
	}

	template <typename T> inline T Peek() const
	{
		auto const result = Common::FromBigEndian(*reinterpret_cast<const T*>(Rbuffer));
		return result;
	}

	template <typename T> inline T PeekUnswapped(s32 _uOffset) const
	{
		auto const result = *reinterpret_cast<const T*>(Rbuffer + _uOffset);
		return result;
	}

	template <typename T> inline T PeekUnswapped() const
	{
		auto const result = *reinterpret_cast<const T*>(Rbuffer);
		return result;
	}

	template <typename T, bool swap = true> inline T Read()
	{
		auto const result = PeekUnswapped<T>();
		ReadSkip(sizeof(T));
		if (swap)
		{
			return Common::FromBigEndian(result);
		}
		return result;
	}

	template <typename T> inline T ReadUnswapped()
	{
		auto const result = PeekUnswapped<T>();
		ReadSkip(sizeof(T));
		return result;
	}

	inline u8* GetReadPosition() const
	{
		return Rbuffer;
	}

	inline u8* GetEnd() const
	{
		return end;
	}

	inline void SetReadPosition(u8 *source, u8 *e)
	{
		Rbuffer = source;
		end = e;
	}

	inline void SetReadPosition(u8 *source)
	{
		Rbuffer = source;
	}

	template<unsigned int N>
	inline void ReadU32xN(u32 *dst)
	{
		u32* src = (u32*)Rbuffer;
		if (N >= 1) *dst++ = Common::swap32(*src++);
		if (N >= 2) *dst++ = Common::swap32(*src++);
		if (N >= 3) *dst++ = Common::swap32(*src++);
		if (N >= 4) *dst++ = Common::swap32(*src++);
		if (N >= 5) *dst++ = Common::swap32(*src++);
		if (N >= 6) *dst++ = Common::swap32(*src++);
		if (N >= 7) *dst++ = Common::swap32(*src++);
		if (N >= 8) *dst++ = Common::swap32(*src++);
		if (N >= 9) *dst++ = Common::swap32(*src++);
		if (N >= 10) *dst++ = Common::swap32(*src++);
		if (N >= 11) *dst++ = Common::swap32(*src++);
		if (N >= 12) *dst++ = Common::swap32(*src++);
		if (N >= 13) *dst++ = Common::swap32(*src++);
		if (N >= 14) *dst++ = Common::swap32(*src++);
		if (N >= 15) *dst++ = Common::swap32(*src++);
		if (N >= 16) *dst++ = Common::swap32(*src++);
		Rbuffer = (u8*)src;
	}
protected:
	u8* __restrict Rbuffer;
	u8* end;
};

class DataWriter
{
public:
	inline DataWriter(u8 *destination) : Wbuffer(destination)
	{}
	inline DataWriter() : Wbuffer(nullptr)
	{}
	inline void WriteSkip(u32 skip)
	{
		Wbuffer += skip;
	}

	template <typename T> inline void Write(T data)
	{
		*((T*)Wbuffer) = data;
		WriteSkip(sizeof(T));
	}

	inline u8* GetWritePosition() const
	{
		return Wbuffer;
	}

	inline void SetWritePosition(u8 *destination)
	{
		Wbuffer = destination;
	}
protected:
	u8* __restrict Wbuffer;
};

extern DataReader g_VideoData;
