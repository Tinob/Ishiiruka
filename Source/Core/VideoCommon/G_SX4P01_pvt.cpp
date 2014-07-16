#include "VideoCommon/G_SX4P01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u16>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u16>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_0_I8_6666_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_0_Dir_565_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Color_ReadDirect_16b_565(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_0_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_u16_T2_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u16>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_T5_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u8, s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u8, s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_T3_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Color_ReadDirect_32b_8888(pipelinestate);
	_TexCoord_ReadDirect<u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx1_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_T4_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadDirect<u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u8, s8, 2>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u8_T4_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x301
	if (iSSE >= 0x301)
	{
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if(iSSE >= 0x401)
	{
		_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadDirect_16x3_SSE4<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadDirect_16x2_SSE4<true>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<s16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_16b_4444<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_SX4P01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 264745006
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170638154622128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170638154622128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170638154622128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 103260213
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559277974704] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559277974704] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559277974704] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 86397680
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120677053823664] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120677053823664] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120677053823664] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 81317275
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167673438217392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167673438217392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167673438217392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 46482340
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[721754671622255] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[721754671622255] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[721754671622255] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 34058348
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[11806265431344751] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[11806265431344751] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[11806265431344751] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 32545775
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724719388027265] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724719388027265] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[724719388027265] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 26266740
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519499048529] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519499048529] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519499048529] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 26026816
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598177176240] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598177176240] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598177176240] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 23122872
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[11803379591587439] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[11803379591587439] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[11803379591587439] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 22272019
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[166951978278064] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[166951978278064] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[166951978278064] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 15426999
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906987985150] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906987985150] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906987985150] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 15132462
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[11806344379960193] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[11806344379960193] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[11806344379960193] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 13788631
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121500002100400] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121500002100400] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121500002100400] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 13612448
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119233768775344] = P_mtx0_3_I8_flt_C0_0_I8_6666_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119233768775344] = P_mtx0_3_I8_flt_C0_0_I8_6666_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[119233768775344] = P_mtx0_3_I8_flt_C0_0_I8_6666_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 10927416
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121421279208624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121421279208624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121421279208624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 10228308
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121500046864465] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121500046864465] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121500046864465] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 9696456
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490808812909310] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490808812909310] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[490808812909310] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 9559326
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120778542161072] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120778542161072] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120778542161072] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 8578272
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116166333988016] = P_mtx0_3_Dir_flt_C0_0_Dir_565_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116166333988016] = P_mtx0_3_Dir_flt_C0_0_Dir_565_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[116166333988016] = P_mtx0_3_Dir_flt_C0_0_Dir_565_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 8546331
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598221902767] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598221902767] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598221902767] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 8537114
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167673482943919] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167673482943919] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167673482943919] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 8317120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2012265773379021] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2012265773379021] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2012265773379021] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 6784550
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168473717145776] = P_mtx0_3_I16_flt_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168473717145776] = P_mtx0_3_I16_flt_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[168473717145776] = P_mtx0_3_I16_flt_C0_0_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 6396232
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[491327039996289] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[491327039996289] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[491327039996289] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 5483664
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121421323972689] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121421323972689] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121421323972689] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 5039304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167594760089681] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167594760089681] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167594760089681] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 4900500
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121240958987857] = P_mtx1_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121240958987857] = P_mtx1_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121240958987857] = P_mtx1_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 4565089
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906635709789] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906635709789] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906635709789] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 4409942
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2938079605588317] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_u16_T2_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2938079605588317] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_u16_T2_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2938079605588317] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_u16_T2_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 4099548
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[11802658131648111] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[11802658131648111] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[11802658131648111] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 4057411
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519454284464] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519454284464] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519454284464] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 3880164
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490887491037295] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490887491037295] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[490887491037295] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 3850244
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167673130668558] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167673130668558] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167673130668558] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 3829614
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2941044322274177] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2941044322274177] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[2941044322274177] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_0_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 3792294
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724561788487517] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724561788487517] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[724561788487517] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 3568508
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119234076286640] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119234076286640] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[119234076286640] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 3526536
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120597869664944] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120597869664944] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120597869664944] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 3201514
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1512993463182169000] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1512993463182169000] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1512993463182169000] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 3021900
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121939551059120] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121939551059120] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121939551059120] = P_mtx0_3_Dir_flt_C0_0_I16_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 2923650
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120778586887599] = P_mtx1_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120778586887599] = P_mtx1_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120778586887599] = P_mtx1_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2872040
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120699864033361] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120699864033361] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120699864033361] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 2867966
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[47217733586851439] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[47217733586851439] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[47217733586851439] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 2864876
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598221940305] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598221940305] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598221940305] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2660400
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490887183525999] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490887183525999] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[490887183525999] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2482440
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[126081860871019855] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_T5_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[126081860871019855] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_T5_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[126081860871019855] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_T5_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2314660
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121421323935151] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121421323935151] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121421323935151] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 2182934
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598138737328] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598138737328] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598138737328] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1854748
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877735696718959] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877735696718959] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877735696718959] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1730622
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[10879910869247599] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[10879910869247599] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[10879910869247599] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1665394
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519146773168] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519146773168] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519146773168] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 1637568
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251981167455131] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251981167455131] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[251981167455131] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 1416212
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119256841732272] = P_mtx0_3_I16_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119256841732272] = P_mtx0_3_I16_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119256841732272] = P_mtx0_3_I16_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 1384154
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121319637115568] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121319637115568] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121319637115568] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1366398
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[720954488791151] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[720954488791151] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[720954488791151] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1302988
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67287422985930] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67287422985930] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67287422985930] = P_mtx0_3_I8_s8_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 1280339
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490064542760833] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490064542760833] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[490064542760833] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 1273684
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985666113135] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985666113135] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985666113135] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1269496
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121420971659790] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121420971659790] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121420971659790] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 1263456
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167594715325616] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167594715325616] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167594715325616] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 1216842
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[8062350718295151] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[8062350718295151] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[8062350718295151] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1195070
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519146735630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519146735630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519146735630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 946530
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519499010991] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519499010991] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519499010991] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 865230
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7878457156658287] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7878457156658287] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7878457156658287] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 860700
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7878378433626223] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7878378433626223] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7878378433626223] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 843588
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120699780830384] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120699780830384] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120699780830384] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 824940
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[166771613293232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[166771613293232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[166771613293232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 821940
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120778234612238] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120778234612238] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120778234612238] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 797008
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[721754716348782] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[721754716348782] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[721754716348782] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 755854
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119076476747440] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119076476747440] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119076476747440] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 692550
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906943183821] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906943183821] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906943183821] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 646098
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535622347887] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535622347887] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535622347887] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 638096
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519454246926] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519454246926] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519454246926] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 620912
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2199304627857007] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2199304627857007] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2199304627857007] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 616320
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119155199639216] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119155199639216] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119155199639216] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 541756
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[721033211682927] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[721033211682927] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[721033211682927] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 534776
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967456899456111] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967456899456111] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967456899456111] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 523020
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121240958950319] = P_mtx1_3_I8_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121240958950319] = P_mtx1_3_I8_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121240958950319] = P_mtx1_3_I8_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 506942
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535622347613] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535622347613] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535622347613] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 480912
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120361700952078] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120361700952078] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120361700952078] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 471884
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598177138702] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598177138702] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598177138702] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 465628
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985710877200] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985710877200] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985710877200] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 459795
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877735658280047] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877735658280047] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877735658280047] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 440154
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535622310349] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535622310349] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535622310349] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 436414
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906943221085] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906943221085] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906943221085] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 422904
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120597831188494] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120597831188494] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120597831188494] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 419424
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877656973827183] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877656973827183] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877656973827183] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 418768
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489749542201050] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489749542201050] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[489749542201050] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 418396
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1512993463220607912] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1512993463220607912] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1512993463220607912] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I16_s16_T4_mtx0_1_I16_s16_T5_mtx0_1_I16_s16_T6_mtx0_1_I16_s16_T7_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 401208
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519415845552] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519415845552] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519415845552] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 390906
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535622170061] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535622170061] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535622170061] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 383775
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877814645334401] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_T3_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877814645334401] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_T3_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877814645334401] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_T3_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 381388
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2891105986780783] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2891105986780783] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2891105986780783] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 364940
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31518663703419741] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31518663703419741] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[31518663703419741] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_T4_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 336500
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120361970062000] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120361970062000] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120361970062000] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 316836
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69679697616048] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69679697616048] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[69679697616048] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u8_<0>;
	}
	// num_verts= 287156
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118919030963888] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118919030963888] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118919030963888] = P_mtx0_3_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 282186
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2706490965204591] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2706490965204591] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2706490965204591] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 265198
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985666112861] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985666112861] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985666112861] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 264708
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985666075323] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985666075323] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985666075323] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 255408
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167594407776782] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167594407776782] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167594407776782] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 252492
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967614499135873] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967614499135873] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967614499135873] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 228216
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[8801204419058799] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[8801204419058799] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[8801204419058799] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_T2_mtx0_1_I16_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 223664
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906635672251] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906635672251] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906635672251] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 196992
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985666075597] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985666075597] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985666075597] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 191968
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906597270877] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906597270877] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906597270877] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 159980
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119076784258736] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119076784258736] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119076784258736] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 136500
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167594760052143] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167594760052143] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167594760052143] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I16_u16_<0>;
	}
	// num_verts= 128874
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489908397049056] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx1_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489908397049056] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx1_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489908397049056] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx1_1_I8_u16_<0>;
	}
	// num_verts= 128448
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120597869627406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120597869627406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120597869627406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 109360
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877735624891503] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877735624891503] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877735624891503] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 100408
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490808812871772] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490808812871772] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[490808812871772] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 95130
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[7877735696718685] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[7877735696718685] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[7877735696718685] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 84744
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121240914223792] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121240914223792] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[121240914223792] = P_mtx0_3_I8_flt_Nrm_0_0_I16_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 84376
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251800802564144] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251800802564144] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[251800802564144] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 82544
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[490808460596411] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[490808460596411] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[490808460596411] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 81726
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906943221359] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906943221359] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906943221359] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 79032
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119234230042288] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119234230042288] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[119234230042288] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 76474
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985358564301] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985358564301] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985358564301] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 67950
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535622207599] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535622207599] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535622207599] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 60652
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31518626855935087] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_T4_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31518626855935087] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_T4_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[31518626855935087] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u16_T4_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 52272
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906949546238] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906949546238] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906949546238] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 51956
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66792166546608] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66792166546608] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66792166546608] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_u8_<0>;
	}
	// num_verts= 42484
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118918723321209] = P_mtx0_2_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118918723321209] = P_mtx0_2_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118918723321209] = P_mtx0_2_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 38520
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120440692953776] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120440692953776] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120440692953776] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 37304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120677406099025] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120677406099025] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120677406099025] = P_mtx1_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 32040
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519108296718] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519108296718] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519108296718] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 30840
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31518663487937647] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u8_T4_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31518663487937647] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u8_T4_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[31518663487937647] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_T3_mtx0_1_I8_u8_T4_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 26728
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120362008500912] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120362008500912] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120362008500912] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 22616
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985627674223] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985627674223] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985627674223] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 21204
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985358601839] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985358601839] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985358601839] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 19592
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906635672525] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906635672525] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906635672525] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 17010
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119155507150512] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119155507150512] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119155507150512] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 16728
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120361970043231] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120361970043231] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120361970043231] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 11544
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119076169198606] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119076169198606] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119076169198606] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 11308
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967456899315823] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967456899315823] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967456899315823] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_1_I8_6666_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 10104
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120361662513166] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120361662513166] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120361662513166] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 9060
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118919030945119] = P_mtx0_2_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118919030945119] = P_mtx0_2_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118919030945119] = P_mtx0_2_I8_flt_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 8512
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1966171521598849] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1966171521598849] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[1966171521598849] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 7502
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985358564027] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985358564027] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985358564027] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 5544
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489906987947612] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489906987947612] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489906987947612] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 2912
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[488621565223809] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[488621565223809] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[488621565223809] = P_mtx0_3_I8_flt_C0_0_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 2056
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535583908975] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535583908975] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535583908975] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1664
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535314799053] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535314799053] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535314799053] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1116
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489749151449239] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489749151449239] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[489749151449239] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_4444_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 424
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120361970024462] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120361970024462] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120361970024462] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 336
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67028335184398] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67028335184398] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[67028335184398] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 180
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118918723415054] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118918723415054] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118918723415054] = P_mtx0_3_I8_s16_C0_1_I8_4444_T0_mtx0_1_I8_u8_<0>;
	}
}
