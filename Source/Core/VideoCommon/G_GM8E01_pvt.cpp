// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_GM8E01_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_GM8E01_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_flt_
// num_verts= 24932407
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[22887189925897] = TemplatedLoader<0x301, 0x000f0f00u, 0x40a00c09u, 0x00000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[22887189925897] = TemplatedLoader<0, 0x000f0f00u, 0x40a00c09u, 0x00000009u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s16_C0_1_Dir_4444_
// num_verts= 11329846
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20164974968470] = TemplatedLoader<0x301, 0x00001100u, 0x4000e007u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20164974968470] = TemplatedLoader<0, 0x00001100u, 0x4000e007u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_
// num_verts= 10822718
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[31133432690185] = TemplatedLoader<0x301, 0x003f0f00u, 0x41201009u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[31133432690185] = TemplatedLoader<0, 0x003f0f00u, 0x41201009u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_
// num_verts= 7671147
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[23044654928905] = TemplatedLoader<0x301, 0x000f0f00u, 0x41201009u, 0x00000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[23044654928905] = TemplatedLoader<0, 0x000f0f00u, 0x41201009u, 0x00000009u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_
// num_verts= 5082850
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21022441425720] = TemplatedLoader<0x301, 0x00030f00u, 0x41200c09u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21022441425720] = TemplatedLoader<0, 0x00030f00u, 0x41200c09u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_s8_C0_1_Dir_8888_
// num_verts= 972930
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20168242313528] = TemplatedLoader<0x301, 0x00001500u, 0x40016409u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20168242313528] = TemplatedLoader<0, 0x00001500u, 0x40016409u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_
// num_verts= 742268
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[23044635709449] = TemplatedLoader<0x301, 0x000f0f00u, 0x41200c09u, 0x00000009u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[23044635709449] = TemplatedLoader<0, 0x000f0f00u, 0x41200c09u, 0x00000009u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_u16_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_
// num_verts= 730216
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[30975967687177] = TemplatedLoader<0x301, 0x003f0f00u, 0x40a00c09u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[30975967687177] = TemplatedLoader<0, 0x003f0f00u, 0x40a00c09u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_s8_
// num_verts= 558356
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20452190556472] = TemplatedLoader<0x301, 0x00011100u, 0x40616009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20452190556472] = TemplatedLoader<0, 0x00011100u, 0x40616009u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_flt_
// num_verts= 258020
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20162659061560] = TemplatedLoader<0x301, 0x00000f00u, 0x40001009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20162659061560] = TemplatedLoader<0, 0x00000f00u, 0x40001009u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I16_flt_Nrm_0_0_I16_s16_T0_mtx0_1_I16_flt_T1_mtx0_1_I16_flt_T2_mtx0_1_I16_flt_
// num_verts= 228150
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[31133413470729] = TemplatedLoader<0x301, 0x003f0f00u, 0x41200c09u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[31133413470729] = TemplatedLoader<0, 0x003f0f00u, 0x41200c09u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_C0_1_Dir_8888_T0_mtx0_1_Dir_flt_T1_mtx0_1_Dir_flt_T2_mtx0_1_Dir_flt_
// num_verts= 26400
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[24058683668489] = TemplatedLoader<0x301, 0x00151100u, 0x41216009u, 0x00001209u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[24058683668489] = TemplatedLoader<0, 0x00151100u, 0x41216009u, 0x00001209u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_flt_C0_1_Dir_8888_
// num_verts= 10372
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20168299971896] = TemplatedLoader<0x301, 0x00001500u, 0x40017009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20168299971896] = TemplatedLoader<0, 0x00001500u, 0x40017009u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_T0_mtx0_1_Dir_u16_
// num_verts= 3972
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20518689874232] = TemplatedLoader<0x301, 0x00010100u, 0x40a00009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20518689874232] = TemplatedLoader<0, 0x00010100u, 0x40a00009u, 0x00000000u, 0x00000000u>;
  }
}
