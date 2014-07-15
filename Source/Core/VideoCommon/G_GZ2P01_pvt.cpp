#include "VideoCommon/G_GZ2P01_pvt.h"
#include "VideoCommon/VertexLoader_ColorFuncs.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"
#include "VideoCommon/VertexLoader_PositionFuncs.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/VideoConfig.h"

template <int iSSE>
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_(VertexLoader *loader)
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
void P_mtx0_2_I16_flt_T0_mtx0_1_Dir_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 2>(pipelinestate);
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
void P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_2_I16_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u16, false>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u16, float, 2>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I8_flt_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_C0_1_I16_8888_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_flt_(VertexLoader *loader)
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
	_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);
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
void P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_2_Dir_s8_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
{
	TPipelineState pipelinestate = g_PipelineState;
	u32 loopcount = pipelinestate.count;
	while(loopcount)
	{
	pipelinestate.tcIndex = 0;
	pipelinestate.colIndex = 0;
	pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0; 
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBoxPrepare();
	_Pos_ReadDirect<s8, 2>(pipelinestate);
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_(VertexLoader *loader)
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
void P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
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
		_Pos_ReadIndex_Float_SSSE3<u8, true>(pipelinestate);
	}
	else
#endif
	{
		_Pos_ReadIndex<u8, float, 3>(pipelinestate);
	}
	if(g_ActiveConfig.bUseBBox) VertexLoader_BBox::UpdateBoundingBox();
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_(VertexLoader *loader)
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
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}
template <int iSSE>
void P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_(VertexLoader *loader)
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
	_TexCoord_ReadDirect<s8, 2>(pipelinestate);
	pipelinestate.Write<u32>(0);
	--loopcount;
	}
}

void G_GZ2P01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// num_verts= 35048130
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161902136279974] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161902136279974] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161902136279974] = P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_<0>;
	}
	// num_verts= 34333480
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[67467788064607] = P_mtx0_2_I16_flt_T0_mtx0_1_Dir_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[67467788064607] = P_mtx0_2_I16_flt_T0_mtx0_1_Dir_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[67467788064607] = P_mtx0_2_I16_flt_T0_mtx0_1_Dir_flt_<0>;
	}
	// num_verts= 32366998
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
	// num_verts= 15559468
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
	// num_verts= 2425534
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20940098934623] = P_mtx0_2_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20940098934623] = P_mtx0_2_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[20940098934623] = P_mtx0_2_I16_flt_<0>;
	}
	// num_verts= 1008500
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
	// num_verts= 286594
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
	// num_verts= 155572
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[161979059354800] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_flt_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[161979059354800] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_flt_<0x301>;
	}
	else
#endif
	{
	pvlmap[161979059354800] = P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_flt_<0>;
	}
	// num_verts= 108604
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
	// num_verts= 108060
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[66870889307001] = P_mtx0_2_Dir_s8_T0_mtx0_1_Dir_s8_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[66870889307001] = P_mtx0_2_Dir_s8_T0_mtx0_1_Dir_s8_<0x301>;
	}
	else
#endif
	{
	pvlmap[66870889307001] = P_mtx0_2_Dir_s8_T0_mtx0_1_Dir_s8_<0>;
	}
	// num_verts= 96084
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[159735937388046] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_<0x401>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[159735937388046] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_<0x301>;
	}
	else
#endif
	{
	pvlmap[159735937388046] = P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_<0>;
	}
	// num_verts= 19036
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
	// num_verts= 6216
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
	// num_verts= 824
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
}
