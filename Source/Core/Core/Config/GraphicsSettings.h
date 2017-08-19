// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <string>
#include "Common/CommonTypes.h"
#include "Common/Config/Config.h"

namespace Config
{
// Configuration Information

// Graphics.Hardware

extern const ConfigInfo<bool> GFX_VSYNC;
extern const ConfigInfo<int> GFX_ADAPTER;

// Graphics.Settings

extern const ConfigInfo<bool> GFX_WIDESCREEN_HACK;
extern const ConfigInfo<int> GFX_ASPECT_RATIO;
extern const ConfigInfo<int> GFX_SUGGESTED_ASPECT_RATIO;
extern const ConfigInfo<bool> GFX_CROP;
extern const ConfigInfo<bool> GFX_USE_XFB;
extern const ConfigInfo<bool> GFX_USE_REAL_XFB;
extern const ConfigInfo<int> GFX_SAFE_TEXTURE_CACHE_COLOR_SAMPLES;
extern const ConfigInfo<bool> GFX_SHOW_FPS;
extern const ConfigInfo<bool> GFX_SHOW_NETPLAY_PING;
extern const ConfigInfo<bool> GFX_SHOW_NETPLAY_MESSAGES;
extern const ConfigInfo<bool> GFX_LOG_RENDER_TIME_TO_FILE;
extern const ConfigInfo<bool> GFX_OVERLAY_STATS;
extern const ConfigInfo<bool> GFX_OVERLAY_PROJ_STATS;
extern const ConfigInfo<bool> GFX_DUMP_TEXTURES;
extern const ConfigInfo<bool> GFX_HIRES_TEXTURES;
extern const ConfigInfo<bool> GFX_HIRES_MATERIAL_MAPS;
extern const ConfigInfo<bool> GFX_HIRES_MATERIAL_MAPS_BUILD;
extern const ConfigInfo<bool> GFX_CONVERT_HIRES_TEXTURES;
extern const ConfigInfo<bool> GFX_CACHE_HIRES_TEXTURES;
extern const ConfigInfo<bool> GFX_DUMP_EFB_TARGET;
extern const ConfigInfo<bool> GFX_DUMP_FRAMES_AS_IMAGES;
extern const ConfigInfo<bool> GFX_FREE_LOOK;
extern const ConfigInfo<bool> GFX_COMPILE_SHADERS_ON_STARTUP;
extern const ConfigInfo<bool> GFX_USE_FFV1;
extern const ConfigInfo<std::string> GFX_DUMP_FORMAT;
extern const ConfigInfo<std::string> GFX_DUMP_CODEC;
extern const ConfigInfo<std::string> GFX_DUMP_PATH;
extern const ConfigInfo<int> GFX_BITRATE_KBPS;
extern const ConfigInfo<bool> GFX_INTERNAL_RESOLUTION_FRAME_DUMPS;
extern const ConfigInfo<bool> GFX_ENABLE_GPU_TEXTURE_DECODING;
extern const ConfigInfo<bool> GFX_ENABLE_COMPUTE_TEXTURE_ENCODING;
extern const ConfigInfo<bool> GFX_ENABLE_PIXEL_LIGHTING;
extern const ConfigInfo<bool> GFX_FORCED_LIGHTING;

extern const ConfigInfo<bool> GFX_FORCE_PHONG_SHADING;
extern const ConfigInfo<int> GFX_RIM_POWER;
extern const ConfigInfo<int> GFX_RIM_INTENSITY;
extern const ConfigInfo<int> GFX_RIM_BASE;
extern const ConfigInfo<int> GFX_SPECULAR_MULTIPLIER;

extern const ConfigInfo<bool> GFX_SIMULATE_BUMP_MAPPING;
extern const ConfigInfo<int> GFX_SIMULATE_BUMP_STRENGTH;
extern const ConfigInfo<int> GFX_SIMULATE_BUMP_DETAIL_FREQUENCY;
extern const ConfigInfo<int> GFX_SIMULATE_BUMP_THRESHOLD;
extern const ConfigInfo<int> GFX_SIMULATE_BUMP_DETAIL_BLEND;

extern const ConfigInfo<bool> GFX_FAST_DEPTH_CALC;
extern const ConfigInfo<u32> GFX_MSAA;
extern const ConfigInfo<bool> GFX_SSAA;
extern const ConfigInfo<int> GFX_EFB_SCALE;
extern const ConfigInfo<bool> GFX_TEXFMT_OVERLAY_ENABLE;
extern const ConfigInfo<bool> GFX_TEXFMT_OVERLAY_CENTER;
extern const ConfigInfo<bool> GFX_ENABLE_WIREFRAME;
extern const ConfigInfo<bool> GFX_DISABLE_FOG;
extern const ConfigInfo<bool> GFX_BORDERLESS_FULLSCREEN;
extern const ConfigInfo<bool> GFX_ENABLE_VALIDATION_LAYER;
extern const ConfigInfo<bool> GFX_BACKEND_MULTITHREADING;
extern const ConfigInfo<int> GFX_COMMAND_BUFFER_EXECUTE_INTERVAL;
extern const ConfigInfo<bool> GFX_SHADER_CACHE;

extern const ConfigInfo<bool> GFX_SW_ZCOMPLOC;
extern const ConfigInfo<bool> GFX_SW_ZFREEZE;
extern const ConfigInfo<bool> GFX_SW_DUMP_OBJECTS;
extern const ConfigInfo<bool> GFX_SW_DUMP_TEV_STAGES;
extern const ConfigInfo<bool> GFX_SW_DUMP_TEV_TEX_FETCHES;
extern const ConfigInfo<int> GFX_SW_DRAW_START;
extern const ConfigInfo<int> GFX_SW_DRAW_END;

// Graphics.Enhancements

extern const ConfigInfo<bool> GFX_ENHANCE_FORCE_FILTERING;
extern const ConfigInfo<bool> GFX_ENHANCE_DISABLE_FILTERING;
extern const ConfigInfo<int> GFX_ENHANCE_MAX_ANISOTROPY;  // NOTE - this is x in (1 << x)
extern const ConfigInfo<bool> GFX_ENHANCE_POST_ENABLED;
extern const ConfigInfo<int> GFX_ENHANCE_POST_TRIGUER;
extern const ConfigInfo<std::string> GFX_ENHANCE_POST_SHADERS;
extern const ConfigInfo<std::string> GFX_ENHANCE_SCALING_SHADER;
extern const ConfigInfo<bool> GFX_ENHANCE_FORCE_TRUE_COLOR;
extern const ConfigInfo<bool> GFX_ENHANCE_USE_SCALING_FILTER;
extern const ConfigInfo<int> GFX_ENHANCE_TEXTURE_SCALING_TYPE;
extern const ConfigInfo<int> GFX_ENHANCE_TEXTURE_SCALING_FACTOR;
extern const ConfigInfo<bool> GFX_ENHANCE_USE_DEPOSTERIZE;
extern const ConfigInfo<bool> GFX_ENHANCE_TESSELLATION;
extern const ConfigInfo<bool> GFX_ENHANCE_TESSELLATION_EARLY_CULLING;
extern const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_DISTANCE;
extern const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_MAX;
extern const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_ROUNDING_INTENSITY;
extern const ConfigInfo<int> GFX_ENHANCE_TESSELLATION_DISPLACEMENT_INTENSITY;

// Graphics.Stereoscopy

extern const ConfigInfo<int> GFX_STEREO_MODE;
extern const ConfigInfo<int> GFX_STEREO_DEPTH;
extern const ConfigInfo<int> GFX_STEREO_CONVERGENCE_PERCENTAGE;
extern const ConfigInfo<bool> GFX_STEREO_SWAP_EYES;
extern const ConfigInfo<int> GFX_STEREO_CONVERGENCE;
extern const ConfigInfo<bool> GFX_STEREO_EFB_MONO_DEPTH;
extern const ConfigInfo<int> GFX_STEREO_DEPTH_PERCENTAGE;
extern const ConfigInfo<std::string> GFX_STEREO_SHADER;

// Graphics.Hacks

extern const ConfigInfo<bool> GFX_HACK_EFB_ACCESS_ENABLE;
extern const ConfigInfo<bool> GFX_HACK_EFB_FAST_ACCESS_ENABLE;
extern const ConfigInfo<int> GFX_HACK_BBOX_MODE;
extern const ConfigInfo<bool> GFX_HACK_FORCE_PROGRESSIVE;
extern const ConfigInfo<bool> GFX_HACK_SKIP_EFB_COPY_TO_RAM;
extern const ConfigInfo<bool> GFX_HACK_COPY_EFB_ENABLED;
extern const ConfigInfo<bool> GFX_HACK_EFB_EMULATE_FORMAT_CHANGES;
extern const ConfigInfo<bool> GFX_HACK_VERTEX_ROUDING;
extern const ConfigInfo<bool> GFX_HACK_FORCE_DUAL_SOURCE;
extern const ConfigInfo<bool> GFX_HACK_FULL_ASYNC_SHADER_COMPILATION;
extern const ConfigInfo<bool> GFX_HACK_WAIT_FOR_SHADER_COMPILATION;
extern const ConfigInfo<bool> GFX_HACK_LAST_HISTORY_EFBTORAM;
extern const ConfigInfo<bool> GFX_HACK_FORCE_LOGICOP_BLEND;

// Graphics.GameSpecific

extern const ConfigInfo<int> GFX_PROJECTION_HACK;
extern const ConfigInfo<int> GFX_PROJECTION_HACK_SZNEAR;
extern const ConfigInfo<int> GFX_PROJECTION_HACK_SZFAR;
extern const ConfigInfo<std::string> GFX_PROJECTION_HACK_ZNEAR;
extern const ConfigInfo<std::string> GFX_PROJECTION_HACK_ZFAR;
extern const ConfigInfo<bool> GFX_PERF_QUERIES_ENABLE;



}  // namespace Config
