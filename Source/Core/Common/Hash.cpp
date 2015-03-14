// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <algorithm>
#include "xxhash.h"
#include "Common/Common.h"
#include "Common/CommonFuncs.h"
#include "Common/CPUDetect.h"
#include "Common/Hash.h"
#include "Common/Intrinsics.h"



// uint32_t
// WARNING - may read one more byte!
// Implementation from Wikipedia.
u32 HashFletcher(const u8* data_u8, size_t length)
{
	const u16* data = (const u16*)data_u8; /* Pointer to the data to be summed */
	size_t len = (length + 1) / 2; /* Length in 16-bit words */
	u32 sum1 = 0xffff, sum2 = 0xffff;

	while (len)
	{
		size_t tlen = len > 360 ? 360 : len;
		len -= tlen;

		do {
			sum1 += *data++;
			sum2 += sum1;
		}
		while (--tlen);

		sum1 = (sum1 & 0xffff) + (sum1 >> 16);
		sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	}

	// Second reduction step to reduce sums to 16 bits
	sum1 = (sum1 & 0xffff) + (sum1 >> 16);
	sum2 = (sum2 & 0xffff) + (sum2 >> 16);
	return(sum2 << 16 | sum1);
}


// Implementation from Wikipedia
// Slightly slower than Fletcher above, but slightly more reliable.
#define MOD_ADLER 65521
// data: Pointer to the data to be summed; len is in bytes
u32 HashAdler32(const u8* data, size_t len)
{
	u32 a = 1, b = 0;

	while (len)
	{
		size_t tlen = len > 5550 ? 5550 : len;
		len -= tlen;

		do
		{
			a += *data++;
			b += a;
		}
		while (--tlen);

		a = (a & 0xffff) + (a >> 16) * (65536 - MOD_ADLER);
		b = (b & 0xffff) + (b >> 16) * (65536 - MOD_ADLER);
	}

	// It can be shown that a <= 0x1013a here, so a single subtract will do.
	if (a >= MOD_ADLER)
	{
		a -= MOD_ADLER;
	}

	// It can be shown that b can reach 0xfff87 here.
	b = (b & 0xffff) + (b >> 16) * (65536 - MOD_ADLER);

	if (b >= MOD_ADLER)
	{
		b -= MOD_ADLER;
	}

	return((b << 16) | a);
}

// Stupid hash - but can't go back now :)
// Don't use for new things. At least it's reasonably fast.
u32 HashEctor(const u8* ptr, u32 length)
{
	u32 crc = 0;

	for (u32 i = 0; i < length; i++)
	{
		crc ^= ptr[i];
		crc = (crc << 3) | (crc >> 29);
	}

	return(crc);
}


#if _ARCH_64

// CRC32 hash using the SSE4.2 instruction
u64 GetCRC32(const u8 *src, u32 len, u32 samples)
{
#if _M_SSE >= 0x402
	u64 h[4] = { len, 0, 0, 0 };
	u32 Step = (len / 8);
	const u64 *data = (const u64 *)src;
	const u64 *end = data + Step;
	if (samples == 0) samples = std::max(Step, 1u);
	Step = Step / samples;
	if (Step < 1) Step = 1;
	while (data < end - Step * 3)
	{
		h[0] = _mm_crc32_u64(h[0], data[Step * 0]);
		h[1] = _mm_crc32_u64(h[1], data[Step * 1]);
		h[2] = _mm_crc32_u64(h[2], data[Step * 2]);
		h[3] = _mm_crc32_u64(h[3], data[Step * 3]);
		data += Step * 4;
	}
	if (data < end - Step * 0)
		h[0] = _mm_crc32_u64(h[0], data[Step * 0]);
	if (data < end - Step * 1)
		h[1] = _mm_crc32_u64(h[1], data[Step * 1]);
	if (data < end - Step * 2)
		h[2] = _mm_crc32_u64(h[2], data[Step * 2]);
	const u8 *data2 = (const u8*)end;
	u64 temp = 0;
	switch (len & 7)
	{
	case 7: temp += u64(data2[6]) << 48;
	case 6: temp += u64(data2[5]) << 40;
	case 5: temp += u64(data2[4]) << 32;
	case 4: temp += u64(data2[3]) << 24;
	case 3: temp += u64(data2[2]) << 16;
	case 2: temp += u64(data2[1]) << 8;
	case 1: temp += u64(data2[0]);
		h[0] = _mm_crc32_u64(h[0], temp);
	};
	// FIXME: is there a better way to combine these partial hashes?
	return h[0] + (h[1] << 10) + (h[2] << 21) + (h[3] << 32);
#else
	return 0;
#endif
}

u64 GetXXHash(const u8 *src, u32 len, u32 samples)
{
	return XXH64(src, len, samples);
}

/*
 * NOTE: This hash function is used for custom texture loading/dumping, so
 * it should not be changed, which would require all custom textures to be
 * recalculated for their new hash values. If the hashing function is
 * changed, make sure this one is still used when the legacy parameter is
 * true.
 */
u64 GetHashHiresTexture(const u8 *src, u32 len, u32 samples)
{
	const u64 m = 0xc6a4a7935bd1e995;
	u64 h = len * m;
	const int r = 47;
	u32 Step = (len / 8);
	const u64 *data = (const u64 *)src;
	const u64 *end = data + Step;
	if (samples == 0) samples = std::max(Step, 1u);
	Step = Step / samples;
	if (Step < 1) Step = 1;
	while (data < end)
	{
		u64 k = data[0];
		data+=Step;
		k *= m;
		k ^= k >> r;
		k *= m;
		h ^= k;
		h *= m;
	}

	const u8 * data2 = (const u8*)end;

	switch (len & 7)
	{
	case 7: h ^= u64(data2[6]) << 48;
	case 6: h ^= u64(data2[5]) << 40;
	case 5: h ^= u64(data2[4]) << 32;
	case 4: h ^= u64(data2[3]) << 24;
	case 3: h ^= u64(data2[2]) << 16;
	case 2: h ^= u64(data2[1]) << 8;
	case 1: h ^= u64(data2[0]);
			h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}
#else
// CRC32 hash using the SSE4.2 instruction
u64 GetCRC32(const u8 *src, u32 len, u32 samples)
{
#if _M_SSE >= 0x402
	u32 h = len;
	u32 Step = (len/4);
	const u32 *data = (const u32 *)src;
	const u32 *end = data + Step;
	if (samples == 0) samples = std::max(Step, 1u);
	Step  = Step / samples;
	if (Step < 1) Step = 1;
	while (data < end)
	{
		h = _mm_crc32_u32(h, data[0]);
		data += Step;
	}

	const u8 *data2 = (const u8*)end;
	return (u64)_mm_crc32_u32(h, u32(data2[0]));
#else
	return 0;
#endif
}

u64 GetXXHash(const u8 *src, int len, u32 samples)
{
	return (u64)XXH32(src, len, samples);
}

/*
 * FIXME: The old 32-bit version of this hash made different hashes than the
 * 64-bit version. Until someone can make a new version of the 32-bit one that
 * makes identical hashes, this is just a c/p of the 64-bit one.
 */
u64 GetHashHiresTexture(const u8 *src, u32 len, u32 samples)
{
	const u64 m = 0xc6a4a7935bd1e995ULL;
	u64 h = len * m;
	const int r = 47;
	u32 Step = (len / 8);
	const u64 *data = (const u64 *)src;
	const u64 *end = data + Step;
	if (samples == 0) samples = std::max(Step, 1u);
	Step = Step / samples;
	if (Step < 1) Step = 1;
	while (data < end)
	{
		u64 k = data[0];
		data+=Step;
		k *= m;
		k ^= k >> r;
		k *= m;
		h ^= k;
		h *= m;
	}

	const u8 * data2 = (const u8*)end;

	switch (len & 7)
	{
	case 7: h ^= u64(data2[6]) << 48;
	case 6: h ^= u64(data2[5]) << 40;
	case 5: h ^= u64(data2[4]) << 32;
	case 4: h ^= u64(data2[3]) << 24;
	case 3: h ^= u64(data2[2]) << 16;
	case 2: h ^= u64(data2[1]) << 8;
	case 1: h ^= u64(data2[0]);
			h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}
#endif

static u64(*ptrHashFunction)(const u8 *src, u32 len, u32 samples) = &GetXXHash;

u64 GetHash64(const u8 *src, u32 len, u32 samples)
{
	return ptrHashFunction(src, len, samples);
}

// sets the hash function used for the texture cache
void SetHash64Function()
{
#if _M_SSE >= 0x402
	// sse4.2 crc32 version used only on intel, in amd is slower than xxhash
	if (cpu_info.vendor == VENDOR_INTEL && cpu_info.bSSE4_2) 
	{
		ptrHashFunction = &GetCRC32;
	}
	else
#endif
	{
		ptrHashFunction = &GetXXHash;
	}
}



