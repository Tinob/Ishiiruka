// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Core/Config/GraphicsSettings.h"

#include <string>

#include "VideoCommon/VideoConfig.h"

namespace Config
{
// Configuration Information

// Graphics.Hardware

const ConfigInfo<bool> GFX_VSYNC{{System::GFX, "Hardware", "VSync"}, false};
const ConfigInfo<int> GFX_ADAPTER{{System::GFX, "Hardware", "Adapter"}, 0};

// Graphics.Settings

const ConfigInfo<bool> GFX_WIDESCREEN_HACK{{System::GFX, "Settings", "wideScreenHack"}, false};
const ConfigInfo<int> GFX_ASPECT_RATIO{{System::GFX, "Settings", "AspectRatio"},
                                       static_cast<int>(ASPECT_AUTO)};
const ConfigInfo<bool> GFX_CROP{{System::GFX, "Settings", "Crop"}, false};
const ConfigInfo<bool> GFX_USE_XFB{{System::GFX, "Settings", "UseXFB"}, false};
const ConfigInfo<bool> GFX_USE_REAL_XFB{{System::GFX, "Settings", "UseRealXFB"}, false};
const ConfigInfo<int> GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES{
    {System::GFX, "Settings", "SafeTextureCacheColorSamples"}, 128};
const ConfigInfo<bool> GFX_SHOW_FPS{{System::GFX, "Settings", "ShowFPS"}, false};
const ConfigInfo<bool> GFX_SHOW_NETPLAY_PING{{System::GFX, "Settings", "ShowNetPlayPing"}, false};
const ConfigInfo<bool> GFX_SHOW_NETPLAY_MESSAGES{{System::GFX, "Settings", "ShowNetPlayMessages"},
                                                 false};
const ConfigInfo<bool> GFX_LOG_RENDER_TIME_TO_FILE{{System::GFX, "Settings", "LogRenderTimeToFile"},
                                                   false};
const ConfigInfo<bool> GFX_OVERLAY_STATS{{System::GFX, "Settings", "OverlayStats"}, false};
const ConfigInfo<bool> GFX_OVERLAY_PROJ_STATS{{System::GFX, "Settings", "OverlayProjStats"}, false};
const ConfigInfo<bool> GFX_DUMP_TEXTURES{{System::GFX, "Settings", "DumpTextures"}, false};
const ConfigInfo<bool> GFX_HIRES_TEXTURES{{System::GFX, "Settings", "HiresTextures"}, false};
const ConfigInfo<bool> GFX_HIRES_MATERIAL_MAPS{ { System::GFX, "Settings", "HiresMaterialMaps" }, false };
const ConfigInfo<bool> GFX_HIRES_MATERIAL_MAPS_BUILD{ { System::GFX, "Settings", "HiresMaterialMapsBuild" }, false };
const ConfigInfo<bool> GFX_CONVERT_HIRES_TEXTURES{{System::GFX, "Settings", "ConvertHiresTextures"},
                                                  false};
const ConfigInfo<bool> GFX_CACHE_HIRES_TEXTURES{{System::GFX, "Settings", "CacheHiresTextures"},
                                                false};
const ConfigInfo<bool> GFX_DUMP_EFB_TARGET{{System::GFX, "Settings", "DumpEFBTarget"}, false};
const ConfigInfo<bool> GFX_DUMP_FRAMES_AS_IMAGES{{System::GFX, "Settings", "DumpFramesAsImages"},
                                                 false};
const ConfigInfo<bool> GFX_FREE_LOOK{{System::GFX, "Settings", "FreeLook"}, false};
const ConfigInfo<bool> GFX_COMPILE_SHADERS_ON_STARTUP{ { System::GFX, "Settings", "CompileShaderOnStartup" }, true };
const ConfigInfo<bool> GFX_USE_FFV1{{System::GFX, "Settings", "UseFFV1"}, false};
const ConfigInfo<std::string> GFX_DUMP_FORMAT{{System::GFX, "Settings", "DumpFormat"}, "avi"};
const ConfigInfo<std::string> GFX_DUMP_CODEC{{System::GFX, "Settings", "DumpCodec"}, ""};
const ConfigInfo<std::string> GFX_DUMP_PATH{{System::GFX, "Settings", "DumpPath"}, ""};
const ConfigInfo<int> GFX_BITRATE_KBPS{{System::GFX, "Settings", "BitrateKbps"}, 2500};
const ConfigInfo<bool> GFX_INTERNAL_RESOLUTION_FRAME_DUMPS{
    {System::GFX, "Settings", "InternalResolutionFrameDumps"}, false};
const ConfigInfo<bool> GFX_ENABLE_GPU_TEXTURE_DECODING{
    {System::GFX, "Settings", "EnableGPUTextureDecoding"}, false};
const ConfigInfo<bool> GFX_ENABLE_COMPUTE_TEXTURE_ENCODING{ { System::GFX, "Settings", "EnableComputeTextureEncoding" }, false };
const ConfigInfo<bool> GFX_ENABLE_PIXEL_LIGHTING{{System::GFX, "Settings", "EnablePixelLighting"},
                                                 false};
const ConfigInfo<bool> GFX_FORCED_LIGHTING{ { System::GFX, "Settings", "ForcedLighting" },
false };
const ConfigInfo<bool> GFX_FORCE_PHONG_SHADING{ { System::GFX, "Settings", "ForcePhongShading" },
                                                 false };
const ConfigInfo<int> GFX_RIM_POWER{ { System::GFX, "Settings", "RimPower" }, 80 };
const ConfigInfo<int> GFX_RIM_INTENSITY{ { System::GFX, "Settings", "RimIntesity" }, 0 };
const ConfigInfo<int> GFX_RIM_BASE{ { System::GFX, "Settings", "RimBase" }, 10 };
const ConfigInfo<int> GFX_SPECULAR_MULTIPLIER{ { System::GFX, "Settings", "SpecularMultiplier" }, 255 };

const ConfigInfo<bool> GFX_SIMULATE_BUMP_MAPPING{ { System::GFX, "Settings", "SimBumpEnabled" },
                                                 false };
const ConfigInfo<int> GFX_SIMULATE_BUMP_STRENGTH{ { System::GFX, "Settings", "SimBumpStrength" }, 0 };
const ConfigInfo<int> GFX_SIMULATE_BUMP_DETAIL_FREQUENCY{ { System::GFX, "Settings", "SimBumpDetailFrequency" }, 128 };
const ConfigInfo<int> GFX_SIMULATE_BUMP_THRESHOLD{ { System::GFX, "Settings", "SimBumpThreshold" }, 16 };
const ConfigInfo<int> GFX_SIMULATE_BUMP_DETAIL_BLEND{ { System::GFX, "Settings", "SimBumpDetailBlend" }, 16 };

const ConfigInfo<bool> GFX_FAST_DEPTH_CALC{{System::GFX, "Settings", "FastDepthCalc"}, true};
const ConfigInfo<u32> GFX_MSAA{{System::GFX, "Settings", "MSAA"}, 1};
const ConfigInfo<bool> GFX_SSAA{{System::GFX, "Settings", "SSAA"}, false};
const ConfigInfo<int> GFX_EFB_SCALE{{System::GFX, "Settings", "EFBScale"},
                                    static_cast<int>(SCALE_1X)};
const ConfigInfo<bool> GFX_TEXFMT_OVERLAY_ENABLE{{System::GFX, "Settings", "TexFmtOverlayEnable"},
                                                 false};
const ConfigInfo<bool> GFX_TEXFMT_OVERLAY_CENTER{{System::GFX, "Settings", "TexFmtOverlayCenter"},
                                                 false};
const ConfigInfo<bool> GFX_ENABLE_WIREFRAME{{System::GFX, "Settings", "WireFrame"}, false};
const ConfigInfo<bool> GFX_DISABLE_FOG{{System::GFX, "Settings", "DisableFog"}, false};
const ConfigInfo<bool> GFX_BORDERLESS_FULLSCREEN{{System::GFX, "Settings", "BorderlessFullscreen"},
                                                 false};
const ConfigInfo<bool> GFX_ENABLE_VALIDATION_LAYER{
    {System::GFX, "Settings", "EnableValidationLayer"}, false};
const ConfigInfo<bool> GFX_BACKEND_MULTITHREADING{
    {System::GFX, "Settings", "BackendMultithreading"}, true};
const ConfigInfo<int> GFX_COMMAND_BUFFER_EXECUTE_INTERVAL{
    {System::GFX, "Settings", "CommandBufferExecuteInterval"}, 100};
const ConfigInfo<bool> GFX_SHADER_CACHE{{System::GFX, "Settings", "ShaderCache"}, true};

const ConfigInfo<bool> GFX_SW_ZCOMPLOC{{System::GFX, "Settings", "SWZComploc"}, true};
const ConfigInfo<bool> GFX_SW_ZFREEZE{{System::GFX, "Settings", "SWZFreeze"}, true};
const ConfigInfo<bool> GFX_SW_DUMP_OBJECTS{{System::GFX, "Settings", "SWDumpObjects"}, false};
const ConfigInfo<bool> GFX_SW_DUMP_TEV_STAGES{{System::GFX, "Settings", "SWDumpTevStages"}, false};
const ConfigInfo<bool> GFX_SW_DUMP_TEV_TEX_FETCHES{{System::GFX, "Settings", "SWDumpTevTexFetches"},
                                                   false};
const ConfigInfo<int> GFX_SW_DRAW_START{{System::GFX, "Settings", "SWDrawStart"}, 0};
const ConfigInfo<int> GFX_SW_DRAW_END{{System::GFX, "Settings", "SWDrawEnd"}, 100000};

// Graphics.Enhancements

const ConfigInfo<bool> GFX_ENHANCE_FORCE_FILTERING{{System::GFX, "Enhancements", "ForceFiltering"},
                                                   false};
const ConfigInfo<bool> GFX_ENHANCE_DISABLE_FILTERING{ { System::GFX, "Enhancements", "DisableFiltering" },
false };
const ConfigInfo<int> GFX_ENHANCE_MAX_ANISOTROPY{{System::GFX, "Enhancements", "MaxAnisotropy"}, 0};
const ConfigInfo<bool> GFX_ENHANCE_POST_ENABLED{
    {System::GFX, "Enhancements", "PostProcessingEnable"}, false};
const ConfigInfo<int> GFX_ENHANCE_POST_TRIGUER{ { System::GFX, "Enhancements", "PostProcessingTrigger" }, 0 };
const ConfigInfo<std::string> GFX_ENHANCE_POST_SHADERS{
  { System::GFX, "Enhancements", "PostProcessingShaders" }, "" };
const ConfigInfo<std::string> GFX_ENHANCE_SCALING_SHADER{
  { System::GFX, "Enhancements", "ScalingShader" }, "" };

const ConfigInfo<bool> GFX_ENHANCE_FORCE_TRUE_COLOR{{System::GFX, "Enhancements", "ForceTrueColor"},
                                                    true};
const ConfigInfo<bool> GFX_ENHANCE_USE_SCALING_FILTER{ { System::GFX, "Enhancements", "UseScalingFilter" },
true };

const ConfigInfo<int> GFX_ENHANCE_TEXTURE_SCALING_TYPE{ { System::GFX, "Enhancements", "TextureScalingType" }, 0 };
const ConfigInfo<int> GFX_ENHANCE_TEXTURE_SCALING_FACTOR{ { System::GFX, "Enhancements", "TextureScalingFactor" }, 2 };
const ConfigInfo<bool> GFX_ENHANCE_USE_DEPOSTERIZE{ { System::GFX, "Enhancements", "UseDePosterize" },
true };

const ConfigInfo<bool> GFX_ENHANCE_TESSELLATION{ { System::GFX, "Enhancements", "Tessellation" }, true };
const ConfigInfo<bool> GFX_ENHANCE_TESSELLATION_EARLY_CULLING{ { System::GFX, "Enhancements", "TessellationEarlyCulling" }, false };
const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_DISTANCE{ { System::GFX, "Enhancements", "TessellationDistance" }, 0 };
const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_MAX{ { System::GFX, "Enhancements", "TessellationMax" }, 6 };
const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_ROUNDING_INTENSITY{ { System::GFX, "Enhancements", "TessellationRoundingIntensity" }, 0 };
const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_DISPLACEMENT_INTENSITY{ { System::GFX, "Enhancements", "TessellationDisplacementIntensity" }, 0 };
// Graphics.Stereoscopy

const ConfigInfo<int> GFX_STEREO_MODE{{System::GFX, "Stereoscopy", "StereoMode"}, 0};
const ConfigInfo<int> GFX_STEREO_DEPTH{{System::GFX, "Stereoscopy", "StereoDepth"}, 20};
const ConfigInfo<int> GFX_STEREO_CONVERGENCE_PERCENTAGE{
    {System::GFX, "Stereoscopy", "StereoConvergencePercentage"}, 100};
const ConfigInfo<bool> GFX_STEREO_SWAP_EYES{{System::GFX, "Stereoscopy", "StereoSwapEyes"}, false};
const ConfigInfo<int> GFX_STEREO_CONVERGENCE{{System::GFX, "Stereoscopy", "StereoConvergence"}, 20};
const ConfigInfo<bool> GFX_STEREO_EFB_MONO_DEPTH{{System::GFX, "Stereoscopy", "StereoEFBMonoDepth"},
                                                 false};
const ConfigInfo<int> GFX_STEREO_DEPTH_PERCENTAGE{
    {System::GFX, "Stereoscopy", "StereoDepthPercentage"}, 100};
const ConfigInfo<std::string> GFX_STEREO_SHADER{
  { System::GFX, "Stereoscopy", "StereoShader" }, std::string("Anaglyph/dubois")};

// Graphics.Hacks

const ConfigInfo<bool> GFX_HACK_EFB_ACCESS_ENABLE{{System::GFX, "Hacks", "EFBAccessEnable"}, true};
const ConfigInfo<bool> GFX_HACK_EFB_FAST_ACCESS_ENABLE{ { System::GFX, "Hacks", "EFBFastAccess" }, false };
const ConfigInfo<int> GFX_HACK_BBOX_MODE{{System::GFX, "Hacks", "BoundingBoxMode"}, 0};
const ConfigInfo<bool> GFX_HACK_FORCE_PROGRESSIVE{{System::GFX, "Hacks", "ForceProgressive"}, true};
const ConfigInfo<bool> GFX_HACK_SKIP_EFB_COPY_TO_RAM{{System::GFX, "Hacks", "EFBToTextureEnable"},
                                                     true};
const ConfigInfo<bool> GFX_HACK_COPY_EFB_ENABLED{{System::GFX, "Hacks", "EFBScaledCopy"}, true};
const ConfigInfo<bool> GFX_HACK_EFB_EMULATE_FORMAT_CHANGES{
    {System::GFX, "Hacks", "EFBEmulateFormatChanges"}, false};
const ConfigInfo<bool> GFX_HACK_VERTEX_ROUDING{{System::GFX, "Hacks", "VertexRounding"}, false};

const ConfigInfo<bool> GFX_HACK_FORCE_DUAL_SOURCE{ { System::GFX, "Hacks", "ForceDualSourceBlend" }, false };
const ConfigInfo<bool> GFX_HACK_FULL_ASYNC_SHADER_COMPILATION{ { System::GFX, "Hacks", "FullAsyncShaderCompilation" }, false };
const ConfigInfo<bool> GFX_HACK_WAIT_FOR_SHADER_COMPILATION{ { System::GFX, "Hacks", "WaitForShaderCompilation" }, false };
const ConfigInfo<bool> GFX_HACK_LAST_HISTORY_EFBTORAM{ { System::GFX, "Hacks", "LastStoryEFBToRam" }, false };
const ConfigInfo<bool> GFX_HACK_FORCE_LOGICOP_BLEND{ { System::GFX, "Hacks", "ForceLogicOpBlend" }, false };

// Graphics.GameSpecific

const ConfigInfo<int> GFX_PROJECTION_HACK{{System::GFX, "GameSpecific", "ProjectionHack"}, 0};
const ConfigInfo<int> GFX_PROJECTION_HACK_SZNEAR{{System::GFX, "GameSpecific", "PH_SZNear"}, 0};
const ConfigInfo<int> GFX_PROJECTION_HACK_SZFAR{{System::GFX, "GameSpecific", "PH_SZFar"}, 0};
const ConfigInfo<std::string> GFX_PROJECTION_HACK_ZNEAR{{System::GFX, "GameSpecific", "PH_ZNear"},
                                                        ""};
const ConfigInfo<std::string> GFX_PROJECTION_HACK_ZFAR{{System::GFX, "GameSpecific", "PH_ZFar"},
                                                       ""};
const ConfigInfo<bool> GFX_PERF_QUERIES_ENABLE{{System::GFX, "GameSpecific", "PerfQueriesEnable"},
                                               false};
}  // namespace Config
