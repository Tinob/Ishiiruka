// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2++
// Refer to the license.txt file included.

#pragma once
#include "VideoBackends/DX11/D3DBase.h"

namespace DX11
{

class BBox
{
public:
  static void Init();
  static void Shutdown();

  static void Update();
  static void Set(s32 index, s32 value);
  static s32 Get(s32 index);
};

};
