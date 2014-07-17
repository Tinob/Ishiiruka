#include "VideoCommon/G_SEME4Q_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_(VertexLoader *loader)
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
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_2_Dir_s16_(VertexLoader *loader)
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
		_Pos_ReadDirect_16x2_SSE4<true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadDirect<s16, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_(VertexLoader *loader)
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
void P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_u8_(VertexLoader *loader)
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
		_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);
	}
	else
#endif
	{
		_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);
	}
	Color_ReadIndex_32b_8888<u16>(pipelinestate);
	_TexCoord_ReadIndex<u16, u8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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

void G_SEME4Q_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 508135427
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170559547047088] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170559547047088] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[170559547047088] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 438063420
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170561366083494] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170561366083494] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[170561366083494] = P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 1731345
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[31763708574896] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[31763708574896] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_<0x301>;
	}
	else
#endif
	{
	pvlmap[31763708574896] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_<0>;
	}
	// num_verts= 1672860
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[724640780451951] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[724640780451951] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[724640780451951] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_C0_1_I16_8888_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 820146
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20579368927421] = P_mtx0_2_Dir_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20579368927421] = P_mtx0_2_Dir_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[20579368927421] = P_mtx0_2_Dir_s16_<0>;
	}
	// num_verts= 589120
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[254688333614815] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[254688333614815] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[254688333614815] = P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 34986
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[170323436030128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_u8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[170323436030128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_u8_<0x301>;
	}
	else
#endif
	{
	pvlmap[170323436030128] = P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_u8_<0>;
	}
	// num_verts= 108
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161900336463024] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161900336463024] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[161900336463024] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_<0>;
	}
}
