// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_G4BP08_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_G4BP08_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx1_3_Dir_flt_Nrm_0_0_Dir_s8_T0_mtx0_1_Dir_flt_
// num_verts= 210524
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20972993202488] = TemplatedLoader<0x301, 0x00010500u, 0x41200409u, 0x80000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20972993202488] = TemplatedLoader<0, 0x00010500u, 0x41200409u, 0x80000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_s8_Nrm_0_0_Dir_s8_T0_mtx0_1_Dir_s8_
// num_verts= 191420
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20442619154770] = TemplatedLoader<0x301, 0x00010500u, 0x40600403u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20442619154770] = TemplatedLoader<0, 0x00010500u, 0x40600403u, 0x00000000u, 0x00000000u>;
  }
  // P_mtx0_3_Dir_flt_Nrm_0_0_Dir_s8_T0_mtx0_1_Dir_flt_
// num_verts= 143592
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20678787942712] = TemplatedLoader<0x301, 0x00010500u, 0x41200409u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20678787942712] = TemplatedLoader<0, 0x00010500u, 0x41200409u, 0x00000000u, 0x00000000u>;
  }
}
