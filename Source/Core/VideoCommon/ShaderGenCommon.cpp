// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/ShaderGenCommon.h"
#include "Common/CommonPaths.h"
#include "Common/FileUtil.h"
#include "Core/ConfigManager.h"

ShaderHostConfig ShaderHostConfig::GetCurrent()
{
  ShaderHostConfig bits = {};
  bits.msaa = g_ActiveConfig.iMultisamples > 1;
  bits.ssaa = g_ActiveConfig.iMultisamples > 1 && g_ActiveConfig.bSSAA &&
    g_ActiveConfig.backend_info.bSupportsSSAA;
  bits.stereo = g_ActiveConfig.iStereoMode > 0;
  bits.wireframe = g_ActiveConfig.bWireFrame;
  bits.fast_depth_calc = g_ActiveConfig.bFastDepthCalc;
  bits.bounding_box = g_ActiveConfig.iBBoxMode == BBoxMode::BBoxGPU;
  bits.backend_dual_source_blend = g_ActiveConfig.backend_info.bSupportsDualSourceBlend;
  bits.backend_geometry_shaders = g_ActiveConfig.backend_info.bSupportsGeometryShaders;
  bits.backend_early_z = g_ActiveConfig.backend_info.bSupportsEarlyZ;
  bits.backend_bbox = g_ActiveConfig.backend_info.bSupportsBBox;
  bits.backend_gs_instancing = g_ActiveConfig.backend_info.bSupportsGSInstancing;
  bits.backend_clip_control = g_ActiveConfig.backend_info.bSupportsClipControl;
  bits.backend_ssaa = g_ActiveConfig.backend_info.bSupportsSSAA;
  bits.backend_atomics = g_ActiveConfig.backend_info.bSupportsFragmentStoresAndAtomics;
  bits.backend_depth_clamp = g_ActiveConfig.backend_info.bSupportsDepthClamp;
  bits.backend_reversed_depth_range = g_ActiveConfig.backend_info.bSupportsReversedDepthRange;
  bits.backend_bitfield = g_ActiveConfig.backend_info.bSupportsBitfield;
  bits.backend_dynamic_sampler_indexing =
    g_ActiveConfig.backend_info.bSupportsDynamicSamplerIndexing;
  return bits;
}

std::string GetDiskShaderCacheFileName(API_TYPE api_type, const char* type, bool include_gameid,
  bool include_host_config, bool uid)
{
  std::string filename;
  if (uid)
  {
    if (!File::Exists(File::GetUserPath(D_SHADERUIDCACHE_IDX)))
      File::CreateDir(File::GetUserPath(D_SHADERUIDCACHE_IDX));
    filename = File::GetUserPath(D_SHADERUIDCACHE_IDX);
  }
  else
  {
    if (!File::Exists(File::GetUserPath(D_SHADERCACHE_IDX)))
      File::CreateDir(File::GetUserPath(D_SHADERCACHE_IDX));
    filename = File::GetUserPath(D_SHADERCACHE_IDX);
  }
  switch (api_type)
  {
  case API_TYPE::API_D3D11:
    filename += "IDX11";
    break;
  case API_TYPE::API_D3D9:
  case API_TYPE::API_D3D9_SM20:
  case API_TYPE::API_D3D9_SM30:
    filename += "IDX9";
    break;
  case API_TYPE::API_OPENGL:
    filename += "IOGL";
    break;
  case API_TYPE::API_VULKAN:
    filename += "IVK";
    break;
  default:
    break;
  }

  filename += '-';
  filename += type;

  if (include_gameid)
  {
    filename += '-';
    filename += SConfig::GetInstance().GetGameID();
  }

  if (include_host_config)
  {
    // We're using 20 bits, so 5 hex characters.
    ShaderHostConfig host_config = ShaderHostConfig::GetCurrent();
    filename += StringFromFormat("-%05X", host_config.bits);
  }

  filename += ".cache";
  return filename;
}
