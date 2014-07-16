#include "VideoCommon/G_GKYE01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I8_flt_C0_0_Dir_565_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_16b_565(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_C0_1_Dir_8888_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_Dir_8888_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I16_s16_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx1_1_Inv_s8_(VertexLoader *loader)
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
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
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
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I8_s16_C0_1_Dir_8888_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Pos_ReadIndex<u16, s8, 3>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx1_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I8_s8_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I16_s16_C0_0_Dir_565_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_16b_565(pipelinestate);
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
void P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Pos_ReadIndex<u16, s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I16_s16_C0_1_Dir_6666_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Color_ReadDirect_24b_6666(pipelinestate);
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
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_24b_6666(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I8_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_flt_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx1_3_I16_s16_C0_0_Dir_565_(VertexLoader *loader)
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
	_Color_ReadDirect_16b_565(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx1_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Pos_ReadIndex<u8, s8, 3>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_I8_s16_C0_1_Dir_4444_(VertexLoader *loader)
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
	_Color_ReadDirect_16b_4444(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx1_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_24b_6666(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_C0_1_Dir_6666_(VertexLoader *loader)
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
	_Color_ReadDirect_24b_6666(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s8_C0_1_Dir_8888_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u16, s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	_Color_ReadDirect_32b_8888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare(pipelinestate);
	_Pos_ReadIndex<u16, s8, 3>(pipelinestate);
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
void P_mtx0_3_I8_s16_C0_0_Dir_565_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_16b_565(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_T0_mtx0_1_I8_s8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_6666_(VertexLoader *loader)
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
	_Color_ReadDirect_24b_6666(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx1_3_I8_s16_T0_mtx0_1_I8_s16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_(VertexLoader *loader)
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
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_GKYE01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 141489092
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162702191386800] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162702191386800] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[162702191386800] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 8228703
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[715981922105710] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[715981922105710] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[715981922105710] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 4640011
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20759733931022] = P_mtx0_3_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20759733931022] = P_mtx0_3_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[20759733931022] = P_mtx0_3_I8_s16_<0>;
	}
	// num_verts= 3468050
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115648568130991] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115648568130991] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[115648568130991] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 3411473
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900688700847] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900688700847] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900688700847] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1771142
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116031807405744] = P_mtx0_3_I8_flt_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116031807405744] = P_mtx0_3_I8_flt_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116031807405744] = P_mtx0_3_I8_flt_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 1132629
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[713817170792909] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[713817170792909] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[713817170792909] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1071890
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23459846231281] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23459846231281] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[23459846231281] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 1009505
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23827630022832] = P_mtx0_3_I16_flt_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23827630022832] = P_mtx0_3_I16_flt_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23827630022832] = P_mtx0_3_I16_flt_C0_1_Dir_8888_<0>;
	}
	// num_verts= 877740
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113382136323760] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113382136323760] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113382136323760] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 684544
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23647265038000] = P_mtx0_3_I8_flt_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23647265038000] = P_mtx0_3_I8_flt_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23647265038000] = P_mtx0_3_I8_flt_C0_1_Dir_8888_<0>;
	}
	// num_verts= 668648
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116190944463886] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116190944463886] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116190944463886] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 663148
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113562501271054] = P_mtx0_3_I16_s16_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113562501271054] = P_mtx0_3_I16_s16_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113562501271054] = P_mtx0_3_I16_s16_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 645772
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
	// num_verts= 602804
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115726900308494] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115726900308494] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[115726900308494] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 555434
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113145967648432] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113145967648432] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113145967648432] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 547840
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825427659695] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825427659695] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825427659695] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 526034
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
	// num_verts= 427342
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113145967610894] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113145967610894] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113145967610894] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 419687
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113483778416816] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113483778416816] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113483778416816] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 390274
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
	// num_verts= 362564
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23223639117041] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx1_1_Inv_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23223639117041] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx1_1_Inv_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[23223639117041] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx1_1_Inv_s8_<0>;
	}
	// num_verts= 359556
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115569806800303] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115569806800303] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[115569806800303] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 234208
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
	// num_verts= 216972
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[117634235837359] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[117634235837359] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[117634235837359] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 203988
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115853133733040] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115853133733040] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[115853133733040] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 173464
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162623468457486] = P_mtx0_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162623468457486] = P_mtx0_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[162623468457486] = P_mtx0_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 167760
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23647265000462] = P_mtx0_3_I8_s16_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23647265000462] = P_mtx0_3_I8_s16_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23647265000462] = P_mtx0_3_I8_s16_C0_1_Dir_8888_<0>;
	}
	// num_verts= 149920
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114927069677675] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114927069677675] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114927069677675] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 139879
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114667981876143] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114667981876143] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114667981876143] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 134034
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[117712958729135] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[117712958729135] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[117712958729135] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 130326
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
	// num_verts= 108700
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[162623820732847] = P_mtx1_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[162623820732847] = P_mtx1_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[162623820732847] = P_mtx1_3_I16_s16_C0_1_Dir_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 106288
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
	// num_verts= 92682
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[115727252583855] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[115727252583855] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[115727252583855] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 85844
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116369618136590] = P_mtx0_3_I16_s16_C0_0_Dir_565_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116369618136590] = P_mtx0_3_I16_s16_C0_0_Dir_565_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116369618136590] = P_mtx0_3_I16_s16_C0_0_Dir_565_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 82422
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[117814600747115] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[117814600747115] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[117814600747115] = P_mtx1_3_I16_s8_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 79488
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116449724829198] = P_mtx0_3_I16_s16_C0_1_Dir_6666_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116449724829198] = P_mtx0_3_I16_s16_C0_1_Dir_6666_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116449724829198] = P_mtx0_3_I16_s16_C0_1_Dir_6666_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 73914
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303413431984] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303413431984] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303413431984] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 72816
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116033498680334] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116033498680334] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116033498680334] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 66003
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116033191169038] = P_mtx0_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116033191169038] = P_mtx0_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116033191169038] = P_mtx0_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 63280
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113303765707345] = P_mtx1_3_I8_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113303765707345] = P_mtx1_3_I8_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113303765707345] = P_mtx1_3_I8_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 58184
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114589258984367] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114589258984367] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[114589258984367] = P_mtx1_3_I8_s16_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 55800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113484130692177] = P_mtx1_3_I16_flt_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113484130692177] = P_mtx1_3_I16_flt_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113484130692177] = P_mtx1_3_I16_flt_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 38480
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23826290948527] = P_mtx1_3_I16_s16_C0_0_Dir_565_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23826290948527] = P_mtx1_3_I16_s16_C0_0_Dir_565_<0x301>;
	}
	else
#endif
	{
	pvlmap[23826290948527] = P_mtx1_3_I16_s16_C0_0_Dir_565_<0>;
	}
	// num_verts= 26932
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116371309486256] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116371309486256] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116371309486256] = P_mtx0_3_I16_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 25602
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825427584619] = P_mtx1_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825427584619] = P_mtx1_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825427584619] = P_mtx1_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 24585
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[485578433400507] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[485578433400507] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[485578433400507] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 22088
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116269667355662] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116269667355662] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116269667355662] = P_mtx0_3_I8_s16_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 21449
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116190944501424] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116190944501424] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[116190944501424] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 20836
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
	// num_verts= 20416
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
	// num_verts= 19304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23646649977870] = P_mtx0_3_I8_s16_C0_1_Dir_4444_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23646649977870] = P_mtx0_3_I8_s16_C0_1_Dir_4444_<0x301>;
	}
	else
#endif
	{
	pvlmap[23646649977870] = P_mtx0_3_I8_s16_C0_1_Dir_4444_<0>;
	}
	// num_verts= 18790
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114825075309258] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114825075309258] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114825075309258] = P_mtx0_3_I8_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 13254
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116033543444399] = P_mtx1_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116033543444399] = P_mtx1_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116033543444399] = P_mtx1_3_I8_s16_C0_1_Dir_6666_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 13104
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113562501308592] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113562501308592] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113562501308592] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 12906
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23646957489166] = P_mtx0_3_I8_s16_C0_1_Dir_6666_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23646957489166] = P_mtx0_3_I8_s16_C0_1_Dir_6666_<0x301>;
	}
	else
#endif
	{
	pvlmap[23646957489166] = P_mtx0_3_I8_s16_C0_1_Dir_6666_<0>;
	}
	// num_verts= 11628
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23827629910218] = P_mtx0_3_I16_s8_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23827629910218] = P_mtx0_3_I16_s8_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23827629910218] = P_mtx0_3_I16_s8_C0_1_Dir_8888_<0>;
	}
	// num_verts= 8878
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[114926717402314] = P_mtx0_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[114926717402314] = P_mtx0_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[114926717402314] = P_mtx0_3_I16_s8_Nrm_0_0_I8_s8_T0_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 8534
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
	// num_verts= 7980
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116031807368206] = P_mtx0_3_I8_s16_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116031807368206] = P_mtx0_3_I8_s16_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116031807368206] = P_mtx0_3_I8_s16_C0_0_Dir_565_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 7792
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113224690502670] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113224690502670] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113224690502670] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 7095
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[25089896587278] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_6666_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[25089896587278] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_6666_<0x301>;
	}
	else
#endif
	{
	pvlmap[25089896587278] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_6666_<0>;
	}
	// num_verts= 5304
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[116033498717872] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[116033498717872] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[116033498717872] = P_mtx0_3_I8_flt_C0_1_Dir_8888_T0_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 5229
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482690902368605] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482690902368605] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[482690902368605] = P_mtx0_3_I8_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 3514
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[117712606453774] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[117712606453774] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[117712606453774] = P_mtx0_3_I8_s16_Nrm_0_0_I8_s8_C0_1_Dir_8888_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 2615
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113382488561583] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113382488561583] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[113382488561583] = P_mtx1_3_I8_s16_T0_mtx0_1_I8_s16_<0>;
	}
	// num_verts= 1841
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
	// num_verts= 1112
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482871267353437] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482871267353437] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[482871267353437] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u16_T1_mtx0_1_I8_u16_<0>;
	}
	// num_verts= 1080
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482713821569337] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482713821569337] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[482713821569337] = P_mtx0_3_I16_flt_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0>;
	}
	// num_verts= 684
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[482533456546967] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[482533456546967] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[482533456546967] = P_mtx0_3_I8_s16_T0_mtx0_1_I8_u8_T1_mtx0_1_I8_u8_<0>;
	}
}
