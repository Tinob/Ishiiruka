#include "VideoCommon/G_GLME01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_C0_1_I8_8888_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_s16_(VertexLoader *loader)
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
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_1_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
		_Normal_Index_S16_SSE4<u16, 3>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, s16, 3, 0>(pipelinestate);
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_(VertexLoader *loader)
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
void P_mtx1_3_Dir_flt_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
		_Pos_ReadDirect_Float_SSSE3<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx1_3_I8_s16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
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
#if _M_SSE >= 0x301
	if(iSSE >= 0x301)
	{
		_Normal_Direct_FLOAT_SSSE3<1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Direct<float, 1>(pipelinestate);
	}
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_TexCoord_ReadDirect_16x2_SSE4<false>(pipelinestate);
	}
	else
#endif
	{
		_TexCoord_ReadDirect<u16, 2>(pipelinestate);
	}
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_Dir_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_(VertexLoader *loader)
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

void G_GLME01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 149455967
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161979116975630] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161979116975630] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161979116975630] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 32168054
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[26713822055505] = P_mtx1_3_I16_flt_C0_1_I8_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[26713822055505] = P_mtx1_3_I16_flt_C0_1_I8_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[26713822055505] = P_mtx1_3_I16_flt_C0_1_I8_8888_<0>;
	}
	// num_verts= 22892070
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161981582941928] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161981582941928] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161981582941928] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 15738604
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162114818672081] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162114818672081] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[162114818672081] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 5723308
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
	// num_verts= 3583044
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161981582941654] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161981582941654] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161981582941654] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_s16_<0>;
	}
	// num_verts= 2162366
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162114801862097] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162114801862097] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[162114801862097] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_<0>;
	}
	// num_verts= 1516152
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23104555611662] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23104555611662] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[23104555611662] = P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_<0>;
	}
	// num_verts= 1425600
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[715981617916527] = P_mtx0_3_I16_flt_Nrm_1_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[715981617916527] = P_mtx0_3_I16_flt_Nrm_1_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[715981617916527] = P_mtx0_3_I16_flt_Nrm_1_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1014800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162114743027153] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162114743027153] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[162114743027153] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0>;
	}
	// num_verts= 887488
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[112965954938961] = P_mtx1_3_Dir_flt_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[112965954938961] = P_mtx1_3_Dir_flt_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[112965954938961] = P_mtx1_3_Dir_flt_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 361339
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[159814660279822] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[159814660279822] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[159814660279822] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 154860
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113146319886255] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113146319886255] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113146319886255] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 99384
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[70716126000304] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[70716126000304] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[70716126000304] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 45384
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69837143380831] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69837143380831] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[69837143380831] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 29696
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[713817170830447] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[713817170830447] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[713817170830447] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 16152
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168113140894222] = P_mtx0_3_Dir_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168113140894222] = P_mtx0_3_Dir_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[168113140894222] = P_mtx0_3_Dir_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[159950286331345] = P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[159950286331345] = P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[159950286331345] = P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_<0>;
	}
}
