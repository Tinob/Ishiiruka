#include "VideoCommon/G_RMGP01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while (loopcount)
	{
		pipelinestate.tcIndex = 0;
		pipelinestate.colIndex = 0;
		pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0;
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
#if _M_SSE >= 0x401
		if (iSSE >= 0x401)
		{
			_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
		}
		else
#endif
		{
			_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
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
		pipelinestate.Write<u32>(0);
		--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while (loopcount)
	{
		pipelinestate.tcIndex = 0;
		pipelinestate.colIndex = 0;
		pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0;
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
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
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
#if _M_SSE >= 0x401
		if (iSSE >= 0x401)
		{
			_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);
		}
		else
#endif
		{
			_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);
		}
		Color_ReadIndex_32b_8888<u16>(pipelinestate);
		pipelinestate.Write<u32>(0);
		--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while (loopcount)
	{
		pipelinestate.tcIndex = 0;
		pipelinestate.colIndex = 0;
		pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0;
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
		if (iSSE >= 0x401)
		{
			_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
		}
		else
#endif
		{
			_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
		}
		if (g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
		Color_ReadIndex_32b_8888<u16>(pipelinestate);
		pipelinestate.Write<u32>(0);
		--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_s8_T0_mtx0_1_I8_s8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
	_Pos_ReadIndex<u8, s8, 3>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_flt_T0_mtx0_1_I8_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T2_mtx1_1_Inv_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
	pipelinestate.Write<u8>(pipelinestate.curposmtx);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	pipelinestate.Write<u8>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u8_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	_TexCoord_ReadDirect<u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	pipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;
	pipelinestate.texmtxread++;
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
#if _M_SSE >= 0x401
	if (iSSE >= 0x401)
	{
		_Pos_ReadIndex_16x3_SSE4<u16, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_2_Dir_u16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_flt_C0_1_Dir_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	_Color_ReadDirect_32b_8888(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_2_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
		_Pos_ReadIndex<u16, s16, 2>(pipelinestate);
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
void P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	_Color_ReadDirect_32b_8888(pipelinestate);
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
void P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
void P_mtx0_3_Dir_s16_C0_1_Dir_8888_(VertexLoader *loader)
{
	TPipelineState &pipelinestate = g_PipelineState;
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_RMGP01_pvt::Initialize(std::map<size_t, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 796831388
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559585448462] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559585448462] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559585448462] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 106483526
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23104536392206] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23104536392206] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[23104536392206] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_<0>;
	}
	// num_verts= 63346826
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559937723823] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559937723823] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559937723823] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 57526589
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900374864398] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900374864398] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900374864398] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 37320704
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113224690427594] = P_mtx0_3_I8_s8_T0_mtx0_1_I8_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113224690427594] = P_mtx0_3_I8_s8_T0_mtx0_1_I8_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[113224690427594] = P_mtx0_3_I8_s8_T0_mtx0_1_I8_s8_<0>;
	}
	// num_verts= 14366472
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23104888667567] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23104888667567] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[23104888667567] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_<0>;
	}
	// num_verts= 13430240
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[21300905800880] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[21300905800880] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[21300905800880] = P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_<0>;
	}
	// num_verts= 13430240
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251800802582913] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251800802582913] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[251800802582913] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 11640279
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559585486000] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559585486000] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559585486000] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 8214464
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168395148009648] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168395148009648] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[168395148009648] = P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 7992765
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900374901936] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900374901936] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900374901936] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 7142254
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170561346826500] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170561346826500] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170561346826500] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 6350696
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31763746976270] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31763746976270] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[31763746976270] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0>;
	}
	// num_verts= 4825089
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[168395147972110] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[168395147972110] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[168395147972110] = P_mtx0_3_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 4721496
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368983728] = P_mtx0_3_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368983728] = P_mtx0_3_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368983728] = P_mtx0_3_Dir_flt_<0>;
	}
	// num_verts= 4503973
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67107058113712] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67107058113712] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67107058113712] = P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 2488348
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31764099251631] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31764099251631] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[31764099251631] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_<0>;
	}
	// num_verts= 2251088
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69837143399600] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69837143399600] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[69837143399600] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_<0>;
	}
	// num_verts= 2025448
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900727139759] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900727139759] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900727139759] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1832400
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[11798047686755201] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[11798047686755201] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[11798047686755201] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_T3_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 1792800
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170560642274545] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170560642274545] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170560642274545] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_<0>;
	}
	// num_verts= 1791395
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170564165660684] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170564165660684] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170564165660684] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 1337064
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67107058094943] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67107058094943] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67107058094943] = P_mtx0_2_Dir_flt_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 1323264
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[113280494230704] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[113280494230704] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[113280494230704] = P_mtx0_3_Dir_flt_T0_mtx0_1_I8_flt_<0>;
	}
	// num_verts= 1315600
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170562051377222] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170562051377222] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170562051377222] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx1_1_I16_s16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 1079028
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23466900053168] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23466900053168] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23466900053168] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_<0>;
	}
	// num_verts= 606828
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69994589164383] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69994589164383] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[69994589164383] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 410040
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170562756558007] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T2_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170562756558007] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T2_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170562756558007] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T2_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 11365840
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		pvlmap[724640818890863] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724640818890863] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
		pvlmap[724640818890863] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1326988
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		pvlmap[31763747013808] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31763747013808] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
		pvlmap[31763747013808] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_C0_1_I16_8888_<0>;
	}
	// num_verts= 441040
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		pvlmap[29599309499918] = P_mtx0_3_I16_s16_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[29599309499918] = P_mtx0_3_I16_s16_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
		pvlmap[29599309499918] = P_mtx0_3_I16_s16_C0_1_I16_8888_<0>;
	}
	// num_verts= 214770
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69994589183152] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69994589183152] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[69994589183152] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 170968
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23104536429744] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23104536429744] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[23104536429744] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_<0>;
	}
	// num_verts= 113136
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66792166452763] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66792166452763] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66792166452763] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_u8_<0>;
	}
	// num_verts= 50580
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67107058019867] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67107058019867] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67107058019867] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 20736
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161902136242436] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161902136242436] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161902136242436] = P_mtx1_3_I16_s16_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 19124
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368889883] = P_mtx0_2_Dir_u16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368889883] = P_mtx0_2_Dir_u16_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368889883] = P_mtx0_2_Dir_u16_<0>;
	}
	// num_verts= 7864
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23466900034399] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23466900034399] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23466900034399] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_<0>;
	}
	// num_verts= 2380
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368964959] = P_mtx0_2_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368964959] = P_mtx0_2_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368964959] = P_mtx0_2_Dir_flt_<0>;
	}
	// num_verts= 1852
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724640818853325] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724640818853325] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[724640818853325] = P_mtx0_3_I16_s16_Nrm_0_0_I16_s16_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 1536
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69915866272607] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69915866272607] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[69915866272607] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 1040
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[69915866291376] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[69915866291376] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[69915866291376] = P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_<0>;
	}
	// num_verts= 588
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[251800802489068] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[251800802489068] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[251800802489068] = P_mtx0_2_Dir_u16_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 16
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[254688333633584] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[254688333633584] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[254688333633584] = P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 0
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[23466900015630] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[23466900015630] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[23466900015630] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_<0>;
	}
}
