// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.


// IMPORTANT: UI etc should modify g_Config. Graphics code should read g_ActiveConfig.
// The reason for this is to get rid of race conditions etc when the configuration
// changes in the middle of a frame. This is done by copying g_Config to g_ActiveConfig
// at the start of every frame. Noone should ever change members of g_ActiveConfig
// directly.

#pragma once

#include <string>
#include <vector>

#include "Common/CommonTypes.h"
#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/XFMemory.h"


// Log in two categories, and save three other options in the same byte
#define CONF_LOG          1
#define CONF_PRIMLOG      2
#define CONF_SAVETARGETS  8
#define CONF_SAVESHADERS  16

enum AspectMode
{
  ASPECT_AUTO = 0,
  ASPECT_ANALOG_WIDE = 1,
  ASPECT_ANALOG = 2,
  ASPECT_STRETCH = 3,
  ASPECT_4_3 = 4,
  ASPECT_16_9 = 5,
  ASPECT_16_10 = 6,
};

enum EFBScale
{
  SCALE_FORCE_INTEGRAL = -1,
  SCALE_AUTO,
  SCALE_AUTO_INTEGRAL,
  SCALE_1X,
  SCALE_1_5X,
  SCALE_2X,
  SCALE_2_5X,
  SCALE_3X,
  SCALE_4X,
  SCALE_5X,
  SCALE_6X,
  SCALE_7X,
  SCALE_8X
};

enum StereoMode
{
  STEREO_OFF = 0,
  STEREO_SBS,
  STEREO_TAB,
  STEREO_SHADER,
  STEREO_3DVISION,
};

enum BBoxMode : s32
{
  BBoxNone = 0,
  BBoxCPU = 1,
  BBoxGPU = 2
};

enum FilteringMode : s32
{
  Disabled = 0,
  Accurate = 1,
  Normal = 2,
  Forced = 3,
};

enum HostCullMode : s32
{
  Native = 0,
  NoCull = 1,
  NoCullExceptAll = 2,
  FrontNoBlending = 3,
  Front = 4,
  BackNoBlending = 5,
  Back = 6,
};

struct ProjectionHackConfig final
{
  bool m_enable;
  bool m_sznear;
  bool m_szfar;
  std::string m_znear;
  std::string m_zfar;
};

// NEVER inherit from this class.
struct VideoConfig final
{
  VideoConfig();
  void ClearFormats();
  void Refresh();
  void VerifyValidity();
  void UpdateProjectionHack();
  bool IsVSync() const;
  bool PixelLightingEnabled(const XFMemory& xfr, const u32 components) const;
  bool CanPrecompileUberShaders() const;

  // General
  bool bVSync;
  bool bWidescreenHack;
  int iAspectRatio;
  bool bCrop;   // Aspect ratio controls.
  bool bUseXFB;
  bool bUseRealXFB;
  bool bBlackFrameInsertion;
  // OpenCL/OpenMP
  bool bEnableOpenCL;
  bool bOMPDecoder;

  // Enhancements
  u32 iMultisamples;
  bool bSSAA;
  int iEFBScale;
  FilteringMode eFilteringMode;
  HostCullMode eCullMode;
  int iMaxAnisotropy;
  bool bPostProcessingEnable;
  int iPostProcessingTrigger;
  bool bPostProcessingEfbMustBePerspective;
  bool bPostProcessingEfbMustBeAspect;
  bool bPostProcessingEfbFailsafe;
  int iPostProcessingEfbMinResolutionPercent;
  int iPostProcessingEfbIndex;
  std::string sPostProcessingShaders;
  std::string sScalingShader;
  std::string sStereoShader;
  bool bUseScalingFilter;
  bool bTexDeposterize;
  int iTexScalingType;
  int iTexScalingFactor;
  bool bTessellation;
  bool bTessellationEarlyCulling;
  int iTessellationDistance;
  int iTessellationMax;
  int iTessellationRoundingIntensity;
  int iTessellationDisplacementIntensity;
  bool bForceTrueColor;
  bool bHPFrameBuffer;
  // Information
  bool bShowFPS;
  bool bShowNetPlayPing;
  bool bShowNetPlayMessages;
  bool bShowInputDisplay;
  bool bOverlayStats;
  bool bOverlayProjStats;
  bool bTexFmtOverlayEnable;
  bool bTexFmtOverlayCenter;
  bool bLogRenderTimeToFile;


  // Render
  bool bWireFrame;
  bool bDisableFog;

  // Utility
  bool bDumpTextures;  
  bool bHiresTextures;
  bool bHiresMaterialMaps;
  bool bHiresMaterialMapsBuild;
  bool bCacheHiresTextures;
  bool bWaitForCacheHiresTextures;
  bool bDumpEFBTarget;
  bool bDumpFramesAsImages;
  bool bUseFFV1;
  std::string sDumpCodec;
  std::string sDumpFormat;
  std::string sDumpPath;
  bool bInternalResolutionFrameDumps;
  bool bFreeLook;
  bool bBorderlessFullscreen;
  int iBitrateKbps;
  bool bCompileShaderOnStartup;


  // Hacks
  bool bEFBAccessEnable;
  bool bEFBFastAccess;
  bool bForceProgressive;
  bool bPerfQueriesEnable;
  bool bFullAsyncShaderCompilation;
  bool bEnableGPUTextureDecoding;
  bool bEnableComputeTextureEncoding;
  bool bEFBEmulateFormatChanges;
  bool bSkipEFBCopyToRam;
  bool bCopyEFBScaled;
  int iEFBScaledExcludeMin;
  int iEFBScaledExcludeMax;
  int iSafeTextureCache_ColorSamples;
  ProjectionHackConfig phack;
  float fAspectRatioHackW, fAspectRatioHackH;
  bool bEnablePixelLighting;
  bool bForcedLighting;
  bool bForcePhongShading;
  int iRimPower;
  int iRimIntesity;
  int iRimBase;
  int iSpecularMultiplier;
  bool bLastStoryEFBToRam;
  bool bForceLogicOpBlend;
  bool bForcedDithering;
  bool bSimBumpEnabled;
  int iSimBumpDetailBlend;
  int iSimBumpDetailFrequency;
  int iSimBumpThreshold;
  int iSimBumpStrength;

  bool bFastDepthCalc;
  bool bVertexRounding;
  int iBBoxMode;
  //for dx9-backend
  bool bForceDualSourceBlend;
  int iLog; // CONF_ bits
  int iSaveTargetId; // TODO: Should be dropped

  // Stereoscopy
  int iStereoMode;
  int iStereoDepth;
  int iStereoConvergence;
  int iStereoConvergencePercentage;
  bool bStereoSwapEyes;
  bool bStereoEFBMonoDepth;
  int iStereoDepthPercentage;

  // D3D only config, mostly to be merged into the above
  int iAdapter;

  // VideoSW Debugging
  int drawStart;
  int drawEnd;
  bool bZComploc;
  bool bZFreeze;
  bool bDumpObjects;
  bool bDumpTevStages;
  bool bDumpTevTextureFetches;

  bool bEnableValidationLayer;
  bool bEnableShaderDebug;

  // Multithreaded submission, currently only supported with Vulkan.
  bool bBackendMultithreading;

  // Early command buffer execution interval in number of draws.
  // Currently only supported with Vulkan.
  int iCommandBufferExecuteInterval;

  // The following options determine the ubershader mode:
  //   No ubershaders:
  //     - bBackgroundShaderCompiling = false
  //     - bDisableSpecializedShaders = false
  //   Hybrid/background compiling:
  //     - bBackgroundShaderCompiling = true
  //     - bDisableSpecializedShaders = false
  //   Ubershaders only:
  //     - bBackgroundShaderCompiling = false
  //     - bDisableSpecializedShaders = true

  // Enable background shader compiling, use ubershaders while waiting.
  bool bBackgroundShaderCompiling;

  // Use ubershaders only, don't compile specialized shaders.
  bool bDisableSpecializedShaders;
  
  // Static config per API
  // TODO: Move this out of VideoConfig
  struct
  {
    API_TYPE APIType;

    std::vector<std::string> Adapters; // for D3D9 and D3D11
    std::vector<u32> AAModes;

    // TODO: merge AdapterName and Adapters array
    std::string AdapterName; // for OpenGL

    u32 MaxTextureSize;

    bool bSupportedFormats[HostTextureFormat::PC_TEX_NUM_FORMATS]; // used for D3D9 in TextureCache		
    bool bSupportsDualSourceBlend; // only supported by D3D11 and OpenGL
    bool bSupportsPixelLighting;
    bool bSupportsNormalMaps;
    bool bSupportsSeparateAlphaFunction;
    bool bSupportsBindingLayout; // needed by PixelShaderGen, so must stay in VideoCommon
    bool bSupportsEarlyZ; // needed by PixelShaderGen, so must stay in VideoCommon
    bool bNeedBlendIndices; // needed by PixelShaderGen, so must stay in VideoCommon
    bool bSupportsOversizedViewports;
    bool bSupportsPostProcessing;
    bool bSupportsGeometryShaders;
    bool bSupportsComputeShaders;
    bool bSupports3DVision;
    bool bSupportsExclusiveFullscreen;
    bool bSupportsBBox;
    bool bSupportsGSInstancing; // Needed by GeometryShaderGen, so must stay in VideoCommon
    bool bSupportsPaletteConversion;
    bool bSupportsClipControl; // Needed by VertexShaderGen, so must stay in VideoCommon		
    bool bSupportsSSAA;
    bool bSupportsTessellation;
    bool bSupportsScaling;
    bool bSupportsDepthClamp;  // Needed by VertexShaderGen, so must stay in VideoCommon
    bool bSupportsGPUTextureDecoding;
    bool bSupportsComputeTextureEncoding;
    bool bSupportsMultithreading;
    bool bSupportsValidationLayer;
    bool bSupportsReversedDepthRange;
    bool bSupportsInternalResolutionFrameDumps;
    bool bSupportsAsyncShaderCompilation;
    bool bSupportsFragmentStoresAndAtomics;  // a.k.a. OpenGL SSBOs a.k.a. Direct3D UAVs
    bool bSupportsBitfield;                // Needed by UberShaders, so must stay in VideoCommon
    bool bSupportsDynamicSamplerIndexing;  // Needed by UberShaders, so must stay in VideoCommon
    bool bSupportsUberShaders;
    bool bSupportsHighPrecisionFrameBuffer;
  } backend_info;

  // Utility
  inline bool RealXFBEnabled() const
  {
    return bUseXFB && bUseRealXFB;
  }
  inline bool VirtualXFBEnabled() const
  {
    return bUseXFB && !bUseRealXFB;
  }
  inline bool MultisamplingEnabled() const { return iMultisamples > 1; }
  inline bool ExclusiveFullscreenEnabled() const
  {
    return backend_info.bSupportsExclusiveFullscreen && !bBorderlessFullscreen;
  }
  inline bool HiresMaterialMapsEnabled() const
  {
    return backend_info.bSupportsNormalMaps && bHiresTextures && bHiresMaterialMaps;
  }
  inline bool TessellationEnabled() const
  {
    return backend_info.bSupportsTessellation && bTessellation && bEnablePixelLighting;
  }
  inline bool UseGPUTextureDecoding() const
  {
    return backend_info.bSupportsGPUTextureDecoding && bEnableGPUTextureDecoding;
  }
  inline bool UseHPFrameBuffer()
  {
    return backend_info.bSupportsHighPrecisionFrameBuffer && bHPFrameBuffer;
  }
  inline bool UseVertexRounding() const { return bVertexRounding && iEFBScale != SCALE_1X; }
};

extern VideoConfig g_Config;
extern VideoConfig g_ActiveConfig;

// Called every frame.
void UpdateActiveConfig();
