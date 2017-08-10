// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <cstddef>
#include <string>

#include "Common/CommonTypes.h"
#include "Common/MathUtil.h"
#include "VideoCommon/TextureConfig.h"

class HostTexture
{
public:
  explicit HostTexture(const TextureConfig& c);
  virtual ~HostTexture();
  virtual void Bind(u32 stage) = 0;
  virtual bool Save(const std::string& filename, u32 level);

  virtual void CopyRectangleFromTexture(const HostTexture* source,
    const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect) = 0;
  virtual void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) = 0;
  virtual uintptr_t GetInternalObject() const = 0;

  const TextureConfig& GetConfig() const
  {
    return m_config;
  }

protected:
  TextureConfig m_config;
};
