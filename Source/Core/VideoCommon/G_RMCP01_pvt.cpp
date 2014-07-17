#include "VideoCommon/G_RMCP01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_u16_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
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
		_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);
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
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_RMCP01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 114564372
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[165588031144112] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[165588031144112] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[165588031144112] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 41184252
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119257149243568] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119257149243568] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[119257149243568] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 38191608
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119414595027120] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119414595027120] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[119414595027120] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 27497124
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167598193946457] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_u16_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167598193946457] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_u16_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[167598193946457] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_u16_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 23903724
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[719669264549249] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[719669264549249] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[719669264549249] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 15081906
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120700196846502] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120700196846502] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[120700196846502] = P_mtx1_3_I16_flt_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_u16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 14195346
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[167752820895825] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[167752820895825] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[167752820895825] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 11340420
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121579032503472] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121579032503472] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121579032503472] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 10002564
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[488621718979457] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[488621718979457] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[488621718979457] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_T1_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 5261100
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[121504889823918] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[121504889823918] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[121504889823918] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 821376
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[488306827412079] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[488306827412079] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[488306827412079] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 300636
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120518032044720] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120518032044720] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120518032044720] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_0_I8_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 110376
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120777158322702] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120777158322702] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120777158322702] = P_mtx0_3_I16_s16_Nrm_0_0_I8_s16_C0_0_I8_565_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 109178
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114926755953840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114926755953840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114926755953840] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 56308
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[118919338475184] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[118919338475184] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[118919338475184] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0>;
	}
}
