#include "VideoCommon/G_GMSE01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
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
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_T1_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadDirect_16x2_SSE4<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadDirect_16x2_SSE4<false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<u16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	Color_ReadIndex_32b_8888<u8>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_0_Dir_888_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_Color_ReadDirect_24b_888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadDirect_16x2_SSE4<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
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
void P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_Color_ReadDirect_32b_8888(pipelinestate);
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_GMSE01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 577623607
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168473870901424] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168473870901424] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[168473870901424] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 154511880
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20940098953392] = P_mtx0_3_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20940098953392] = P_mtx0_3_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20940098953392] = P_mtx0_3_I16_flt_<0>;
	}
	// num_verts= 116112156
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170638308340238] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170638308340238] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170638308340238] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 71055456
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168473870863886] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168473870863886] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[168473870863886] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 49024855
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2932385265408223] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2932385265408223] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[2932385265408223] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 35805649
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[722555104306561] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[722555104306561] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[722555104306561] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 34237744
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20940098915854] = P_mtx0_3_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20940098915854] = P_mtx0_3_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[20940098915854] = P_mtx0_3_I16_s16_<0>;
	}
	// num_verts= 31459506
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170638308377776] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170638308377776] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170638308377776] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 30277195
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2932385617683584] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2932385617683584] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[2932385617683584] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 20865728
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[722555104269023] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[722555104269023] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[722555104269023] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 16973968
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559956980817] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559956980817] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559956980817] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 11037728
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[254688333652353] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[254688333652353] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[254688333652353] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 10476336
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900746396753] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900746396753] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900746396753] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 8871715
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724641190385680] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724641190385680] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[724641190385680] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 4883794
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559604705456] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559604705456] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559604705456] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 4393575
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724719541782913] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724719541782913] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[724719541782913] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 2639429
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31764118508625] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31764118508625] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[31764118508625] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0>;
	}
	// num_verts= 2613328
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[715981960544622] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[715981960544622] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[715981960544622] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 2110888
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161979097756174] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161979097756174] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161979097756174] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 1399102
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170638660615599] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170638660615599] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170638660615599] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 1000352
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69994589145614] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69994589145614] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[69994589145614] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 902888
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[715981608269261] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[715981608269261] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[715981608269261] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 591570
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161979097793712] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161979097793712] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161979097793712] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 501900
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900394121392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900394121392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900394121392] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 362004
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113326332633264] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113326332633264] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113326332633264] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 329652
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251643356742780] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_T1_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251643356742780] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_T1_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[251643356742780] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_T1_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 326560
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[722476381414511] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[722476381414511] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[722476381414511] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 314766
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31763766233264] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31763766233264] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[31763766233264] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_<0>;
	}
	// num_verts= 270732
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69837143362062] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69837143362062] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[69837143362062] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 184690
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2941044476029825] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2941044476029825] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[2941044476029825] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 179086
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[716060331161311] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[716060331161311] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[716060331161311] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 177974
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2938880038553473] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2938880038553473] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[2938880038553473] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 174372
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66949612236315] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66949612236315] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[66949612236315] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 95320
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66870889419615] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66870889419615] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66870889419615] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_s8_<0>;
	}
	// num_verts= 29264
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[119099703460016] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[119099703460016] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[119099703460016] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 7116
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161979469288529] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161979469288529] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161979469288529] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 4692
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23465516252336] = P_mtx0_3_Dir_flt_C0_0_Dir_888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23465516252336] = P_mtx0_3_Dir_flt_C0_0_Dir_888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23465516252336] = P_mtx0_3_Dir_flt_C0_0_Dir_888_<0>;
	}
	// num_verts= 800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66949612273853] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66949612273853] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[66949612273853] = P_mtx0_2_Dir_s16_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 464
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66949612292622] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66949612292622] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[66949612292622] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 404
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69758420470286] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69758420470286] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[69758420470286] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s8_<0>;
	}
}
