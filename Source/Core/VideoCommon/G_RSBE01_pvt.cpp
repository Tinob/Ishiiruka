#include "VideoCommon/G_RSBE01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);
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
void P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_3_Dir_flt_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_2_I8_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_2_I8_s8_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_u16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, u16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	Color_ReadIndex_16b_565<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
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
void P_mtx0_2_I8_s8_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_s16_C0_1_I8_8888_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_16b_565<u8>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_6666_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_24b_6666<u8>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	Color_ReadIndex_16b_565<u8>(pipelinestate);
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
void P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
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

void G_RSBE01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 38455022
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121580813100966] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121580813100966] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121580813100966] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0>;
	}	
	// num_verts= 7230741
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120678988176806] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120678988176806] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120678988176806] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 6180840
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161257657073840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161257657073840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161257657073840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 5887298
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121398686738096] = P_mtx0_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121398686738096] = P_mtx0_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121398686738096] = P_mtx0_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 5337840
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[166851015191121] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[166851015191121] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[166851015191121] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 5012160
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120677579074129] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120677579074129] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120677579074129] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 4818600
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22383095691103] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22383095691103] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[22383095691103] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_<0>;
	}
	// num_verts= 4721636
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22203063743407] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22203063743407] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[22203063743407] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_<0>;
	}
	// num_verts= 4509322
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120857591783600] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120857591783600] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120857591783600] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 4179647
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22202730706271] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22202730706271] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[22202730706271] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_<0>;
	}
	// num_verts= 4134556
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121400448116134] = P_mtx1_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121400448116134] = P_mtx1_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121400448116134] = P_mtx1_3_I8_flt_Nrm_0_0_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 4047360
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22203083000401] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22203083000401] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[22203083000401] = P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_<0>;
	}
	// num_verts= 2926946
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115084220938079] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115084220938079] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[115084220938079] = P_mtx0_2_I16_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 2097436
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[112965602663600] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[112965602663600] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[112965602663600] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 1420936
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967614672110977] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967614672110977] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967614672110977] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_T2_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 1185444
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[169916867657904] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[169916867657904] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[169916867657904] = P_mtx0_3_I16_flt_Nrm_0_0_I8_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 809722
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120677226779999] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120677226779999] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120677226779999] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 583512
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303413375677] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303413375677] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303413375677] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 574064
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22202711468046] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22202711468046] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[22202711468046] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_<0>;
	}
	// num_verts= 538564
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113145967592125] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113145967592125] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113145967592125] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 263756
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114904208228608] = P_mtx1_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114904208228608] = P_mtx1_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[114904208228608] = P_mtx1_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 217600
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825113823246] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825113823246] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825113823246] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 171308
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825113860784] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825113860784] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825113860784] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 116188
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114746390931470] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114746390931470] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114746390931470] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 108192
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
	// num_verts= 97695
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[27976101533023] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[27976101533023] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[27976101533023] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0>;
	}
	// num_verts= 93096
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489984282274765] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489984282274765] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489984282274765] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 73472
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518031988413] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518031988413] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518031988413] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 73372
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113145967517049] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113145967517049] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113145967517049] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 67912
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118919338418877] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118919338418877] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118919338418877] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 65534
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114588945147918] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114588945147918] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114588945147918] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 63352
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482533456528198] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482533456528198] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[482533456528198] = P_mtx0_2_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 37216
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518032007182] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518032007182] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518032007182] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 35440
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114588906709006] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114588906709006] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114588906709006] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 32920
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113145967554587] = P_mtx0_2_I8_u16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113145967554587] = P_mtx0_2_I8_u16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113145967554587] = P_mtx0_2_I8_u16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 31396
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113460859196767] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113460859196767] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[113460859196767] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 31104
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518032025951] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518032025951] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518032025951] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 28446
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120360586204861] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120360586204861] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120360586204861] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 26856
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825075384334] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825075384334] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825075384334] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 19880
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114667629619551] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114667629619551] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114667629619551] = P_mtx0_2_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 19252
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598484649998] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598484649998] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598484649998] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 11660
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120360586223630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120360586223630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120360586223630] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 9216
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114588945185456] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114588945185456] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114588945185456] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 8848
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114746390969008] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114746390969008] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114746390969008] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 7350
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[27976101551792] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[27976101551792] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[27976101551792] = P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_8888_<0>;
	}
	// num_verts= 6672
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114746352492558] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114746352492558] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114746352492558] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 6480
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114667629582013] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114667629582013] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114667629582013] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 5796
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114588906690237] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114588906690237] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114588906690237] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 5506
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113460859215536] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113460859215536] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[113460859215536] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 5290
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[27976082294798] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[27976082294798] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[27976082294798] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_<0>;
	}
	// num_verts= 5208
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120519761758222] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120519761758222] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120519761758222] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 5120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114667668039694] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114667668039694] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114667668039694] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 5120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114667668077232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114667668077232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114667668077232] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 5082
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120596793337870] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120596793337870] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120596793337870] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 4976
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114588906633930] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114588906633930] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114588906633930] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 4320
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518070446094] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518070446094] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518070446094] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 3988
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120596793375408] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120596793375408] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120596793375408] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 3560
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[484291344890416] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[484291344890416] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[484291344890416] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 3360
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303413300601] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303413300601] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303413300601] = P_mtx0_2_I8_s8_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 2932
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825075365565] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825075365565] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825075365565] = P_mtx0_2_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2376
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120439309115406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120439309115406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[120439309115406] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 2172
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[26533104739005] = P_mtx0_2_I8_s16_C0_1_I8_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[26533104739005] = P_mtx0_2_I8_s16_C0_1_I8_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[26533104739005] = P_mtx0_2_I8_s16_C0_1_I8_8888_<0>;
	}
	// num_verts= 2040
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
	// num_verts= 1836
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[22202673029134] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[22202673029134] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[22202673029134] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_<0>;
	}
	// num_verts= 1638
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[27974410220895] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[27974410220895] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_<0x301>;
	}
	else
#endif
	{
	pvlmap[27974410220895] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_<0>;
	}
	// num_verts= 1216
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20759733949791] = P_mtx0_2_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20759733949791] = P_mtx0_2_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20759733949791] = P_mtx0_2_I8_flt_<0>;
	}
	// num_verts= 994
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[27975794021727] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_6666_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[27975794021727] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_6666_<0x301>;
	}
	else
#endif
	{
	pvlmap[27975794021727] = P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_C0_1_I8_6666_<0>;
	}
	// num_verts= 800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518070483632] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518070483632] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518070483632] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 544
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
	// num_verts= 340
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482848348133936] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482848348133936] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[482848348133936] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
}
