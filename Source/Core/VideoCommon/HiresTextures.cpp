// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <cinttypes>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <xxhash.h>

#include <SOIL/SOIL.h>

#include "Common/CommonPaths.h"
#include "Common/File.h"
#include "Common/FileSearch.h"
#include "Common/FileUtil.h"
#include "Common/Flag.h"
#include "Common/Logging/Log.h"
#include "Common/MemoryUtil.h"
#include "Common/StringUtil.h"
#include "Common/Swap.h"
#include "Common/Thread.h"
#include "Common/Timer.h"

#include "Core/Config/GraphicsSettings.h"
#include "Core/ConfigManager.h"
#include "Core/Host.h"

#include "VideoCommon/HiresTextures.h"
#include "VideoCommon/ImageLoader.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "VideoCommon/TextureUtil.h"
#include "VideoCommon/VideoConfig.h"

struct hires_mip_level
{
  bool is_compressed;
  std::string path;
  std::string extension;
  hires_mip_level() : is_compressed(false), path(), extension() {}
  hires_mip_level(const std::string& p, const std::string& e, bool compressed)
      : is_compressed(compressed), path(p), extension(e)
  {
  }
};

enum MapType
{
  color = 0,
  material = 1,
  emissive = 2,
  normal = 3,
  bump = 4,
  specular = 5,

};

enum EnvType
{
  positiveX = 0,
  negativeX = 1,
  positiveY = 2,
  negativeY = 3,
  positiveZ = 4,
  negativeZ = 5
};

static const std::string s_maps_tags[] = {std::string(""),      std::string(".mat"),
                                          std::string(".lum"),  std::string(".nrm"),
                                          std::string(".bump"), std::string(".spec")};

static const std::string s_env_tags[] = {std::string(".px"), std::string(".nx"),
                                         std::string(".py"), std::string(".ny"),
                                         std::string(".pz"), std::string(".nz")};

struct HiresTextureCacheItem
{
  bool has_arbitrary_mips;
  std::array<std::vector<hires_mip_level>, MapType::specular + 1> maps;

  HiresTextureCacheItem(size_t minsize) : has_arbitrary_mips(false)
  {
    maps[MapType::color].resize(minsize);
  }
};

struct EnvTextureCacheItem
{
  bool has_arbitrary_mips;
  std::array<std::vector<hires_mip_level>, EnvType::negativeZ + 1> maps;

  EnvTextureCacheItem(size_t minsize) : has_arbitrary_mips(false)
  {
    maps[EnvType::positiveX].resize(minsize);
  }
};

typedef std::unordered_map<std::string, HiresTextureCacheItem> HiresTextureCache;
typedef std::unordered_map<std::string, EnvTextureCacheItem> EnvTextureCache;
static HiresTextureCache s_textureMap;
static EnvTextureCache s_enviromentMap;
typedef std::unordered_map<std::string, std::shared_ptr<HiresTexture>> TextureCache;
static TextureCache s_textureCache;
static TextureCache s_enviromentCache;

static std::mutex s_textureCacheMutex;
static Common::Flag s_textureCacheAbortLoading;

static std::atomic<size_t> size_sum;
static size_t max_mem = 0;
static std::thread s_prefetcher;

static const std::string s_format_prefix = "tex1_";
static const std::string s_enviroment_prefix = "env_";

HiresTexture::HiresTexture()
    : m_format(PC_TEX_FMT_NONE), m_height(0), m_levels(0), m_nrm_levels(0), m_lum_levels(0),
      m_cached_data(nullptr), m_cached_data_size(0)
{
}

void HiresTexture::Init()
{
  size_sum.store(0);
  size_t sys_mem = Common::MemPhysical();
  size_t recommended_min_mem = 2 * size_t(1024 * 1024 * 1024);
  // keep 2GB memory for system stability if system RAM is 4GB+ - use half of memory in other cases
  max_mem = (sys_mem / 2 < recommended_min_mem) ? (sys_mem / 2) : (sys_mem - recommended_min_mem);
  Update();
}

void HiresTexture::Shutdown()
{
  if (s_prefetcher.joinable())
  {
    s_textureCacheAbortLoading.Set();
    s_prefetcher.join();
  }
  s_textureMap.clear();
  s_enviromentMap.clear();
  s_textureCache.clear();
}

std::set<std::string> HiresTexture::GetTextureDirectory(const std::string& game_id)
{
  std::set<std::string> result;
  const std::string texture_directory = File::GetUserPath(D_HIRESTEXTURES_IDX) + game_id;

  if (File::Exists(texture_directory))
  {
    result.insert(texture_directory);
  }
  else
  {
    // If there's no directory with the region-specific ID, look for a 3-character region-free one
    const std::string region_free_directory =
        File::GetUserPath(D_HIRESTEXTURES_IDX) + game_id.substr(0, 3);

    if (File::Exists(region_free_directory))
    {
      result.insert(region_free_directory);
    }
  }

  const auto match_gameid = [game_id](const std::string& filename) {
    std::string basename;
    SplitPath(filename, nullptr, &basename, nullptr);
    return basename == game_id || basename == game_id.substr(0, 3);
  };

  // Look for any other directories that might be specific to the given gameid
  const auto root_directory = File::GetUserPath(D_HIRESTEXTURES_IDX);
  const auto files = Common::DoFileSearch({root_directory}, {".txt"}, true);
  for (const auto& file : files)
  {
    if (match_gameid(file))
    {
      // The following code is used to calculate the top directory
      // of a found gameid.txt file
      // ex:  <dolphin dir>/Load/Textures/My folder/gameids/<gameid>.txt
      // would insert "<dolphin dir>/Load/Textures/My folder"
      const auto directory_path = file.substr(root_directory.size());
      const std::size_t first_path_separator_position = directory_path.find_first_of(DIR_SEP_CHR);
      result.insert(root_directory + directory_path.substr(0, first_path_separator_position));
    }
  }

  return result;
}

static const std::string ddscode = ".dds";
static const std::string cddscode = ".DDS";
static const std::string miptag = "mip";

void HiresTexture::ProccessEnviroment(const std::string& fileitem, std::string& filename,
                                      const std::string& extension)
{
  size_t map_index = 0;
  for (size_t tag = 0; tag <= EnvType::negativeZ; tag++)
  {
    if (StringEndsWith(filename, s_env_tags[tag]))
    {
      map_index = tag;
      filename = filename.substr(0, filename.size() - s_env_tags[tag].size());
      break;
    }
  }
  const bool is_compressed = extension.compare(ddscode) == 0 || extension.compare(cddscode) == 0;
  hires_mip_level mip_level_detail(fileitem, extension, is_compressed);
  u32 level = 0;
  size_t idx = filename.find_last_of('_');
  std::string miplevel = filename.substr(idx + 1, std::string::npos);
  if (miplevel.substr(0, miptag.length()) == miptag)
  {
    sscanf(miplevel.substr(3, std::string::npos).c_str(), "%i", &level);
    filename = filename.substr(0, idx);
  }
  EnvTextureCache::iterator iter = s_enviromentMap.find(filename);
  u32 min_item_size = level + 1;
  if (iter == s_enviromentMap.end())
  {
    EnvTextureCacheItem item(min_item_size);
    item.maps[map_index].resize(min_item_size);
    std::vector<hires_mip_level>& dst = item.maps[map_index];
    dst[level] = mip_level_detail;
    s_enviromentMap.emplace(filename, item);
  }
  else
  {
    std::vector<hires_mip_level>& dst = iter->second.maps[map_index];
    if (dst.size() < min_item_size)
    {
      dst.resize(min_item_size);
    }
    dst[level] = mip_level_detail;
  }
}

void HiresTexture::ProccessTexture(const std::string& fileitem, std::string& filename,
                                   const std::string& extension, const bool BuildMaterialMaps)
{
  size_t map_index = 0;
  size_t max_type = BuildMaterialMaps ? MapType::specular : MapType::normal;
  bool arbitrary_mips = false;
  for (size_t tag = 1; tag <= MapType::specular; tag++)
  {
    if (StringEndsWith(filename, s_maps_tags[tag]))
    {
      map_index = tag;
      filename = filename.substr(0, filename.size() - s_maps_tags[tag].size());
      break;
    }
  }
  if (map_index > max_type)
  {
    return;
  }
  if (BuildMaterialMaps && map_index == MapType::material)
  {
    return;
  }
  else if (!BuildMaterialMaps && map_index == MapType::color)
  {
    const size_t arb_index = filename.rfind("_arb");
    arbitrary_mips = arb_index != std::string::npos;
    if (arbitrary_mips)
      filename.erase(arb_index, 4);
  }
  const bool is_compressed = extension.compare(ddscode) == 0 || extension.compare(cddscode) == 0;
  hires_mip_level mip_level_detail(fileitem, extension, is_compressed);
  u32 level = 0;
  size_t idx = filename.find_last_of('_');
  std::string miplevel = filename.substr(idx + 1, std::string::npos);
  if (miplevel.substr(0, miptag.length()) == miptag)
  {
    sscanf(miplevel.substr(3, std::string::npos).c_str(), "%i", &level);
    filename = filename.substr(0, idx);
  }
  HiresTextureCache::iterator iter = s_textureMap.find(filename);
  u32 min_item_size = level + 1;
  if (iter == s_textureMap.end())
  {
    HiresTextureCacheItem item(min_item_size);
    if (arbitrary_mips)
    {
      item.has_arbitrary_mips = true;
    }
    item.maps[map_index].resize(min_item_size);
    std::vector<hires_mip_level>& dst = item.maps[map_index];
    dst[level] = mip_level_detail;
    s_textureMap.emplace(filename, item);
  }
  else
  {
    std::vector<hires_mip_level>& dst = iter->second.maps[map_index];
    if (arbitrary_mips)
    {
      iter->second.has_arbitrary_mips = true;
    }
    if (dst.size() < min_item_size)
    {
      dst.resize(min_item_size);
    }
    dst[level] = mip_level_detail;
  }
}

void HiresTexture::Update()
{
  bool BuildMaterialMaps = g_ActiveConfig.bHiresMaterialMapsBuild;
  if (s_prefetcher.joinable())
  {
    s_textureCacheAbortLoading.Set();
    s_prefetcher.join();
  }

  if (!g_ActiveConfig.bHiresTextures)
  {
    s_textureMap.clear();
    s_enviromentMap.clear();
    s_textureCache.clear();
    s_enviromentCache.clear();
    size_sum.store(0);
    return;
  }

  if (!g_ActiveConfig.bCacheHiresTextures)
  {
    s_textureCache.clear();
    s_enviromentCache.clear();
    size_sum.store(0);
  }

  s_textureMap.clear();
  s_enviromentMap.clear();
  const std::string& game_id = SConfig::GetInstance().GetGameID();
  const std::set<std::string> texture_directories = GetTextureDirectory(game_id);
  const std::string resource_directory = File::GetSysDirectory() + RESOURCES_DIR DIR_SEP;
  std::vector<std::string> Extensions;
  Extensions.push_back(".png");
  if (!BuildMaterialMaps)
  {
    Extensions.push_back(".dds");
  }

  std::vector<std::string> Resourcefilenames =
      Common::DoFileSearch({resource_directory}, Extensions, /*recursive*/ true);

  for (const std::string& fileitem : Resourcefilenames)
  {
    std::string filename;
    std::string extension;
    SplitPath(fileitem, nullptr, &filename, &extension);
    if (filename.rfind(s_format_prefix, 0) == 0)
    {
      ProccessTexture(fileitem, filename, extension, BuildMaterialMaps);
    }
    else if (filename.rfind(s_enviroment_prefix, 0) == 0)
    {
      filename = filename.substr(s_enviroment_prefix.length());
      ProccessEnviroment(fileitem, filename, extension);
    }
  }

  for (const auto& texture_directory : texture_directories)
  {
    std::vector<std::string> filenames =
        Common::DoFileSearch({texture_directory}, Extensions, /*recursive*/ true);

    for (const std::string& fileitem : filenames)
    {
      std::string filename;
      std::string extension;
      SplitPath(fileitem, nullptr, &filename, &extension);
      if (filename.rfind(s_format_prefix, 0) == 0)
      {
        ProccessTexture(fileitem, filename, extension, BuildMaterialMaps);
      }
      else if (filename.rfind(s_enviroment_prefix, 0) == 0)
      {
        filename = filename.substr(s_enviroment_prefix.length());
        ProccessEnviroment(fileitem, filename, extension);
      }
    }
  }

  if (g_ActiveConfig.bCacheHiresTextures && s_textureMap.size() > 0)
  {
    // remove cached but deleted textures
    auto iter = s_textureCache.begin();
    while (iter != s_textureCache.end())
    {
      if (s_textureMap.find(iter->first) == s_textureMap.end())
      {
        size_sum.fetch_sub(iter->second->m_cached_data_size);
        iter = s_textureCache.erase(iter);
      }
      else
      {
        iter++;
      }
    }
    // remove cached but deleted enviroment textures
    auto iterenv = s_enviromentCache.begin();
    while (iterenv != s_enviromentCache.end())
    {
      if (s_enviromentMap.find(iterenv->first) == s_enviromentMap.end())
      {
        size_sum.fetch_sub(iterenv->second->m_cached_data_size);
        iterenv = s_enviromentCache.erase(iterenv);
      }
      else
      {
        iterenv++;
      }
    }
    s_textureCacheAbortLoading.Clear();
    s_prefetcher = std::thread(Prefetch);
    if (g_ActiveConfig.bWaitForCacheHiresTextures && s_prefetcher.joinable())
    {
      s_prefetcher.join();
    }
  }
}

void HiresTexture::Prefetch()
{
  Common::SetCurrentThreadName("Prefetcher");
  const size_t total = s_textureMap.size() + s_enviromentMap.size();
  size_t count = 0;
  size_t notification = 10;
  u32 starttime = Common::Timer::GetTimeMs();
  for (const auto& entry : s_textureMap)
  {
    const std::string& base_filename = entry.first;

    std::unique_lock<std::mutex> lk(s_textureCacheMutex);

    auto iter = s_textureCache.find(base_filename);
    if (iter == s_textureCache.end())
    {
      lk.unlock();
      HiresTexture* ptr = Load(
          base_filename, [](size_t requested_size) { return new u8[requested_size]; }, true);
      lk.lock();
      if (ptr != nullptr)
      {
        size_sum.fetch_add(ptr->m_cached_data_size);
        iter = s_textureCache.insert(
            iter, std::make_pair(base_filename, std::shared_ptr<HiresTexture>(ptr)));
      }
    }

    if (s_textureCacheAbortLoading.IsSet())
    {
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog("", -1, -1);
      }
      return;
    }

    if (size_sum.load() > max_mem)
    {
      Config::SetCurrent(Config::GFX_HIRES_TEXTURES, false);

      OSD::AddMessage(
          StringFromFormat(
              "Custom Textures prefetching after %.1f MB aborted, not enough RAM available",
              size_sum / (1024.0 * 1024.0)),
          10000);
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog("", -1, -1);
      }
      return;
    }
    count++;
    size_t percent = (count * 100) / total;
    if (percent >= notification)
    {
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog(GetStringT("Prefetching Custom Textures...").c_str(),
                                  static_cast<int>(count), static_cast<int>(total));
      }
      else
      {
        OSD::AddMessage(StringFromFormat("Custom Textures prefetching %.1f MB %zu %% finished",
                                         size_sum / (1024.0 * 1024.0), percent),
                        2000);
      }
      notification += 10;
    }
  }

  for (const auto& entry : s_enviromentMap)
  {
    const std::string& base_filename = entry.first;

    std::unique_lock<std::mutex> lk(s_textureCacheMutex);

    auto iter = s_enviromentCache.find(base_filename);
    if (iter == s_enviromentCache.end())
    {
      lk.unlock();
      HiresTexture* ptr = LoadEnviroment(
          base_filename, [](size_t requested_size) { return new u8[requested_size]; }, true);
      lk.lock();
      if (ptr != nullptr)
      {
        size_sum.fetch_add(ptr->m_cached_data_size);
        iter = s_enviromentCache.insert(
            iter, std::make_pair(base_filename, std::shared_ptr<HiresTexture>(ptr)));
      }
    }

    if (s_textureCacheAbortLoading.IsSet())
    {
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog("", -1, -1);
      }
      return;
    }

    if (size_sum.load() > max_mem)
    {
      Config::SetCurrent(Config::GFX_HIRES_TEXTURES, false);

      OSD::AddMessage(
          StringFromFormat(
              "Custom Textures prefetching after %.1f MB aborted, not enough RAM available",
              size_sum / (1024.0 * 1024.0)),
          10000);
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog("", -1, -1);
      }
      return;
    }
    count++;
    size_t percent = (count * 100) / total;
    if (percent >= notification)
    {
      if (g_ActiveConfig.bWaitForCacheHiresTextures)
      {
        Host_UpdateProgressDialog(GetStringT("Prefetching Custom Textures...").c_str(),
                                  static_cast<int>(count), static_cast<int>(total));
      }
      else
      {
        OSD::AddMessage(StringFromFormat("Custom Textures prefetching %.1f MB %zu %% finished",
                                         size_sum / (1024.0 * 1024.0), percent),
                        2000);
      }
      notification += 10;
    }
  }

  if (g_ActiveConfig.bWaitForCacheHiresTextures)
  {
    Host_UpdateProgressDialog("", -1, -1);
  }
  u32 stoptime = Common::Timer::GetTimeMs();
  OSD::AddMessage(StringFromFormat("Custom Textures loaded, %.1f MB in %.1f s",
                                   size_sum / (1024.0 * 1024.0), (stoptime - starttime) / 1000.0),
                  10000);
}

std::string HiresTexture::GenBaseName(const u8* texture, size_t texture_size, const u8* tlut,
                                      size_t tlut_size, u32 width, u32 height, int format,
                                      bool has_mipmaps, bool dump)
{
  std::string name = "";
  HiresTextureCache::iterator convert_iter;
  // checking for min/max on paletted textures
  u32 min = 0xffff;
  u32 max = 0;
  switch (tlut_size)
  {
  case 0:
    break;
  case 16 * 2:
    for (size_t i = 0; i < texture_size; i++)
    {
      min = std::min<u32>(min, texture[i] & 0xf);
      min = std::min<u32>(min, texture[i] >> 4);
      max = std::max<u32>(max, texture[i] & 0xf);
      max = std::max<u32>(max, texture[i] >> 4);
    }
    break;
  case 256 * 2:
    for (size_t i = 0; i < texture_size; i++)
    {
      min = std::min<u32>(min, texture[i]);
      max = std::max<u32>(max, texture[i]);
    }
    break;
  case 16384 * 2:
    for (size_t i = 0; i < texture_size / 2; i++)
    {
      min = std::min<u32>(min, Common::swap16(((u16*)texture)[i]) & 0x3fff);
      max = std::max<u32>(max, Common::swap16(((u16*)texture)[i]) & 0x3fff);
    }
    break;
  }
  if (tlut_size > 0)
  {
    tlut_size = 2 * (max + 1 - min);
    tlut += 2 * min;
  }
  u64 tex_hash = XXH64(texture, texture_size, 0);
  u64 tlut_hash = 0;
  if (tlut_size)
    tlut_hash = XXH64(tlut, tlut_size, 0);
  std::string basename = s_format_prefix + StringFromFormat("%dx%d%s_%016" PRIx64, width, height,
                                                            has_mipmaps ? "_m" : "", tex_hash);
  std::string tlutname = tlut_size ? StringFromFormat("_%016" PRIx64, tlut_hash) : "";
  std::string formatname = StringFromFormat("_%d", format);
  std::string fullname = basename + tlutname + formatname;
  std::string wildcardname = basename + "_$" + formatname;

  if (!dump && s_textureMap.find(wildcardname) != s_textureMap.end())
    return wildcardname;

    // else generate the complete texture
  if (dump || s_textureMap.find(fullname) != s_textureMap.end())
    return fullname;

  return "";
}

inline u8* LoadImageFromFile(const char* path, int& width, int& height)
{
  File::IOFile file(path, "rb");
  std::vector<u8> buffer(file.GetSize());
  if (!file.IsOpen() || !file.ReadBytes(buffer.data(), file.GetSize()))
  {
    return nullptr;
  }
  int image_channels;
  return SOIL_load_image_from_memory(buffer.data(), (int)buffer.size(), &width, &height,
                                     &image_channels, SOIL_LOAD_RGBA);
}

inline void ReadImageFile(ImageLoaderParams& ImgInfo)
{
  // libpng path seems to fail with some png files using soil meanwhile
  /*if (ImageLoader::ReadImageFile(ImgInfo))
  {
  ImgInfo.resultTex = PC_TEX_FMT_RGBA32;
  }
  */
  int image_width;
  int image_height;
  ImgInfo.resultTex = PC_TEX_FMT_NONE;
  u8* decoded = LoadImageFromFile(ImgInfo.Path, image_width, image_height);
  if (decoded == nullptr)
  {
    return;
  }

  // Reallocate the memory so we can manage it
  ImgInfo.Width = image_width;
  ImgInfo.Height = image_height;
  ImgInfo.data_size = image_width * image_height * 4;
  ImgInfo.dst = ImgInfo.request_buffer_delegate(ImgInfo.data_size, false);
  if (ImgInfo.dst)
  {
    memcpy(ImgInfo.dst, decoded, ImgInfo.data_size);
    ImgInfo.resultTex = PC_TEX_FMT_RGBA32;
  }
  SOIL_free_image_data(decoded);
}

inline void ReadDDS(ImageLoaderParams& ImgInfo)
{
  if (!ImageLoader::ReadDDS(ImgInfo))
  {
    if (ImgInfo.releaseresourcesonerror && ImgInfo.dst)
    {
      delete[] ImgInfo.dst;
      ImgInfo.dst = nullptr;
    }
    ReadImageFile(ImgInfo);
  }
}

inline void BuildMaterial(const HiresTextureCacheItem& item, ImageLoaderParams& ImgInfo,
                          size_t level)
{
  if (ImgInfo.resultTex != PC_TEX_FMT_RGBA32)
  {
    return;
  }
  bool bump = false;
  u8* bumpdata = ImgInfo.dst;
  if (item.maps[MapType::bump].size() > level)
  {
    auto& leveldata = item.maps[MapType::bump][level];
    int image_width;
    int image_height;
    bumpdata = LoadImageFromFile(leveldata.path.c_str(), image_width, image_height);
    if (bumpdata != nullptr && static_cast<u32>(image_width) == ImgInfo.Width &&
        static_cast<u32>(image_height) == ImgInfo.Height)
    {
      bump = true;
    }
  }
  bool specular = false;
  u8* speculardata = ImgInfo.dst;
  if (item.maps[MapType::specular].size() > level)
  {
    auto& leveldata = item.maps[MapType::specular][level];
    int image_width;
    int image_height;
    speculardata = LoadImageFromFile(leveldata.path.c_str(), image_width, image_height);
    if (speculardata != nullptr && static_cast<u32>(image_width) == ImgInfo.Width &&
        static_cast<u32>(image_height) == ImgInfo.Height)
    {
      specular = true;
    }
  }
  for (size_t i = 0; i < ImgInfo.Height; i++)
  {
    size_t idx = i * ImgInfo.Width * 4;
    for (size_t j = 0; j < ImgInfo.Width; j++)
    {
      ImgInfo.dst[idx + 3] = ImgInfo.dst[idx];
      ImgInfo.dst[idx] = bump ? bumpdata[idx] : 127;
      ImgInfo.dst[idx] = specular ? speculardata[idx] : 51;
      idx += 4;
    }
  }
  if (bumpdata != nullptr && bumpdata != ImgInfo.dst)
  {
    SOIL_free_image_data(bumpdata);
  }
  if (speculardata != nullptr && speculardata != ImgInfo.dst)
  {
    SOIL_free_image_data(speculardata);
  }
}

std::shared_ptr<HiresTexture>
HiresTexture::Search(const std::string& basename,
                     std::function<u8*(size_t)> request_buffer_delegate)
{
  if (g_ActiveConfig.bCacheHiresTextures)
  {
    std::unique_lock<std::mutex> lk(s_textureCacheMutex);

    auto iter = s_textureCache.find(basename);
    if (iter != s_textureCache.end())
    {
      HiresTexture* current = iter->second.get();
      u8* dst = request_buffer_delegate(current->m_cached_data_size);
      memcpy(dst, current->m_cached_data.get(), current->m_cached_data_size);
      return iter->second;
    }
    lk.unlock();
    if (size_sum.load() < max_mem)
    {
      std::shared_ptr<HiresTexture> ptr(Load(
          basename, [](size_t requested_size) { return new u8[requested_size]; }, true));
      lk.lock();
      if (ptr)
      {
        s_textureCache[basename] = ptr;
        HiresTexture* current = ptr.get();
        size_sum.fetch_add(current->m_cached_data_size);
        u8* dst = request_buffer_delegate(current->m_cached_data_size);
        memcpy(dst, current->m_cached_data.get(), current->m_cached_data_size);
      }
      return ptr;
    }
  }
  return std::shared_ptr<HiresTexture>(Load(basename, request_buffer_delegate, false));
}

bool HiresTexture::EnviromentExists(const std::string& basename)
{
  if (s_enviromentMap.size() == 0 || !g_ActiveConfig.HiresMaterialMapsEnabled())
  {
    return false;
  }
  EnvTextureCache::iterator iter = s_enviromentMap.find(basename);
  if (iter == s_enviromentMap.end())
  {
    return false;
  }
  return true;
}

std::shared_ptr<HiresTexture>
HiresTexture::SearchEnviroment(const std::string& basename,
                               std::function<u8*(size_t)> request_buffer_delegate)
{
  if (g_ActiveConfig.bCacheHiresTextures)
  {
    std::unique_lock<std::mutex> lk(s_textureCacheMutex);

    auto iter = s_enviromentCache.find(basename);
    if (iter != s_enviromentCache.end())
    {
      HiresTexture* current = iter->second.get();
      u8* dst = request_buffer_delegate(current->m_cached_data_size);
      memcpy(dst, current->m_cached_data.get(), current->m_cached_data_size);
      return iter->second;
    }
    lk.unlock();
    if (size_sum.load() < max_mem)
    {
      std::shared_ptr<HiresTexture> ptr(LoadEnviroment(
          basename, [](size_t requested_size) { return new u8[requested_size]; }, true));
      lk.lock();
      if (ptr)
      {
        s_enviromentCache[basename] = ptr;
        HiresTexture* current = ptr.get();
        size_sum.fetch_add(current->m_cached_data_size);
        u8* dst = request_buffer_delegate(current->m_cached_data_size);
        memcpy(dst, current->m_cached_data.get(), current->m_cached_data_size);
      }
      return ptr;
    }
  }
  return std::shared_ptr<HiresTexture>(LoadEnviroment(basename, request_buffer_delegate, false));
}

ImageLoaderParams LoadMipLevel(const hires_mip_level& item,
                               const std::function<u8*(size_t, bool)>& bufferdelegate,
                               bool cacheresult)
{
  ImageLoaderParams imgInfo;
  imgInfo.releaseresourcesonerror = cacheresult;
  imgInfo.dst = nullptr;
  imgInfo.Path = item.path.c_str();
  imgInfo.request_buffer_delegate = bufferdelegate;
  if (item.is_compressed)
  {
    ReadDDS(imgInfo);
  }
  else
  {
    ReadImageFile(imgInfo);
  }
  return imgInfo;
}

bool ValidateImage(const ImageLoaderParams& imgInfo, size_t level, u32 maxwidth, u32 maxheight,
                   HostTextureFormat fmt, const char* lavel)
{
  if (maxwidth != imgInfo.Width || maxheight != imgInfo.Height || fmt != imgInfo.resultTex)
  {
    ERROR_LOG(
        VIDEO,
        "Custom texture %s invalid level %zu size: %u %u required: %u %u format: %u in %s map",
        imgInfo.Path, level, imgInfo.Width, imgInfo.Height, maxwidth, maxheight, fmt, lavel);
    return false;
  }
  return true;
}

void SkipMipData(u32 levels, u32 width, u32 height, HostTextureFormat fmt, u8*& buffer_pointer,
                 size_t& remaining_buffer_size)
{
  for (u32 mip_level = 1; mip_level != levels; ++mip_level)
  {
    u32 mip_width = TextureUtil::CalculateLevelSize(width, mip_level);
    u32 mip_height = TextureUtil::CalculateLevelSize(height, mip_level);
    u32 requiredsize = TextureUtil::GetTextureSizeInBytes(mip_width, mip_height, fmt);
    buffer_pointer += requiredsize;
    remaining_buffer_size -= requiredsize;
  }
}

HiresTexture* HiresTexture::Load(const std::string& basename,
                                 std::function<u8*(size_t)> request_buffer_delegate,
                                 bool cacheresult)
{
  if (s_textureMap.size() == 0)
  {
    return nullptr;
  }
  HiresTextureCache::iterator iter = s_textureMap.find(basename);
  if (iter == s_textureMap.end())
  {
    return nullptr;
  }
  HiresTextureCacheItem& current = iter->second;
  if (current.maps[MapType::color].size() == 0)
  {
    return nullptr;
  }
  // First level is mandatory
  if (current.maps[MapType::color][0].path.size() == 0)
  {
    return nullptr;
  }
  HiresTexture* ret = nullptr;
  u8* buffer_pointer;
  u32 maxwidth = 0;
  u32 maxheight = 0;
  bool last_level_is_dds = false;
  bool allocated_data = false;
  bool mipmapsize_included = false;
  size_t material_mat_index =
      current.maps[MapType::color].size() == current.maps[MapType::normal].size() &&
              current.maps[MapType::normal].size() != current.maps[MapType::material].size() ?
          MapType::normal :
          MapType::material;
  size_t emissive_index = MapType::emissive;
  bool nrm_posible =
      current.maps[MapType::color].size() == current.maps[material_mat_index].size() &&
      g_ActiveConfig.HiresMaterialMapsEnabled();
  bool emissive_posible =
      current.maps[MapType::color].size() == current.maps[emissive_index].size() &&
      g_ActiveConfig.HiresMaterialMapsEnabled();
  size_t remaining_buffer_size = 0;
  size_t total_buffer_size = 0;
  std::function<u8*(size_t, bool)> first_level_function = [&](size_t requiredsize,
                                                              bool mipmapsincluded) {
    // Allocate double side buffer if we are going to load normal maps
    allocated_data = true;
    mipmapsize_included = mipmapsincluded;
    // Pre allocate space for the textures and potetially all the posible mip levels
    if (current.maps[MapType::color].size() > 1 && !mipmapsincluded)
    {
      requiredsize = (requiredsize * 4) / 3;
    }
    requiredsize *=
        (nrm_posible && emissive_posible ? 3 : (nrm_posible || emissive_posible ? 2 : 1));
    total_buffer_size = requiredsize;
    remaining_buffer_size = total_buffer_size;
    return request_buffer_delegate(requiredsize);
  };
  std::function<u8*(size_t, bool)> allocation_function = [&](size_t requiredsize,
                                                             bool mipmapsincluded) {
    // is required size is biguer that the remaining size on the packed buffer just reject
    if (requiredsize > remaining_buffer_size)
    {
      return static_cast<u8*>(nullptr);
    }
    // just return the pointer to pack the textures in a single buffer.
    return buffer_pointer;
  };
  for (size_t level = 0; level < current.maps[MapType::color].size(); level++)
  {
    const hires_mip_level& item = current.maps[MapType::color][level];
    ImageLoaderParams imgInfo =
        LoadMipLevel(item, level == 0 ? first_level_function : allocation_function, cacheresult);
    imgInfo.releaseresourcesonerror = cacheresult;
    nrm_posible = nrm_posible && current.maps[material_mat_index][level].path.size() > 0;
    emissive_posible = emissive_posible && current.maps[emissive_index][level].path.size() > 0;
    bool ddsfile = item.is_compressed && TexDecoder::IsCompressed(imgInfo.resultTex);
    if ((level > 0 && ddsfile != last_level_is_dds) || imgInfo.dst == nullptr ||
        imgInfo.resultTex == PC_TEX_FMT_NONE)
    {
      // don't give support to mixed formats
      if (allocated_data && cacheresult && imgInfo.dst != nullptr)
      {
        delete[] imgInfo.dst;
      }
      break;
    }
    last_level_is_dds = ddsfile;
    if (level == 0)
    {
      ret = new HiresTexture();
      ret->has_arbitrary_mips = current.has_arbitrary_mips;
      ret->m_format = imgInfo.resultTex;
      ret->m_width = maxwidth = imgInfo.Width;
      ret->m_height = maxheight = imgInfo.Height;
      if (cacheresult)
      {
        ret->m_cached_data.reset(imgInfo.dst);
        ret->m_cached_data_size = total_buffer_size;
      }
    }
    else
    {
      if (!ValidateImage(imgInfo, level, maxwidth, maxheight, ret->m_format, "color"))
        break;
    }
    ret->m_levels++;
    if (ddsfile)
    {
      u32 requiredsize = TextureUtil::GetTextureSizeInBytes(maxwidth, maxheight, imgInfo.resultTex);
      buffer_pointer = imgInfo.dst + requiredsize;
      remaining_buffer_size -= requiredsize;
    }
    else
    {
      buffer_pointer = imgInfo.dst + imgInfo.data_size;
      remaining_buffer_size -= imgInfo.data_size;
    }

    if (imgInfo.nummipmaps > 0)
    {
      // Give priority to load dds with packed levels
      ret->m_levels = imgInfo.nummipmaps + 1;
      if (nrm_posible || emissive_posible)
      {
        SkipMipData(ret->m_levels, imgInfo.Width, imgInfo.Height, ret->m_format, buffer_pointer,
                    remaining_buffer_size);
      }
      break;
    }

    // no more mipmaps available
    if (maxwidth == 1 && maxheight == 1)
      break;
    maxwidth = std::max(maxwidth >> 1, 1u);
    maxheight = std::max(maxheight >> 1, 1u);
  }
  if (nrm_posible)
  {
    for (size_t level = 0; level < current.maps[material_mat_index].size(); level++)
    {
      const hires_mip_level& item = current.maps[material_mat_index][level];
      ImageLoaderParams imgInfo = LoadMipLevel(item, allocation_function, cacheresult);
      bool ddsfile = item.is_compressed && TexDecoder::IsCompressed(imgInfo.resultTex);
      if ((level > 0 && ddsfile != last_level_is_dds) || imgInfo.dst == nullptr ||
          imgInfo.resultTex == PC_TEX_FMT_NONE)
      {
        // don't give support to mixed formats
        break;
      }
      if (g_ActiveConfig.bHiresMaterialMapsBuild && imgInfo.resultTex == PC_TEX_FMT_RGBA32)
      {
        BuildMaterial(current, imgInfo, level);
      }
      if (level == 0)
      {
        maxwidth = imgInfo.Width;
        maxheight = imgInfo.Height;
        if (!ValidateImage(imgInfo, level, ret->m_width, ret->m_height, ret->m_format, "normal"))
          break;
      }
      else
      {
        if (!ValidateImage(imgInfo, level, maxwidth, maxheight, ret->m_format, "normal"))
          break;
      }
      ret->m_nrm_levels++;
      if (ddsfile)
      {
        u32 requiredsize =
            TextureUtil::GetTextureSizeInBytes(maxwidth, maxheight, imgInfo.resultTex);
        buffer_pointer = imgInfo.dst + requiredsize;
        remaining_buffer_size -= requiredsize;
      }
      else
      {
        buffer_pointer = imgInfo.dst + imgInfo.data_size;
        remaining_buffer_size -= imgInfo.data_size;
      }
      if (imgInfo.nummipmaps > 0)
      {
        // Give priority to load dds with packed levels
        ret->m_nrm_levels = imgInfo.nummipmaps + 1;
        if (emissive_posible)
        {
          SkipMipData(ret->m_nrm_levels, imgInfo.Width, imgInfo.Height, ret->m_format,
                      buffer_pointer, remaining_buffer_size);
        }
        break;
      }
      // no more mipmaps available
      if (maxwidth == 1 && maxheight == 1)
        break;
      maxwidth = std::max(maxwidth >> 1, 1u);
      maxheight = std::max(maxheight >> 1, 1u);
    }
  }
  if (emissive_posible)
  {
    for (size_t level = 0; level < current.maps[emissive_index].size(); level++)
    {
      const hires_mip_level& item = current.maps[emissive_index][level];
      ImageLoaderParams imgInfo = LoadMipLevel(item, allocation_function, cacheresult);
      bool ddsfile = item.is_compressed && TexDecoder::IsCompressed(imgInfo.resultTex);
      if ((level > 0 && ddsfile != last_level_is_dds) || imgInfo.dst == nullptr ||
          imgInfo.resultTex == PC_TEX_FMT_NONE)
      {
        // don't give support to mixed formats
        break;
      }
      if (level == 0)
      {
        maxwidth = imgInfo.Width;
        maxheight = imgInfo.Height;
        if (!ValidateImage(imgInfo, level, ret->m_width, ret->m_height, ret->m_format, "normal"))
          break;
      }
      else
      {
        if (!ValidateImage(imgInfo, level, maxwidth, maxheight, ret->m_format, "normal"))
          break;
      }
      ret->m_lum_levels++;
      if (ddsfile)
      {
        u32 requiredsize =
            TextureUtil::GetTextureSizeInBytes(maxwidth, maxheight, imgInfo.resultTex);
        buffer_pointer = imgInfo.dst + requiredsize;
        remaining_buffer_size -= requiredsize;
      }
      else
      {
        buffer_pointer = imgInfo.dst + imgInfo.data_size;
        remaining_buffer_size -= imgInfo.data_size;
      }
      if (imgInfo.nummipmaps > 0)
      {
        // Give priority to load dds with packed levels
        ret->m_lum_levels = imgInfo.nummipmaps + 1;
        break;
      }
      // no more mipmaps available
      if (maxwidth == 1 && maxheight == 1)
        break;
      maxwidth = std::max(maxwidth >> 1, 1u);
      maxheight = std::max(maxheight >> 1, 1u);
    }
  }
  if (ret != nullptr)
  {
    u32 levels = ret->m_levels;
    // Disable normal map if the size or levels are different
    if (ret->m_nrm_levels != levels)
    {
      ret->m_nrm_levels = 0;
    }
    if (ret->m_lum_levels != levels)
    {
      ret->m_lum_levels = 0;
    }
  }
  return ret;
}

HiresTexture* HiresTexture::LoadEnviroment(const std::string& basename,
                                           std::function<u8*(size_t)> request_buffer_delegate,
                                           bool cacheresult)
{
  if (s_enviromentMap.size() == 0 || !g_ActiveConfig.HiresMaterialMapsEnabled())
  {
    return nullptr;
  }
  EnvTextureCache::iterator iter = s_enviromentMap.find(basename);
  if (iter == s_enviromentMap.end())
  {
    return nullptr;
  }
  // all enviroment faces are mandatory
  EnvTextureCacheItem& current = iter->second;
  bool complete = current.maps.size() == (EnvType::negativeZ + 1);
  size_t levels = SIZE_MAX;
  for (size_t i = 0; i < current.maps.size(); i++)
  {
    levels = std::min(levels, current.maps[i].size());
    if (current.maps[i].size() == 0)
    {
      complete = false;
      break;
    }
  }
  if (!complete)
  {
    return nullptr;
  }
  HiresTexture* ret = nullptr;
  u8* buffer_pointer;
  u32 maxwidth = 0;
  u32 maxheight = 0;
  bool last_level_is_dds = false;
  bool allocated_data = false;
  bool mipmapsize_included = false;
  size_t remaining_buffer_size = 0;
  size_t total_buffer_size = 0;
  std::function<u8*(size_t, bool)> first_level_function = [&](size_t requiredsize,
                                                              bool mipmapsincluded) {
    // Allocate double side buffer if we are going to load normal maps
    allocated_data = true;
    mipmapsize_included = mipmapsincluded;
    // Pre allocate space for the textures and potetially all the posible mip levels
    if (levels > 1 && !mipmapsincluded)
    {
      requiredsize = (requiredsize * 4) / 3;
    }
    total_buffer_size = requiredsize * (EnvType::negativeZ + 1);
    remaining_buffer_size = total_buffer_size;
    buffer_pointer = request_buffer_delegate(total_buffer_size);
    return buffer_pointer;
  };
  std::function<u8*(size_t, bool)> allocation_function = [&](size_t requiredsize,
                                                             bool mipmapsincluded) {
    // is required size is biguer that the remaining size on the packed buffer just reject
    if (requiredsize > remaining_buffer_size)
    {
      return static_cast<u8*>(nullptr);
    }
    // just return the pointer to pack the textures in a single buffer.
    return buffer_pointer;
  };

  for (size_t i = 0; i < current.maps.size(); i++)
  {
    for (size_t level = 0; level < levels; level++)
    {
      const hires_mip_level& item = current.maps[i][level];
      ImageLoaderParams imgInfo = LoadMipLevel(
          item, i == 0 && level == 0 ? first_level_function : allocation_function, cacheresult);
      imgInfo.releaseresourcesonerror = cacheresult;
      bool ddsfile = item.is_compressed && TexDecoder::IsCompressed(imgInfo.resultTex);
      if ((level > 0 && ddsfile != last_level_is_dds) || imgInfo.dst == nullptr ||
          imgInfo.resultTex == PC_TEX_FMT_NONE)
      {
        // don't give support to mixed formats
        if (allocated_data && cacheresult && imgInfo.dst != nullptr)
        {
          delete[] imgInfo.dst;
        }
        break;
      }
      last_level_is_dds = ddsfile;
      if (level == 0 && i == 0)
      {
        ret = new HiresTexture();
        ret->has_arbitrary_mips = current.has_arbitrary_mips;
        ret->m_format = imgInfo.resultTex;
        ret->m_width = maxwidth = imgInfo.Width;
        ret->m_height = maxheight = imgInfo.Height;
        if (cacheresult)
        {
          ret->m_cached_data.reset(imgInfo.dst);
          ret->m_cached_data_size = total_buffer_size;
        }
      }
      else
      {
        if (!ValidateImage(imgInfo, level, maxwidth, maxheight, ret->m_format, "color"))
          break;
      }

      buffer_pointer = imgInfo.dst + imgInfo.data_size;
      remaining_buffer_size -= imgInfo.data_size;

      if (imgInfo.nummipmaps > 0)
      {
        // Give priority to load dds with packed levels
        ret->m_levels = imgInfo.nummipmaps + 1;
        break;
      }
      else if (i == 0)
      {
        ret->m_levels++;
      }

      // no more mipmaps available
      if (maxwidth == 1 && maxheight == 1)
        break;
      maxwidth = std::max(maxwidth >> 1, 1u);
      maxheight = std::max(maxheight >> 1, 1u);
    }
  }
  return ret;
}
