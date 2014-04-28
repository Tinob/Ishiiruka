// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "D3DBase.h"
#include "D3DTexture.h"

#include "Common/CPUDetect.h"

#if _M_SSE >= 0x401
#include <smmintrin.h>
#include <emmintrin.h>
#elif _M_SSE >= 0x301 && !(defined __GNUC__ && !defined __SSSE3__)
#include <tmmintrin.h>
#endif

namespace DX9
{

namespace D3D
{

void ConvertRGBA_BGRA_SSE2(u32 *dst, const s32 dstPitch, u32 *pIn, const s32 width, const s32 height, const s32 pitch)
{
	// Converts RGBA to BGRA:
	// TODO: this would be totally unnecessary if we just change the TextureDecoder_RGBA to decode
	// to BGRA instead.
	for (s32 y = 0; y < height; y++, pIn += pitch)
	{
		u8 *pIn8 = (u8 *)pIn;
		u8 *pBits = (u8 *)((u8*)dst + (y * dstPitch));

		// Batch up loads/stores into 16 byte chunks to use SSE2 efficiently:
		s32 sse2blocks = (width * 4) / 16;
		s32 sse2remainder = (width * 4) & 15;

		// Do conversions in batches of 16 bytes:
		if (sse2blocks > 0)
		{
			// Generate a constant of all FF bytes:
			const __m128i allFFs128 = _mm_cmpeq_epi32(_mm_setzero_si128(), _mm_setzero_si128());
			__m128i *src128 = (__m128i *)pIn8;
			__m128i *dst128 = (__m128i *)pBits;

			// Increment by 16 bytes at a time:
			for (int i = 0; i < sse2blocks; ++i, ++dst128, ++src128)
			{
				// Load up 4 colors simultaneously:
				__m128i rgba = _mm_loadu_si128(src128);
				// Swap the R and B components:
				// Isolate the B component and shift it left 16 bits:
				// ABGR
				const __m128i bMask = _mm_srli_epi32(allFFs128, 24);
				const __m128i bNew = _mm_slli_epi32(_mm_and_si128(rgba, bMask), 16);
				// Isolate the R component and shift it right 16 bits:
				const __m128i rMask = _mm_slli_epi32(bMask, 16);
				const __m128i rNew = _mm_srli_epi32(_mm_and_si128(rgba, rMask), 16);
				// Now mask off the old R and B components from the rgba data to get 0g0a:
				const __m128i _g_a = _mm_or_si128(
					_mm_and_si128(
						rgba,
						_mm_or_si128(
							_mm_slli_epi32(bMask, 8),
							_mm_slli_epi32(rMask, 8)
						)
					),
					_mm_or_si128(rNew, bNew)
				);
				// Finally, OR up all the individual components to get BGRA:
				const __m128i bgra = _mm_or_si128(_g_a, _mm_or_si128(rNew, bNew));
				_mm_storeu_si128(dst128, bgra);
			}
		}

		// Take the remainder colors at the end of the row that weren't able to
		// be included into the last 16 byte chunk:
		if (sse2remainder > 0)
		{
			for (s32 x = (sse2blocks * 16); x < (width * 4); x += 4)
			{
				pBits[x + 0] = pIn8[x + 2];
				pBits[x + 1] = pIn8[x + 1];
				pBits[x + 2] = pIn8[x + 0];
				pBits[x + 3] = pIn8[x + 3];
			}
		}
	}

	// Memory fence to make sure the stores are good:
	_mm_mfence();
}

void ConvertRGBA_BGRA_SSSE3(u32 *dst, const s32 dstPitch, u32 *pIn, const s32 width, const s32 height, const s32 pitch)
{
	__m128i mask = _mm_set_epi8(15, 12, 13, 14, 11, 8, 9, 10, 7, 4, 5, 6, 3, 0, 1, 2);
	for (s32 y = 0; y < height; y++, pIn += pitch)
	{
		u8 *pIn8 = (u8 *)pIn;
		u8 *pBits = (u8 *)((u8*)dst + (y * dstPitch));

		// Batch up loads/stores into 16 byte chunks to use SSE2 efficiently:
		int ssse3blocks = (width * 4) / 16;
		int ssse3remainder = (width * 4) & 15;

		// Do conversions in batches of 16 bytes:
		if (ssse3blocks > 0)
		{
			__m128i *src128 = (__m128i *)pIn8;
			__m128i *dst128 = (__m128i *)pBits;

			// Increment by 16 bytes at a time:
			for (s32 i = 0; i < ssse3blocks; ++i, ++dst128, ++src128)
			{
				_mm_storeu_si128(dst128, _mm_shuffle_epi8(_mm_loadu_si128(src128), mask));
			}
		}

		// Take the remainder colors at the end of the row that weren't able to
		// be included into the last 16 byte chunk:
		if (ssse3remainder > 0)
		{
			for (s32 x = (ssse3blocks * 16); x < (width * 4); x += 4)
			{
				pBits[x + 0] = pIn8[x + 2];
				pBits[x + 1] = pIn8[x + 1];
				pBits[x + 2] = pIn8[x + 0];
				pBits[x + 3] = pIn8[x + 3];
			}
		}
	}

	// Memory fence to make sure the stores are good:
	_mm_mfence();
}

inline void CopyTextureData(u8 *pDst, const u8 *pSrc, const s32 width, const s32 height, const s32 srcpitch, const s32 dstpitch, const s32 pixelsize)
{
	const s32 rowsize = width * pixelsize;
	if (srcpitch == dstpitch && srcpitch == rowsize)
	{
		memcpy(pDst, pSrc, rowsize * height);
	}
	else
	{
		for (int y = 0; y < height; y++)
		{
			memcpy(pDst, pSrc, rowsize);
			pSrc += srcpitch;
			pDst += dstpitch;
		}
	}
}

inline void ExpandI8Data(u8 *pDst, const u8 *pSrc, const s32 width, const s32 height, const s32 srcpitch, const s32 dstpitch)
{
	for (int y = 0; y < height; y++)
	{
		for (int i = 0; i < width * 2; i += 2) 
		{
			u8 data = pSrc[i >> 1];
			pDst[i] = data;
			pDst[i + 1] = data;
		}
		pSrc += srcpitch;
		pDst += dstpitch;
	}
}

inline void CopyCompressedTextureData(u8 *pDst, const u8 *pSrc, const s32 width, const s32 height, D3DFORMAT fmt,  const s32 dstpitch)
{
	s32 numBlocksWide = (width + 3) >> 2;
	s32 numBlocksHigh = (height + 3) >> 2;
	s32 numBytesPerBlock = (fmt == D3DFMT_DXT1 ? 8 : 16);
	s32 rowBytes = numBlocksWide * numBytesPerBlock;
	s32 numRows = numBlocksHigh;
	if (rowBytes == dstpitch)
	{
		memcpy(pDst, pSrc, rowBytes * numRows);
	}
	else
	{
		u8* pDestBits = pDst;
		const u8* pSrcBits = pSrc;
		// Copy stride line by line   
		for (s32 h = 0; h < numRows; h++)
		{
			memcpy(pDestBits, pSrcBits, rowBytes);
			pDestBits += dstpitch;
			pSrcBits += rowBytes;
		}
	}
}

inline void LoadDataToRect(D3DLOCKED_RECT &Lock, const u8* buffer, const int width, const int height, const int pitch, D3DFORMAT fmt, const bool swap_r_b)
{
	s32 pixelsize = 0;
	switch (fmt)
	{
	case D3DFMT_L8:
	case D3DFMT_A8:
	case D3DFMT_A4L4:
		pixelsize = 1;
	break;
	case D3DFMT_R5G6B5:
		pixelsize = 2;
	break;
	case D3DFMT_A8P8:
		ExpandI8Data((u8*)Lock.pBits, buffer, width, height, pitch, Lock.Pitch);
	break;
	case D3DFMT_A8L8:
		pixelsize = 2;
	break;
	case D3DFMT_A8R8G8B8:
		if (!swap_r_b) {
			pixelsize = 4;
		}
		else {
	#if _M_SSE >= 0x301
			// Uses SSSE3 intrinsics to optimize RGBA -> BGRA swizzle:
			if (cpu_info.bSSSE3) {
				ConvertRGBA_BGRA_SSSE3((u32 *)Lock.pBits, Lock.Pitch, (u32*)buffer, width, height, pitch);
			}
			else
	#endif
				// Uses SSE2 intrinsics to optimize RGBA -> BGRA swizzle:
			{
				ConvertRGBA_BGRA_SSE2((u32 *)Lock.pBits, Lock.Pitch, (u32*)buffer, width, height, pitch);
			}
		}
	break;
	case D3DFMT_DXT1:
	case D3DFMT_DXT3:
	case D3DFMT_DXT5:
		CopyCompressedTextureData((u8*)Lock.pBits, buffer, width, height, fmt, Lock.Pitch);
	break;
	default:
		PanicAlert("D3D: Invalid texture format %i", fmt);
	}
	if (pixelsize > 0)
	{
		CopyTextureData((u8*)Lock.pBits, buffer, width, height, pitch * pixelsize, Lock.Pitch, pixelsize);
	}
}

LPDIRECT3DTEXTURE9 CreateTexture2D(const u8* buffer, const int width, const int height, const int pitch, D3DFORMAT fmt, bool swap_r_b, int levels)
{
	LPDIRECT3DTEXTURE9 pTexture;
	D3DFORMAT cfmt = fmt;
	if (fmt == D3DFMT_A8P8) {
		cfmt = D3DFMT_A8L8;
	}
	if (FAILED(dev->CreateTexture(width, height, levels, 0, cfmt, D3DPOOL_MANAGED, &pTexture, NULL)))
	{
		return 0;
	}
	int level = 0;
	D3DLOCKED_RECT Lock;
	pTexture->LockRect(level, &Lock, NULL, 0);
	LoadDataToRect(Lock, buffer, width, height, pitch, fmt, swap_r_b);
	pTexture->UnlockRect(level); 
	return pTexture;
}

void ReplaceTexture2D(LPDIRECT3DTEXTURE9 pTexture, const u8* buffer, const int width, const int height, const int pitch, D3DFORMAT fmt, bool swap_r_b, int level)
{
	D3DLOCKED_RECT Lock;
	pTexture->LockRect(level, &Lock, NULL, 0);
	LoadDataToRect(Lock, buffer, width, height, pitch, fmt, swap_r_b);
	pTexture->UnlockRect(level); 
}

}  // namespace

}  // namespace DX9