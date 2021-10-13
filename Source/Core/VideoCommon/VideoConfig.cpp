// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <cmath>
#include <mutex>

#include "Common/CPUDetect.h"
#include "Common/CommonTypes.h"
#include "Common/StringUtil.h"
#include "Core/Config/GraphicsSettings.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/Movie.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"

VideoConfig g_Config;
VideoConfig g_ActiveConfig;
static std::mutex config_mutex;
static bool s_has_registered_callback = false;

void UpdateActiveConfig()
{
  if (Movie::IsPlayingInput() && Movie::IsConfigSaved())
    Movie::SetGraphicsConfig();
  std::unique_lock<std::mutex> config_lock(config_mutex);
  g_ActiveConfig = g_Config;
}
void VideoConfig::ClearFormats()
{
  for (s32 i = 0; i < 16; i++)
  {
    backend_info.bSupportedFormats[i] = false;
  }
}

VideoConfig::VideoConfig()
{
  // Needed for the first frame, I think
  fAspectRatioHackW = 1;
  fAspectRatioHackH = 1;

  // disable all features by default
  backend_info.APIType = API_NONE;
  ClearFormats();
  backend_info.bSupportsExclusiveFullscreen = false;

  // Game-specific stereoscopy settings
  bStereoEFBMonoDepth = false;
  iStereoDepthPercentage = 100;
  iStereoConvergence = 20;
  bUseScalingFilter = false;
  bTexDeposterize = false;
  iTexScalingType = 0;
  iTexScalingFactor = 2;
  backend_info.bSupportsMultithreading = false;
  backend_info.bSupportsInternalResolutionFrameDumps = false;
  bEnableValidationLayer = false;
  bEnableShaderDebug = false;
  bBackendMultithreading = true;
  backend_info.MaxTextureSize = 4096;
}

void VideoConfig::Refresh()
{
  if (!s_has_registered_callback)
  {
    // There was a race condition between the video thread and the host thread here, if
    // corrections need to be made by VerifyValidity(). Briefly, the config will contain
    // invalid values. Instead, pause emulation first, which will flush the video thread,
    // update the config and correct it, then resume emulation, after which the video
    // thread will detect the config has changed and act accordingly.
    Config::AddConfigChangedCallback([]()
    {
      Core::RunAsCPUThread([]() { g_Config.Refresh(); }); });
    s_has_registered_callback = true;
  }
  std::unique_lock<std::mutex> config_lock(config_mutex);
  bVSync = Config::Get(Config::GFX_VSYNC);
  iAdapter = Config::Get(Config::GFX_ADAPTER);

  bWidescreenHack = Config::Get(Config::GFX_WIDESCREEN_HACK);
  iAspectRatio = Config::Get(Config::GFX_ASPECT_RATIO);
  bCrop = Config::Get(Config::GFX_CROP);
  bUseXFB = Config::Get(Config::GFX_USE_XFB);
  bUseRealXFB = Config::Get(Config::GFX_USE_REAL_XFB);
  bBlackFrameInsertion = Config::Get(Config::GFX_USE_BLACK_FRAME_INSERTION);  
  iSafeTextureCache_ColorSamples = Config::Get(Config::GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES);
  bShowFPS = Config::Get(Config::GFX_SHOW_FPS);
  bShowNetPlayPing = Config::Get(Config::GFX_SHOW_NETPLAY_PING);
  bShowNetPlayMessages = Config::Get(Config::GFX_SHOW_NETPLAY_MESSAGES);
  bLogRenderTimeToFile = Config::Get(Config::GFX_LOG_RENDER_TIME_TO_FILE);
  bOverlayStats = Config::Get(Config::GFX_OVERLAY_STATS);
  bOverlayProjStats = Config::Get(Config::GFX_OVERLAY_PROJ_STATS);
  bDumpTextures = Config::Get(Config::GFX_DUMP_TEXTURES);
  bHiresTextures = Config::Get(Config::GFX_HIRES_TEXTURES);
  bHiresMaterialMaps = Config::Get(Config::GFX_HIRES_MATERIAL_MAPS);
  bHiresMaterialMapsBuild = Config::Get(Config::GFX_HIRES_MATERIAL_MAPS_BUILD);  
  bCacheHiresTextures = Config::Get(Config::GFX_CACHE_HIRES_TEXTURES);
  bWaitForCacheHiresTextures = Config::Get(Config::GFX_WAIT_CACHE_HIRES_TEXTURES);
  bDumpEFBTarget = Config::Get(Config::GFX_DUMP_EFB_TARGET);
  bDumpFramesAsImages = Config::Get(Config::GFX_DUMP_FRAMES_AS_IMAGES);
  bFreeLook = Config::Get(Config::GFX_FREE_LOOK);
  bCompileShaderOnStartup = Config::Get(Config::GFX_COMPILE_SHADERS_ON_STARTUP);
  bUseFFV1 = Config::Get(Config::GFX_USE_FFV1);
  sDumpFormat = Config::Get(Config::GFX_DUMP_FORMAT);
  sDumpCodec = Config::Get(Config::GFX_DUMP_CODEC);
  sDumpPath = Config::Get(Config::GFX_DUMP_PATH);
  iBitrateKbps = Config::Get(Config::GFX_BITRATE_KBPS);
  bInternalResolutionFrameDumps = Config::Get(Config::GFX_INTERNAL_RESOLUTION_FRAME_DUMPS);
  bEnableGPUTextureDecoding = Config::Get(Config::GFX_ENABLE_GPU_TEXTURE_DECODING);
  bEnableComputeTextureEncoding = Config::Get(Config::GFX_ENABLE_COMPUTE_TEXTURE_ENCODING);
  bEnablePixelLighting = Config::Get(Config::GFX_ENABLE_PIXEL_LIGHTING);
  bForcedLighting = Config::Get(Config::GFX_FORCED_LIGHTING);
  bForcePhongShading = Config::Get(Config::GFX_FORCE_PHONG_SHADING);
  iRimPower = Config::Get(Config::GFX_RIM_POWER);
  iRimIntesity = Config::Get(Config::GFX_RIM_INTENSITY);
  iRimBase = Config::Get(Config::GFX_RIM_BASE);
  iSpecularMultiplier = Config::Get(Config::GFX_SPECULAR_MULTIPLIER);
  bSimBumpEnabled = Config::Get(Config::GFX_SIMULATE_BUMP_MAPPING);
  iSimBumpStrength = Config::Get(Config::GFX_SIMULATE_BUMP_STRENGTH);
  iSimBumpDetailFrequency = Config::Get(Config::GFX_SIMULATE_BUMP_DETAIL_FREQUENCY);
  iSimBumpThreshold = Config::Get(Config::GFX_SIMULATE_BUMP_THRESHOLD);
  iSimBumpDetailBlend = Config::Get(Config::GFX_SIMULATE_BUMP_DETAIL_BLEND);
  bForcedDithering = Config::Get(Config::GFX_FORCED_DITHERING);

  bFastDepthCalc = Config::Get(Config::GFX_FAST_DEPTH_CALC);
  iMultisamples = Config::Get(Config::GFX_MSAA);
  bSSAA = Config::Get(Config::GFX_SSAA);
  iEFBScale = Config::Get(Config::GFX_EFB_SCALE);
  bTexFmtOverlayEnable = Config::Get(Config::GFX_TEXFMT_OVERLAY_ENABLE);
  bTexFmtOverlayCenter = Config::Get(Config::GFX_TEXFMT_OVERLAY_CENTER);
  bWireFrame = Config::Get(Config::GFX_ENABLE_WIREFRAME);
  bDisableFog = Config::Get(Config::GFX_DISABLE_FOG);
  bBorderlessFullscreen = Config::Get(Config::GFX_BORDERLESS_FULLSCREEN);
  bEnableValidationLayer = Config::Get(Config::GFX_ENABLE_VALIDATION_LAYER);
  bEnableShaderDebug = Config::Get(Config::GFX_ENABLE_SHADER_DEBUG);
  bBackendMultithreading = Config::Get(Config::GFX_BACKEND_MULTITHREADING);
  iCommandBufferExecuteInterval = Config::Get(Config::GFX_COMMAND_BUFFER_EXECUTE_INTERVAL);

  bZComploc = Config::Get(Config::GFX_SW_ZCOMPLOC);
  bZFreeze = Config::Get(Config::GFX_SW_ZFREEZE);
  bDumpObjects = Config::Get(Config::GFX_SW_DUMP_OBJECTS);
  bDumpTevStages = Config::Get(Config::GFX_SW_DUMP_TEV_STAGES);
  bDumpTevTextureFetches = Config::Get(Config::GFX_SW_DUMP_TEV_TEX_FETCHES);
  drawStart = Config::Get(Config::GFX_SW_DRAW_START);
  drawEnd = Config::Get(Config::GFX_SW_DRAW_END);

  int fmode = Config::Get(Config::GFX_ENHANCE_FILTERING_MODE);
  fmode = std::min(fmode, static_cast<int>(FilteringMode::Forced));
  fmode = std::max(fmode, static_cast<int>(FilteringMode::Disabled));
  eFilteringMode = static_cast<FilteringMode>(fmode);

  int cmode = Config::Get(Config::GFX_HACK_CULL_MODE);
  cmode = std::min(cmode, static_cast<int>(HostCullMode::Back));
  cmode = std::max(cmode, static_cast<int>(HostCullMode::Native));
  eCullMode = static_cast<HostCullMode>(cmode);

  iMaxAnisotropy = Config::Get(Config::GFX_ENHANCE_MAX_ANISOTROPY);
  bPostProcessingEnable = Config::Get(Config::GFX_ENHANCE_POST_ENABLED);
  bPostProcessingEfbMustBePerspective = Config::Get(Config::GFX_POST_EFB_PERSPECTIVE);
  bPostProcessingEfbMustBeAspect = Config::Get(Config::GFX_POST_EFB_ASPECT);
  bPostProcessingEfbFailsafe = Config::Get(Config::GFX_POST_EFB_FAILSAFE);
  iPostProcessingTrigger = Config::Get(Config::GFX_ENHANCE_POST_TRIGUER);
  iPostProcessingEfbMinResolutionPercent = Config::Get(Config::GFX_ENHANCE_POST_EFB_RESOLUTION_PERCENT_MIN);
  iPostProcessingEfbIndex = Config::Get(Config::GFX_ENHANCE_POST_EFB_INDEX);
  sPostProcessingShaders = Config::Get(Config::GFX_ENHANCE_POST_SHADERS);
  sScalingShader = Config::Get(Config::GFX_ENHANCE_SCALING_SHADER);
  bForceTrueColor = Config::Get(Config::GFX_ENHANCE_FORCE_TRUE_COLOR);
  bHPFrameBuffer = Config::Get(Config::GFX_ENHANCE_HP_FRAME_BUFFER);
  bUseScalingFilter = Config::Get(Config::GFX_ENHANCE_USE_SCALING_FILTER);

  iTexScalingType = Config::Get(Config::GFX_ENHANCE_TEXTURE_SCALING_TYPE);
  iTexScalingFactor = Config::Get(Config::GFX_ENHANCE_TEXTURE_SCALING_FACTOR);
  bTexDeposterize = Config::Get(Config::GFX_ENHANCE_USE_DEPOSTERIZE);

  bTessellation = Config::Get(Config::GFX_ENHANCE_TESSELLATION);
  bTessellationEarlyCulling = Config::Get(Config::GFX_ENHANCE_TESSELLATION_EARLY_CULLING);
  iTessellationDistance = Config::Get(Config::GFX_ENHANCE_TESSELLATION_DISTANCE);
  iTessellationMax = Config::Get(Config::GFX_ENHANCE_TESSELLATION_MAX);
  iTessellationRoundingIntensity = Config::Get(Config::GFX_ENHANCE_TESSELLATION_ROUNDING_INTENSITY);
  iTessellationDisplacementIntensity = Config::Get(Config::GFX_ENHANCE_TESSELLATION_DISPLACEMENT_INTENSITY);

  iStereoMode = Config::Get(Config::GFX_STEREO_MODE);
  iStereoDepth = Config::Get(Config::GFX_STEREO_DEPTH);
  iStereoConvergencePercentage = Config::Get(Config::GFX_STEREO_CONVERGENCE_PERCENTAGE);
  bStereoSwapEyes = Config::Get(Config::GFX_STEREO_SWAP_EYES);
  iStereoConvergence = Config::Get(Config::GFX_STEREO_CONVERGENCE);
  bStereoEFBMonoDepth = Config::Get(Config::GFX_STEREO_EFB_MONO_DEPTH);
  iStereoDepthPercentage = Config::Get(Config::GFX_STEREO_DEPTH_PERCENTAGE);
  sStereoShader = Config::Get(Config::GFX_STEREO_SHADER);

  bEFBAccessEnable = Config::Get(Config::GFX_HACK_EFB_ACCESS_ENABLE);
  bEFBFastAccess = Config::Get(Config::GFX_HACK_EFB_FAST_ACCESS_ENABLE);
  iBBoxMode = Config::Get(Config::GFX_HACK_BBOX_MODE);
  bForceProgressive = Config::Get(Config::GFX_HACK_FORCE_PROGRESSIVE);
  bSkipEFBCopyToRam = Config::Get(Config::GFX_HACK_SKIP_EFB_COPY_TO_RAM);
  bCopyEFBScaled = Config::Get(Config::GFX_HACK_COPY_EFB_SCALED);
  iEFBScaledExcludeMin = Config::Get(Config::GFX_HACK_COPY_EFB_SCALED_EXCLUDE_MIN);
  iEFBScaledExcludeMax = Config::Get(Config::GFX_HACK_COPY_EFB_SCALED_EXCLUDE_MAX);
  bEFBEmulateFormatChanges = Config::Get(Config::GFX_HACK_EFB_EMULATE_FORMAT_CHANGES);
  bVertexRounding = Config::Get(Config::GFX_HACK_VERTEX_ROUDING);

  bForceDualSourceBlend = Config::Get(Config::GFX_HACK_FORCE_DUAL_SOURCE);
  bFullAsyncShaderCompilation = Config::Get(Config::GFX_HACK_FULL_ASYNC_SHADER_COMPILATION);
  bLastStoryEFBToRam = Config::Get(Config::GFX_HACK_LAST_HISTORY_EFBTORAM);
  bForceLogicOpBlend = Config::Get(Config::GFX_HACK_FORCE_LOGICOP_BLEND);

  bBackgroundShaderCompiling = Config::Get(Config::GFX_BACKGROUND_SHADER_COMPILING);
  bDisableSpecializedShaders = Config::Get(Config::GFX_DISABLE_SPECIALIZED_SHADERS);

  phack.m_enable = Config::Get(Config::GFX_PROJECTION_HACK) == 1;
  phack.m_sznear = Config::Get(Config::GFX_PROJECTION_HACK_SZNEAR) == 1;
  phack.m_szfar = Config::Get(Config::GFX_PROJECTION_HACK_SZFAR) == 1;
  phack.m_znear = Config::Get(Config::GFX_PROJECTION_HACK_ZNEAR);
  phack.m_zfar = Config::Get(Config::GFX_PROJECTION_HACK_ZFAR);
  bPerfQueriesEnable = Config::Get(Config::GFX_PERF_QUERIES_ENABLE);

  if (iEFBScale == SCALE_FORCE_INTEGRAL)
  {
    // Round down to multiple of native IR
    switch (Config::GetBase(Config::GFX_EFB_SCALE))
    {
    case SCALE_AUTO:
      iEFBScale = SCALE_AUTO_INTEGRAL;
      break;
    case SCALE_1_5X:
      iEFBScale = SCALE_1X;
      break;
    case SCALE_2_5X:
      iEFBScale = SCALE_2X;
      break;
    default:
      iEFBScale = Config::GetBase(Config::GFX_EFB_SCALE);
      break;
    }
  }

  VerifyValidity();
}

void VideoConfig::VerifyValidity()
{
  // Disable while is unstable
  bEnableOpenCL = false;
  // TODO: Check iMaxAnisotropy value
  if (iAdapter < 0 || iAdapter >((int)backend_info.Adapters.size() - 1))
    iAdapter = 0;
  if (std::find(backend_info.AAModes.begin(), backend_info.AAModes.end(), iMultisamples) == backend_info.AAModes.end())
    iMultisamples = 1;
  iMultisamples = std::max(iMultisamples, 1u);
  if (!backend_info.bSupportsPixelLighting) bEnablePixelLighting = false;
  bForcePhongShading = bForcePhongShading && bEnablePixelLighting;
  bHPFrameBuffer = bHPFrameBuffer && bForcePhongShading;
  bForcedLighting = bForcedLighting && bEnablePixelLighting;
  iRimPower = std::min(std::max(iRimPower, 0), 255);
  iRimIntesity = std::min(std::max(iRimIntesity, 0), 255);
  iRimBase = std::min(std::max(iRimBase, 0), 127);
  iSpecularMultiplier = std::min(std::max(iSpecularMultiplier, 0), 510);
  iSimBumpStrength = std::min(std::max(iSimBumpStrength, 0), 1024);
  iSimBumpDetailBlend = std::min(std::max(iSimBumpDetailBlend, 0), 255);
  iSimBumpDetailFrequency = std::min(std::max(iSimBumpDetailFrequency, 4), 255);
  iSimBumpThreshold = std::min(std::max(iSimBumpThreshold, 0), 255);
  iTessellationMax = iTessellationMax < 2 ? 2 : (iTessellationMax > 63 ? 63 : iTessellationMax);
  iTessellationRoundingIntensity = iTessellationRoundingIntensity > 100 ? 100 : (iTessellationRoundingIntensity < 0 ? 0 : iTessellationRoundingIntensity);
  iTessellationDisplacementIntensity = iTessellationDisplacementIntensity > 300 ? 300 : (iTessellationDisplacementIntensity < 0 ? 0 : iTessellationDisplacementIntensity);
  if (iStereoMode > 0)
  {
    if (!backend_info.bSupportsGeometryShaders)
    {
      OSD::AddMessage("Stereoscopic 3D isn't supported by your GPU, support for OpenGL 3.2 is required.", 10000);
      iStereoMode = 0;
    }
    if (bUseXFB && bUseRealXFB)
    {
      OSD::AddMessage("Stereoscopic 3D isn't supported with Real XFB, turning off stereoscopy.", 10000);
      iStereoMode = 0;
    }
  }
  if (iBBoxMode > BBoxGPU || iBBoxMode < BBoxNone)
  {
    iBBoxMode = BBoxNone;
  }
  if (backend_info.APIType & API_D3D9 && iBBoxMode == BBoxGPU)
  {
    iBBoxMode = BBoxCPU;
  }
  if (iTexScalingFactor < 2)
  {
    iTexScalingFactor = 2;
  }
  else if (iTexScalingFactor > 5)
  {
    iTexScalingFactor = 5;
  }
  if (iTexScalingType < 0)
  {
    iTexScalingType = 0;
  }
  else if (iTexScalingType > 10)
  {
    iTexScalingType = 10;
  }
  bHiresMaterialMaps = bHiresMaterialMaps && bHiresTextures && bEnablePixelLighting;
}

bool VideoConfig::IsVSync() const
{
  return bVSync && !Core::GetIsThrottlerTempDisabled();
}

bool VideoConfig::PixelLightingEnabled(const XFMemory& xfr, const u32 components) const
{
  return (xfr.numChan.numColorChans > 0) && bEnablePixelLighting && backend_info.bSupportsPixelLighting && ((components & VB_HAS_NRM0) == VB_HAS_NRM0);
}

bool VideoConfig::CanPrecompileUberShaders() const
{
  // We don't want to precompile ubershaders if they're never going to be used.
  return bBackgroundShaderCompiling || bDisableSpecializedShaders;
}
