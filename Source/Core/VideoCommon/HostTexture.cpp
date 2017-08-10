// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>

#include "VideoCommon/HostTexture.h"

HostTexture::HostTexture(const TextureConfig& c) : m_config(c)
{

}

HostTexture::~HostTexture() = default;

bool HostTexture::Save(const std::string& filename, u32 level)
{
  return false;
}