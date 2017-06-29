// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/VertexLoadingSSE.h"

template <typename T>
__forceinline float TCScale(TPipelineState &pipelinestate, T val)
{
  return val * pipelinestate.tcScale[pipelinestate.tcIndex];
}

template <>
__forceinline float TCScale(TPipelineState &pipelinestate, float val)
{
  return val;
}

template <typename T, s32 N>
__forceinline void _TexCoord_ReadDirect(TPipelineState &pipelinestate)
{
  for (s32 i = 0; i != N; ++i)
    pipelinestate.Write(TCScale(pipelinestate, pipelinestate.Read<T>()));
  ++pipelinestate.tcIndex;
}

template <typename T>
__forceinline u8* IndexedTCoordPosition(TPipelineState &pipelinestate)
{
  auto const index = pipelinestate.Read<T>();
  return cached_arraybases[ARRAY_TEXCOORD0 + pipelinestate.tcIndex] + (index * g_main_cp_state.array_strides[ARRAY_TEXCOORD0 + pipelinestate.tcIndex]);
}

template <typename I, typename T, s32 N>
__forceinline void _TexCoord_ReadIndex(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  auto const data = reinterpret_cast<const T*>(IndexedTCoordPosition<I>(pipelinestate));

  for (s32 i = 0; i != N; ++i)
    pipelinestate.Write(TCScale(pipelinestate, Common::FromBigEndian(data[i])));

  ++pipelinestate.tcIndex;
}

#if _M_SSE >= 0x301

__forceinline void _TexCoord_ReadDirect_UByte2_SSSE3(TPipelineState &pipelinestate)
{
  const u32* pData = reinterpret_cast<const u32 *>(pipelinestate.GetReadPosition());
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  UByte2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  pipelinestate.ReadSkip(2);
  ++pipelinestate.tcIndex;
}

__forceinline void _TexCoord_ReadDirect_SByte2_SSSE3(TPipelineState &pipelinestate)
{
  const u32* pData = reinterpret_cast<const u32 *>(pipelinestate.GetReadPosition());
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  SByte2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  pipelinestate.ReadSkip(2);
  ++pipelinestate.tcIndex;
}

__forceinline void _TexCoord_ReadDirect_UShort2_SSSE3(TPipelineState &pipelinestate)
{
  const u32* pData = reinterpret_cast<const u32 *>(pipelinestate.GetReadPosition());
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  UShort2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  pipelinestate.ReadSkip(2 * sizeof(u16));
  ++pipelinestate.tcIndex;
}

__forceinline void _TexCoord_ReadDirect_Short2_SSSE3(TPipelineState &pipelinestate)
{
  const u32* pData = reinterpret_cast<const u32 *>(pipelinestate.GetReadPosition());
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  Short2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  pipelinestate.ReadSkip(2 * sizeof(s16));
  ++pipelinestate.tcIndex;
}

__forceinline void _TexCoord_ReadDirect_Float2_SSSE3(TPipelineState &pipelinestate)
{
  const __m128i *pData = (const __m128i *)pipelinestate.GetReadPosition();
  Float2ToFloat2sse3((__m128i *)pipelinestate.GetWritePosition(), pData);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  pipelinestate.ReadSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}

template <typename I>
__forceinline void _TexCoord_ReadIndex_UByte2_SSSE3(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  const u32 *pData = reinterpret_cast<const u32 *>(IndexedTCoordPosition<I>(pipelinestate));
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  UByte2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}

template <typename I>
__forceinline void _TexCoord_ReadIndex_SByte2_SSSE3(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  const u32 *pData = reinterpret_cast<const u32 *>(IndexedTCoordPosition<I>(pipelinestate));
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  SByte2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}

template <typename I>
__forceinline void _TexCoord_ReadIndex_UShort2_SSSE3(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  const u32 *pData = reinterpret_cast<const u32 *>(IndexedTCoordPosition<I>(pipelinestate));
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  UShort2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}

template <typename I>
__forceinline void _TexCoord_ReadIndex_Short2_SSSE3(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  const u32 *pData = reinterpret_cast<const u32 *>(IndexedTCoordPosition<I>(pipelinestate));
  const __m128 scale = _mm_set_ps1(pipelinestate.tcScale[pipelinestate.tcIndex]);
  __m64* dst = reinterpret_cast<__m64 *>(pipelinestate.GetWritePosition());
  Short2ToFloat2_SSSE3(pData, scale, dst);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}

template <typename I>
__forceinline void _TexCoord_ReadIndex_Float2_SSSE3(TPipelineState &pipelinestate)
{
  static_assert(!std::numeric_limits<I>::is_signed, "Only unsigned I is sane!");

  const __m128i *pData = (const __m128i *)IndexedTCoordPosition<I>(pipelinestate);
  Float2ToFloat2sse3((__m128i *)pipelinestate.GetWritePosition(), pData);
  pipelinestate.WriteSkip(sizeof(float) * 2);
  ++pipelinestate.tcIndex;
}
#endif

enum ETcoordElements
{
  TC_ELEMENTS_1 = 0,
  TC_ELEMENTS_2 = 1,
  TC_ELEMENTS
};
