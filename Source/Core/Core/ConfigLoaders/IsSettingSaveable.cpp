// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Core/ConfigLoaders/IsSettingSaveable.h"

#include <algorithm>
#include <vector>

#include "Common/Config/Config.h"
#include "Core/Config/GraphicsSettings.h"

namespace ConfigLoaders
{
bool IsSettingSaveable(const Config::ConfigLocation& config_location)
{
  if (config_location.system == Config::System::Logger)
    return true;

  if (config_location.system == Config::System::Main && config_location.section == "NetPlay")
    return true;

  const static std::vector<Config::ConfigLocation> s_setting_saveable{
      // Graphics.Hardware

      Config::GFX_VSYNC.location,
      Config::GFX_ADAPTER.location,

      // Graphics.Settings

      Config::GFX_WIDESCREEN_HACK.location,
      Config::GFX_ASPECT_RATIO.location,
      Config::GFX_CROP.location,
      Config::GFX_USE_XFB.location,
      Config::GFX_USE_REAL_XFB.location,
      Config::GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES.location,
      Config::GFX_SHOW_FPS.location,
      Config::GFX_SHOW_NETPLAY_PING.location,
      Config::GFX_SHOW_NETPLAY_MESSAGES.location,
      Config::GFX_LOG_RENDER_TIME_TO_FILE.location,
      Config::GFX_OVERLAY_STATS.location,
      Config::GFX_OVERLAY_PROJ_STATS.location,
      Config::GFX_DUMP_TEXTURES.location,
      Config::GFX_HIRES_TEXTURES.location,
      Config::GFX_HIRES_MATERIAL_MAPS.location,
      Config::GFX_HIRES_MATERIAL_MAPS_BUILD.location,
      Config::GFX_CACHE_HIRES_TEXTURES.location,
      Config::GFX_WAIT_CACHE_HIRES_TEXTURES.location,
      Config::GFX_DUMP_EFB_TARGET.location,
      Config::GFX_DUMP_FRAMES_AS_IMAGES.location,
      Config::GFX_FREE_LOOK.location,
      Config::GFX_COMPILE_SHADERS_ON_STARTUP.location,
      Config::GFX_USE_FFV1.location,
      Config::GFX_DUMP_FORMAT.location,
      Config::GFX_DUMP_CODEC.location,
      Config::GFX_DUMP_PATH.location,
      Config::GFX_BITRATE_KBPS.location,
      Config::GFX_INTERNAL_RESOLUTION_FRAME_DUMPS.location,
      Config::GFX_ENABLE_GPU_TEXTURE_DECODING.location,
      Config::GFX_ENABLE_COMPUTE_TEXTURE_ENCODING.location,
      Config::GFX_ENABLE_PIXEL_LIGHTING.location,
      Config::GFX_FORCED_LIGHTING.location,
      Config::GFX_FORCED_DITHERING.location,
      Config::GFX_FORCE_PHONG_SHADING.location,
      Config::GFX_RIM_POWER.location,
      Config::GFX_RIM_INTENSITY.location,
      Config::GFX_RIM_BASE.location,
      Config::GFX_SPECULAR_MULTIPLIER.location,
      Config::GFX_SIMULATE_BUMP_MAPPING.location,
      Config::GFX_SIMULATE_BUMP_STRENGTH.location,
      Config::GFX_SIMULATE_BUMP_DETAIL_FREQUENCY.location,
      Config::GFX_SIMULATE_BUMP_THRESHOLD.location,
      Config::GFX_SIMULATE_BUMP_DETAIL_BLEND.location,
      Config::GFX_FAST_DEPTH_CALC.location,
      Config::GFX_MSAA.location,
      Config::GFX_SSAA.location,
      Config::GFX_EFB_SCALE.location,
      Config::GFX_TEXFMT_OVERLAY_ENABLE.location,
      Config::GFX_TEXFMT_OVERLAY_CENTER.location,
      Config::GFX_ENABLE_WIREFRAME.location,
      Config::GFX_DISABLE_FOG.location,
      Config::GFX_BORDERLESS_FULLSCREEN.location,
      Config::GFX_ENABLE_VALIDATION_LAYER.location,
      Config::GFX_ENABLE_SHADER_DEBUG.location,
      Config::GFX_BACKEND_MULTITHREADING.location,
      Config::GFX_COMMAND_BUFFER_EXECUTE_INTERVAL.location,
      Config::GFX_SHADER_CACHE.location,

      Config::GFX_SW_ZCOMPLOC.location,
      Config::GFX_SW_ZFREEZE.location,
      Config::GFX_SW_DUMP_OBJECTS.location,
      Config::GFX_SW_DUMP_TEV_STAGES.location,
      Config::GFX_SW_DUMP_TEV_TEX_FETCHES.location,
      Config::GFX_SW_DRAW_START.location,
      Config::GFX_SW_DRAW_END.location,
      Config::GFX_BACKGROUND_SHADER_COMPILING.location,
      Config::GFX_DISABLE_SPECIALIZED_SHADERS.location,
      // Graphics.Enhancements

      Config::GFX_ENHANCE_FILTERING_MODE.location,
      Config::GFX_ENHANCE_MAX_ANISOTROPY.location,
      Config::GFX_ENHANCE_POST_ENABLED.location,
      Config::GFX_ENHANCE_POST_TRIGUER.location,
      Config::GFX_ENHANCE_POST_SHADERS.location,
      Config::GFX_ENHANCE_SCALING_SHADER.location,
      Config::GFX_ENHANCE_FORCE_TRUE_COLOR.location,
      Config::GFX_ENHANCE_HP_FRAME_BUFFER.location,
      Config::GFX_ENHANCE_USE_SCALING_FILTER.location,
      Config::GFX_ENHANCE_TEXTURE_SCALING_TYPE.location,
      Config::GFX_ENHANCE_TEXTURE_SCALING_FACTOR.location,
      Config::GFX_ENHANCE_USE_DEPOSTERIZE.location,
      Config::GFX_ENHANCE_TESSELLATION.location,
      Config::GFX_ENHANCE_TESSELLATION_EARLY_CULLING.location,
      Config::GFX_ENHANCE_TESSELLATION_DISTANCE.location,
      Config::GFX_ENHANCE_TESSELLATION_MAX.location,
      Config::GFX_ENHANCE_TESSELLATION_ROUNDING_INTENSITY.location,
      Config::GFX_ENHANCE_TESSELLATION_DISPLACEMENT_INTENSITY.location,

      // Graphics.Stereoscopy

      Config::GFX_STEREO_MODE.location,
      Config::GFX_STEREO_DEPTH.location,
      Config::GFX_STEREO_CONVERGENCE_PERCENTAGE.location,
      Config::GFX_STEREO_SWAP_EYES.location,
      Config::GFX_STEREO_CONVERGENCE.location,
      Config::GFX_STEREO_EFB_MONO_DEPTH.location,
      Config::GFX_STEREO_DEPTH_PERCENTAGE.location,

      // Graphics.Hacks

      Config::GFX_HACK_EFB_ACCESS_ENABLE.location,
      Config::GFX_HACK_EFB_FAST_ACCESS_ENABLE.location,
      Config::GFX_HACK_BBOX_MODE.location,
      Config::GFX_HACK_FORCE_PROGRESSIVE.location,
      Config::GFX_HACK_SKIP_EFB_COPY_TO_RAM.location,
      Config::GFX_HACK_COPY_EFB_SCALED.location,
      Config::GFX_HACK_EFB_EMULATE_FORMAT_CHANGES.location,
      Config::GFX_HACK_VERTEX_ROUDING.location,
      Config::GFX_HACK_FORCE_DUAL_SOURCE.location,
      Config::GFX_HACK_FULL_ASYNC_SHADER_COMPILATION.location,
      Config::GFX_HACK_LAST_HISTORY_EFBTORAM.location,
      Config::GFX_HACK_FORCE_LOGICOP_BLEND.location,
      Config::GFX_HACK_CULL_MODE.location,

      // Graphics.GameSpecific

      Config::GFX_PROJECTION_HACK.location,
      Config::GFX_PROJECTION_HACK_SZNEAR.location,
      Config::GFX_PROJECTION_HACK_SZFAR.location,
      Config::GFX_PROJECTION_HACK_ZNEAR.location,
      Config::GFX_PROJECTION_HACK_ZFAR.location,
      Config::GFX_PERF_QUERIES_ENABLE.location,

  };

  return std::find(s_setting_saveable.begin(), s_setting_saveable.end(), config_location) !=
         s_setting_saveable.end();
}
}  // namespace ConfigLoaders
