#include "VideoCommon/G_SLSEXJ_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_(VertexLoader *loader)
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

void G_SLSEXJ_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 2838176
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[719590541657199] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[719590541657199] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[719590541657199] = P_mtx0_3_I16_flt_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1380980
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[488542996087407] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[488542996087407] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[488542996087407] = P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1005288
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482769625223117] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482769625223117] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[482769625223117] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 664560
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[667643734713455] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[667643734713455] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[667643734713455] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 459660
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[120598446211086] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[120598446211086] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[120598446211086] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 408747
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[2707212694216303] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[2707212694216303] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[2707212694216303] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 328659
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[721754940694639] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[721754940694639] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[721754940694639] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I8_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 257304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[674860044638319] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[674860044638319] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[674860044638319] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 159324
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[489985935185519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[489985935185519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[489985935185519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 99258
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[674679679653487] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[674679679653487] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[674679679653487] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 64032
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482769625260655] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482769625260655] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[482769625260655] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 44730
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1967535891420271] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1967535891420271] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1967535891420271] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_C0_1_I8_8888_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 35784
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[1961762520593519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[1961762520593519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[1961762520593519] = P_mtx0_3_I8_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_T1_mtx0_1_I8_s16_T2_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 32568
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
}
