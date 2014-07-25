#include "VideoCommon/G_GZ2P01_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_GZ2P01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
	// P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx1_1_Inv_flt_
// num_verts= 79262418
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[21237928937659] = TemplatedLoader<0x401, 0x00030f02u, 0x40e00c09u, 0x80000009u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[21237928937659] = TemplatedLoader<0x301, 0x00030f02u, 0x40e00c09u, 0x80000009u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[21237928937659] = TemplatedLoader<0, 0x00030f02u, 0x40e00c09u, 0x80000009u, 0x00000000u>;
	}
	// P_mtx1_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_
// num_verts= 75927898
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[21237923793720] = TemplatedLoader<0x401, 0x00030f00u, 0x40e00c09u, 0x80000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[21237923793720] = TemplatedLoader<0x301, 0x00030f00u, 0x40e00c09u, 0x80000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[21237923793720] = TemplatedLoader<0, 0x00030f00u, 0x40e00c09u, 0x80000000u, 0x00000000u>;
	}
	// P_mtx0_2_I16_flt_T0_mtx0_1_Dir_flt_
// num_verts= 72944720
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20677452171751] = TemplatedLoader<0x401, 0x00010300u, 0x41200008u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20677452171751] = TemplatedLoader<0x301, 0x00010300u, 0x41200008u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20677452171751] = TemplatedLoader<0, 0x00010300u, 0x41200008u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_s16_
// num_verts= 34229824
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20597412766008] = TemplatedLoader<0x401, 0x00010100u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20597412766008] = TemplatedLoader<0x301, 0x00010100u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20597412766008] = TemplatedLoader<0, 0x00010100u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_2_I16_flt_
// num_verts= 5153276
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20154682968551] = TemplatedLoader<0x401, 0x00000300u, 0x40000008u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20154682968551] = TemplatedLoader<0x301, 0x00000300u, 0x40000008u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20154682968551] = TemplatedLoader<0, 0x00000300u, 0x40000008u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_flt_C0_1_I16_8888_
// num_verts= 767382
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20187971085112] = TemplatedLoader<0x401, 0x00003300u, 0x40016009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20187971085112] = TemplatedLoader<0x301, 0x00003300u, 0x40016009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20187971085112] = TemplatedLoader<0, 0x00003300u, 0x40016009u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_flt_Nrm_0_0_I16_s8_T0_mtx0_1_I16_flt_
// num_verts= 322276
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[21022402986808] = TemplatedLoader<0x401, 0x00030f00u, 0x41200409u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[21022402986808] = TemplatedLoader<0x301, 0x00030f00u, 0x41200409u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[21022402986808] = TemplatedLoader<0, 0x00030f00u, 0x41200409u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_flt_T0_mtx0_1_I16_s16_
// num_verts= 310152
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20935761679160] = TemplatedLoader<0x401, 0x00030300u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20935761679160] = TemplatedLoader<0x301, 0x00030300u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20935761679160] = TemplatedLoader<0, 0x00030300u, 0x40e00009u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_2_Dir_s8_T0_mtx0_1_Dir_s8_
// num_verts= 255160
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20439966851073] = TemplatedLoader<0x401, 0x00010100u, 0x40600002u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20439966851073] = TemplatedLoader<0x301, 0x00010100u, 0x40600002u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20439966851073] = TemplatedLoader<0, 0x00010100u, 0x40600002u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_s16_T0_mtx0_1_I16_s16_
// num_verts= 232200
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20935761641622] = TemplatedLoader<0x401, 0x00030300u, 0x40e00007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20935761641622] = TemplatedLoader<0x301, 0x00030300u, 0x40e00007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20935761641622] = TemplatedLoader<0, 0x00030300u, 0x40e00007u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I8_flt_T0_mtx0_1_Dir_s8_
// num_verts= 47672
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20440625248824] = TemplatedLoader<0x401, 0x00010200u, 0x40600009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20440625248824] = TemplatedLoader<0x301, 0x00010200u, 0x40600009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20440625248824] = TemplatedLoader<0, 0x00010200u, 0x40600009u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_s16_
// num_verts= 46368
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20977025851192] = TemplatedLoader<0x401, 0x00033f00u, 0x40e17009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20977025851192] = TemplatedLoader<0x301, 0x00033f00u, 0x40e17009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20977025851192] = TemplatedLoader<0, 0x00033f00u, 0x40e17009u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_u16_
// num_verts= 32352
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20518689836694] = TemplatedLoader<0x401, 0x00010100u, 0x40a00007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20518689836694] = TemplatedLoader<0x301, 0x00010100u, 0x40a00007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20518689836694] = TemplatedLoader<0, 0x00010100u, 0x40a00007u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_Dir_s16_T0_mtx0_1_Dir_s8_
// num_verts= 808
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20439966944918] = TemplatedLoader<0x401, 0x00010100u, 0x40600007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20439966944918] = TemplatedLoader<0x301, 0x00010100u, 0x40600007u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20439966944918] = TemplatedLoader<0, 0x00010100u, 0x40600007u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_Dir_s8_
// num_verts= 768
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
	pvlmap[20153366341970] = TemplatedLoader<0x401, 0x00000100u, 0x40000003u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[20153366341970] = TemplatedLoader<0x301, 0x00000100u, 0x40000003u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[20153366341970] = TemplatedLoader<0, 0x00000100u, 0x40000003u, 0x00000000u, 0x00000000u>;
	}
	// P_mtx0_3_I16_flt_C0_1_I16_8888_T0_mtx0_1_I16_flt_
	// num_verts= 10540854
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		pvlmap[21047772668728] = TemplatedLoader<0x401, 0x00033300u, 0x41216009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
	pvlmap[21047772668728] = TemplatedLoader<0x301, 0x00033300u, 0x41216009u, 0x00000000u, 0x00000000u>;
	}
	else
#endif
	{
	pvlmap[21047772668728] = TemplatedLoader<0, 0x00033300u, 0x41216009u, 0x00000000u, 0x00000000u>;
	}
}
