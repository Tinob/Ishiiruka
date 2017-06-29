// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_SDWP18_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_SDWP18_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx1_3_I16_flt_Nrm_0_0_I16_flt_C0_0_I8_565_T0_mtx0_1_I16_flt_
// num_verts= 37902932
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[21337730428728] = TemplatedLoader<0x301, 0x00032f00u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[21337730428728] = TemplatedLoader<0, 0x00032f00u, 0x41201009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx1_3_I8_flt_Nrm_0_0_I8_flt_
// num_verts= 6782698
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20453572989496] = TemplatedLoader<0x301, 0x00000a00u, 0x40001009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20453572989496] = TemplatedLoader<0, 0x00000a00u, 0x40001009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_T0_mtx0_1_I8_flt_
// num_verts= 1637648
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20871717646904] = TemplatedLoader<0x301, 0x00022a00u, 0x41201009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20871717646904] = TemplatedLoader<0, 0x00022a00u, 0x41201009u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx1_3_I16_flt_Nrm_0_0_I8_flt_
// num_verts= 1612930
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20454231255864] = TemplatedLoader<0x301, 0x00000b00u, 0x40001009u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20454231255864] = TemplatedLoader<0, 0x00000b00u, 0x40001009u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_2_I8_flt_Nrm_0_0_I8_flt_
// num_verts= 1405872
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20159367710951] = TemplatedLoader<0x301, 0x00000a00u, 0x40001008u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20159367710951] = TemplatedLoader<0, 0x00000a00u, 0x40001008u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_I8_flt_Nrm_0_0_I8_flt_C0_0_I8_565_
// num_verts= 186184
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20180432253496] = TemplatedLoader<0x301, 0x00002a00u, 0x40001009u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20180432253496] = TemplatedLoader<0, 0x00002a00u, 0x40001009u, 0x00000000u, 0x00000000u>;
  }
}
