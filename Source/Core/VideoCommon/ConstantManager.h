// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonTypes.h"

// all constant buffer attributes must be 16 bytes aligned, so this are the only allowed components:
typedef float float4[4];
typedef u32 uint4[4];
typedef s32 int4[4];

struct PixelShaderConstants
{
  int4 colors[4];
  int4 kcolors[4];
  int4 alpha;
  float4 texdims[8];
  float4 texlayer[8];
  int4 zbias[2];
  int4 indtexscale[2];
  int4 indtexmtx[6];
  int4 fogcolor;
  int4 fogi;
  float4 fogf[2];
  float4 zslope;
  int4 flags;
  float4 efbscale;

  // Constants from here onwards are only used in ubershaders.
  u32 genmode;       // .x
  u32 alphaTest;     // .y
  u32 fogParam3;     // .z
  u32 fogRangeBase;  // .w
  u32 dstalpha;      // .x
  u32 ztex_op;       // .y
  u32 late_ztest;    // .z (bool)
  u32 rgba6_format;  // .w (bool)
  u32 dither;        // .x (bool)
  u32 bounding_box;  // .y (bool)
  u32 pad0;  // .z (bool)
  u32 pad1;  // .w (bool)
  uint4 pack1[16];   // .xy - combiners, .z - tevind, .w - iref
  uint4 pack2[8];    // .x - tevorder, .y - tevksel
  int4 konst[32];    // .rgba
};

struct VertexShaderConstants
{
  float4 cproj[4];
  float4 cDepth;
  float4 cViewport;
  float4 cmtrl[4];
  struct Light
  {
    float4 color;
    float4 cosatt;
    float4 distatt;
    float4 pos;
    float4 dir;
  } lights[8];
  float4 cphong[2];
  float4 ctexmtx[24];
  float4 ctrmtx[64];
  float4 cnmtx[32];
  float4 cpostmtx[64];
  float4 cPLOffset[13];
  // Only for Ubershaders
  u32 components;           // .x
  u32 xfmem_dualTexInfo;    // .y
  u32 xfmem_numColorChans;  // .z
  u32 pad0;                 // .w
  uint4 xfmem_pack1[8];  // .x - texMtxInfo, .y - postMtxInfo, [0..1].z = color, [0..1].w = alpha
};

struct GeometryShaderConstants
{
  float4 stereoparams;
  float4 lineptparams;
  int4 texoffset;
};

struct TessellationShaderConstants
{
  float4 tessparams;
  int4 cullparams;
};
