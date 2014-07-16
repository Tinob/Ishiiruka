#include "VideoCommon/G_SMNP01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
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
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
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
void P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_T3_mtx1_1_Inv_flt_(VertexLoader *loader)
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
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
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
void P_mtx0_2_I16_flt_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_2_I16_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
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
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
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
void P_mtx0_3_I8_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_2_I8_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_2_I8_u8_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u8, u8, 2>(pipelinestate);
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
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
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
	pipelinestate.Write(0.f);
	pipelinestate.Write(0.f); 
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); 
	pipelinestate.Write(0.f); 
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
void P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
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

void G_SMNP01_pvt::Initialize(std::map<size_t, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 105404
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
	// num_verts= 65130
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167678325903372] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167678325903372] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[167678325903372] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 43992
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[166952285751822] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[166952285751822] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[166952285751822] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 42588
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167684285532700] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_T3_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167684285532700] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_T3_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[167684285532700] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_T3_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 35776
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113562501289823] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113562501289823] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113562501289823] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 29926
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
	// num_verts= 29120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113483778398047] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113483778398047] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113483778398047] = P_mtx0_2_I16_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 28704
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167673745691150] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167673745691150] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[167673745691150] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 26000
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
	// num_verts= 24336
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121504889786380] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121504889786380] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121504889786380] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 16224
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303413394446] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303413394446] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303413394446] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 16224
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
	// num_verts= 9568
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303413413215] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303413413215] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303413413215] = P_mtx0_2_I8_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 4680
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67287422929623] = P_mtx0_2_I8_u8_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67287422929623] = P_mtx0_2_I8_u8_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67287422929623] = P_mtx0_2_I8_u8_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 3432
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120603064862220] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120603064862220] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120603064862220] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 2496
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119076784221198] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119076784221198] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119076784221198] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 2080
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[488542996049869] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[488542996049869] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[488542996049869] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1872
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118919338437646] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118919338437646] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118919338437646] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 1248
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119155507112974] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119155507112974] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119155507112974] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 416
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119155507094205] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119155507094205] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119155507094205] = P_mtx0_2_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 312
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118998061329422] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118998061329422] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118998061329422] = P_mtx0_3_I8_s16_C0_1_I8_8888_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 208
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
}
