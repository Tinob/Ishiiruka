// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_GLMP01_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_GLMP01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_
// num_verts= 157539339
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21022460607638] = TemplatedLoader<0x301, 0x00030f00u, 0x41201007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21022460607638] = TemplatedLoader<0, 0x00030f00u, 0x41201007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_C0_1_I8_8888_
// num_verts= 44155017
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20471644083000] = TemplatedLoader<0x301, 0x00002300u, 0x40016009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20471644083000] = TemplatedLoader<0, 0x00002300u, 0x40016009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_flt_
// num_verts= 27762620
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21316673620244] = TemplatedLoader<0x301, 0x00030f03u, 0x41201009u, 0x80000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21316673620244] = TemplatedLoader<0, 0x00030f03u, 0x41201009u, 0x80000009u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_
// num_verts= 16743933
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21317235249656] = TemplatedLoader<0x301, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x04824000u>;
  }
  else
#endif
  {
    pvlmap[21317235249656] = TemplatedLoader<0, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x04824000u>;
  }
  // P_mtx0_3_I8_flt_T0_mtx0_1_I8_flt_
// num_verts= 9856328
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20845310114360] = TemplatedLoader<0x301, 0x00020200u, 0x41200009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20845310114360] = TemplatedLoader<0, 0x00020200u, 0x41200009u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_1_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_
// num_verts= 7224640
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[22965922427127] = TemplatedLoader<0x301, 0x000f0f00u, 0x40e00e09u, 0x00000007u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[22965922427127] = TemplatedLoader<0, 0x000f0f00u, 0x40e00e09u, 0x00000007u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_
// num_verts= 5189843
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21317218439672] = TemplatedLoader<0x301, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x0381c000u>;
  }
  else
#endif
  {
    pvlmap[21317218439672] = TemplatedLoader<0, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x0381c000u>;
  }
  // P_mtx0_3_I16_s16_T0_mtx0_1_I16_flt_
// num_verts= 3961087
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21014484533398] = TemplatedLoader<0x301, 0x00030300u, 0x41200007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21014484533398] = TemplatedLoader<0, 0x00030300u, 0x41200007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_u16_
// num_verts= 2109996
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21316673619696] = TemplatedLoader<0x301, 0x00030f03u, 0x41201009u, 0x80000005u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21316673619696] = TemplatedLoader<0, 0x00030f03u, 0x41201009u, 0x80000005u, 0x00000000u>;
  }
  // P_mtx1_3_Dir_flt_T0_mtx0_1_I8_u8_
// num_verts= 1011732
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20823965540664] = TemplatedLoader<0x301, 0x00020100u, 0x40200009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20823965540664] = TemplatedLoader<0, 0x00020100u, 0x40200009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx1_1_I16_flt_T1_mtx1_1_Inv_s16_
// num_verts= 961738
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21316673619970] = TemplatedLoader<0x301, 0x00030f03u, 0x41201009u, 0x80000007u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21316673619970] = TemplatedLoader<0, 0x00030f03u, 0x41201009u, 0x80000007u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_
// num_verts= 669768
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21317159604728] = TemplatedLoader<0x301, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21317159604728] = TemplatedLoader<0, 0x00030fc0u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_
// num_verts= 617108
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20530913410710] = TemplatedLoader<0x301, 0x00011100u, 0x40a16007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20530913410710] = TemplatedLoader<0, 0x00011100u, 0x40a16007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I8_s16_T0_mtx0_1_I8_u8_
// num_verts= 93160
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20824623769494] = TemplatedLoader<0x301, 0x00020200u, 0x40200007u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20824623769494] = TemplatedLoader<0, 0x00020200u, 0x40200007u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_2_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_u16_
// num_verts= 75408
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20530913429479] = TemplatedLoader<0x301, 0x00011100u, 0x40a16008u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20530913429479] = TemplatedLoader<0, 0x00011100u, 0x40a16008u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_C0_1_Dir_8888_
// num_verts= 62736
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20165589991062] = TemplatedLoader<0x301, 0x00001100u, 0x40016007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20165589991062] = TemplatedLoader<0, 0x00001100u, 0x40016007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_s16_T1_mtx0_1_I16_s16_
// num_verts= 47280
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[22965912817399] = TemplatedLoader<0x301, 0x000f0f00u, 0x40e00c09u, 0x00000007u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[22965912817399] = TemplatedLoader<0, 0x000f0f00u, 0x40e00c09u, 0x00000007u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_1_1_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_
// num_verts= 45666
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[61623365448696] = TemplatedLoader<0x301, 0x00030fc0u, 0xc1201209u, 0x80000000u, 0x04824000u>;
  }
  else
#endif
  {
    pvlmap[61623365448696] = TemplatedLoader<0, 0x00030fc0u, 0xc1201209u, 0x80000000u, 0x04824000u>;
  }
  // P_mtx0_3_I16_s16_Nrm_0_0_I16_flt_
// num_verts= 15072
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20162659024022] = TemplatedLoader<0x301, 0x00000f00u, 0x40001007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20162659024022] = TemplatedLoader<0, 0x00000f00u, 0x40001007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_1_Inv_s16_T7_mtx1_1_Inv_s16_
// num_verts= 6336
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21309242365432] = TemplatedLoader<0x301, 0x000303c0u, 0x41200009u, 0x80000000u, 0x0381c000u>;
  }
  else
#endif
  {
    pvlmap[21309242365432] = TemplatedLoader<0, 0x000303c0u, 0x41200009u, 0x80000000u, 0x0381c000u>;
  }
  // P_mtx0_3_I16_s16_
// num_verts= 4140
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20154682949782] = TemplatedLoader<0x301, 0x00000300u, 0x40000007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20154682949782] = TemplatedLoader<0, 0x00000300u, 0x40000007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_C0_1_I16_8888_T0_mtx0_1_I16_flt_
// num_verts= 3912
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21046456098454] = TemplatedLoader<0x301, 0x00033100u, 0x41216007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21046456098454] = TemplatedLoader<0, 0x00033100u, 0x41216007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_T6_mtx1_1_Inv_flt_T7_mtx1_1_Inv_flt_
// num_verts= 1986
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20449457591800] = TemplatedLoader<0x301, 0x000003c0u, 0x40000009u, 0x80000000u, 0x04824000u>;
  }
  else
#endif
  {
    pvlmap[20449457591800] = TemplatedLoader<0, 0x000003c0u, 0x40000009u, 0x80000000u, 0x04824000u>;
  }
  // P_mtx1_3_I16_flt_T0_mtx0_1_I16_flt_T6_mtx1_0_Inv_u8_T7_mtx1_0_Inv_u8_
// num_verts= 528
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21309183530488] = TemplatedLoader<0x301, 0x000303c0u, 0x41200009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21309183530488] = TemplatedLoader<0, 0x000303c0u, 0x41200009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_T0_mtx1_1_Inv_flt_T1_mtx1_1_Inv_flt_
// num_verts= 330
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20803148975380] = TemplatedLoader<0x301, 0x00000303u, 0x41200009u, 0x80000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20803148975380] = TemplatedLoader<0, 0x00000303u, 0x41200009u, 0x80000009u, 0x00000000u>;
  }
}
