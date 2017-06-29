// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/G_GB4P51_pvt.h"
#include "VideoCommon/VertexLoader_Template.h"



void G_GB4P51_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)
{
  // P_mtx0_3_I16_s16_Nrm_0_0_I16_s8_T0_mtx0_1_I16_s16_
// num_verts= 139264034
#if _M_SSE >= 0x301
  if (cpu_info.bSSSE3)
  {
    pvlmap[20943680057494] = TemplatedLoader<0x301, 0x00030f00u, 0x40e00407u, 0x00000000u, 0x00000000u>;
  }
  else
#endif
  {
    pvlmap[20943680057494] = TemplatedLoader<0, 0x00030f00u, 0x40e00407u, 0x00000000u, 0x00000000u>;
  }
}
