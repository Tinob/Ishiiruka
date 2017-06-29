// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_GFZE01_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_GFZE01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_
// num_verts= 67537925
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21352910363145] = TemplatedLoader<0x301, 0x00050500u, 0x41201009u, 0x00000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21352910363145] = TemplatedLoader<0, 0x00050500u, 0x41201009u, 0x00000009u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_T2_mtx0_1_Dir_flt_
// num_verts= 27065691
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[24049170037769] = TemplatedLoader<0x301, 0x00150500u, 0x41201009u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[24049170037769] = TemplatedLoader<0, 0x00150500u, 0x41201009u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_Nrm_0_0_Dir_s16_T0_mtx0_1_Dir_s16_
// num_verts= 24996385
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20600103452310] = TemplatedLoader<0x301, 0x00010500u, 0x40e00c07u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20600103452310] = TemplatedLoader<0, 0x00010500u, 0x40e00c07u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_Nrm_0_0_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_T2_mtx0_1_Dir_s16_
// num_verts= 21172985
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[23970427748437] = TemplatedLoader<0x301, 0x00150500u, 0x40e00c07u, 0x00000e07u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[23970427748437] = TemplatedLoader<0, 0x00150500u, 0x40e00c07u, 0x00000e07u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_1_0_Dir_flt_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_T2_mtx0_1_Dir_flt_
// num_verts= 9341798
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[24049179647497] = TemplatedLoader<0x301, 0x00150500u, 0x41201209u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[24049179647497] = TemplatedLoader<0, 0x00150500u, 0x41201209u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx1_3_Dir_flt_Nrm_0_0_Dir_flt_T0_mtx0_1_Dir_flt_
// num_verts= 9269969
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20973050860856] = TemplatedLoader<0x301, 0x00010500u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20973050860856] = TemplatedLoader<0, 0x00010500u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_1_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_T2_mtx0_1_Dir_flt_
// num_verts= 7297696
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[24061403221513] = TemplatedLoader<0x301, 0x00151500u, 0x41217209u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[24061403221513] = TemplatedLoader<0, 0x00151500u, 0x41217209u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx1_3_Dir_s16_Nrm_0_0_Dir_s16_T0_mtx0_1_Dir_s16_
// num_verts= 7134932
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20894308712086] = TemplatedLoader<0x301, 0x00010500u, 0x40e00c07u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20894308712086] = TemplatedLoader<0, 0x00010500u, 0x40e00c07u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_T2_mtx0_1_Dir_flt_
// num_verts= 5379890
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[24061393611785] = TemplatedLoader<0x301, 0x00151500u, 0x41217009u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[24061393611785] = TemplatedLoader<0, 0x00151500u, 0x41217009u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_Nrm_0_0_Dir_s16_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_
// num_verts= 4522416
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21274168214101] = TemplatedLoader<0x301, 0x00050500u, 0x40e00c07u, 0x00000007u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21274168214101] = TemplatedLoader<0, 0x00050500u, 0x40e00c07u, 0x00000007u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_Nrm_0_0_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_
// num_verts= 2193030
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20612327026326] = TemplatedLoader<0x301, 0x00011500u, 0x40e16c07u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20612327026326] = TemplatedLoader<0, 0x00011500u, 0x40e16c07u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_Nrm_0_0_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_T1_mtx0_1_Dir_s16_
// num_verts= 2084570
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21286391788117] = TemplatedLoader<0x301, 0x00051500u, 0x40e16c07u, 0x00000007u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21286391788117] = TemplatedLoader<0, 0x00051500u, 0x40e16c07u, 0x00000007u, 0x00000000u>;
  }
  // P_mtx1_3_Dir_s16_Nrm_0_0_Dir_s16_
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20450262400662] = TemplatedLoader<0x301, 0x00000500u, 0x40000c07u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20450262400662] = TemplatedLoader<0, 0x00000500u, 0x40000c07u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx1_3_Dir_s16_Nrm_0_0_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_s16_
// num_verts= 12
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20906532286102] = TemplatedLoader<0x301, 0x00011500u, 0x40e16c07u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20906532286102] = TemplatedLoader<0, 0x00011500u, 0x40e16c07u, 0x80000000u, 0x00000000u>;
  }
}
