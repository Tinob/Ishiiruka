#include "VideoCommon/G_GZLE01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_I8_8888_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I8_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);
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
void P_mtx0_3_Dir_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadDirect<s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadDirect<s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_u8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadDirect<s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_TexCoord_ReadDirect<u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_GZLE01_pvt::Initialize(std::map<size_t, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 371602133
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
	// num_verts= 254628094
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
	// num_verts= 96809091
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
	// num_verts= 78298861
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900727177297] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900727177297] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900727177297] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 19209264
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67028335221936] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67028335221936] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[67028335221936] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 15125136
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20759733968560] = P_mtx0_3_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20759733968560] = P_mtx0_3_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20759733968560] = P_mtx0_3_I8_flt_<0>;
	}
	// num_verts= 13419332
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168395500285009] = P_mtx1_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168395500285009] = P_mtx1_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[168395500285009] = P_mtx1_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 9402870
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
	// num_verts= 6855306
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23104888705105] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23104888705105] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[23104888705105] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_<0>;
	}
	// num_verts= 4085414
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[159735937425584] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[159735937425584] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[159735937425584] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 2975520
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[159814660317360] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[159814660317360] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[159814660317360] = P_mtx0_3_I16_flt_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 1983680
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[29599309537456] = P_mtx0_3_I16_flt_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[29599309537456] = P_mtx0_3_I16_flt_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[29599309537456] = P_mtx0_3_I16_flt_C0_1_I16_8888_<0>;
	}
	// num_verts= 1806564
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
	// num_verts= 1462560
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69632244779184] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69632244779184] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[69632244779184] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 1118096
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67107058076174] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67107058076174] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67107058076174] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 440028
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251722079653325] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251722079653325] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[251722079653325] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 400120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67051254423216] = P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67051254423216] = P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[67051254423216] = P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_<0>;
	}
	// num_verts= 374000
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[72519775848624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[72519775848624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[72519775848624] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 212982
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368946190] = P_mtx0_3_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368946190] = P_mtx0_3_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368946190] = P_mtx0_3_Dir_s16_<0>;
	}
	// num_verts= 146464
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161257599415472] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161257599415472] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161257599415472] = P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 133340
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368871114] = P_mtx0_3_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368871114] = P_mtx0_3_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368871114] = P_mtx0_3_Dir_s8_<0>;
	}
	// num_verts= 124476
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66870889400846] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66870889400846] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66870889400846] = P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_<0>;
	}
	// num_verts= 11556
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
	// num_verts= 9860
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66870889325770] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66870889325770] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66870889325770] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_s8_<0>;
	}
	// num_verts= 8320
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67467788083376] = P_mtx0_3_I16_flt_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67467788083376] = P_mtx0_3_I16_flt_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67467788083376] = P_mtx0_3_I16_flt_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66792166433994] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66792166433994] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66792166433994] = P_mtx0_3_Dir_s8_T0_mtx0_1_Dir_u8_<0>;
	}
}
