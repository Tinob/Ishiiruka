// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/VideoCommon.h"

class HiresTexture
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

  static std::shared_ptr<HiresTexture> Search(const std::string& basename,
                                              std::function<u8*(size_t)> request_buffer_delegate);

  static std::shared_ptr<HiresTexture>
  SearchEnviroment(const std::string& basename, std::function<u8*(size_t)> request_buffer_delegate);

  static bool EnviromentExists(const std::string& basename);

  static std::string GenBaseName(const u8* texture, size_t texture_size, const u8* tlut,
                                 size_t tlut_size, u32 width, u32 height, int format,
                                 bool has_mipmaps, bool dump = false);

  ~HiresTexture(){};
  HostTextureFormat m_format;
  u32 m_width, m_height, m_levels, m_nrm_levels, m_lum_levels;
  bool has_arbitrary_mips;
  std::unique_ptr<u8> m_cached_data;
  size_t m_cached_data_size;

private:
  static void ProccessTexture(const std::string& fileitem, std::string& filename,
                              const std::string& extension, const bool BuildMaterialMaps);
  static void ProccessEnviroment(const std::string& fileitem, std::string& filename,
                                 const std::string& extension);
  static HiresTexture* Load(const std::string& base_filename,
                            std::function<u8*(size_t)> request_buffer_delegate, bool cacheresult);
  static HiresTexture* LoadEnviroment(const std::string& base_filename,
                                      std::function<u8*(size_t)> request_buffer_delegate,
                                      bool cacheresult);

  static void Prefetch();
  HiresTexture();
  static std::set<std::string> GetTextureDirectory(const std::string& game_id);
};
