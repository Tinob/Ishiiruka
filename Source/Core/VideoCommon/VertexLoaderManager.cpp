// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino

#include <map>
#include <memory>
#include <unordered_map>


#include "Core/ConfigManager.h"
#include "Core/HW/Memmap.h"

#include "Common/ThreadPool.h"
#include "Common/StringUtil.h"

#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoConfig.h"



static VertexLoaderBase *s_cpu_loaders[8];
static std::string last_game_code;

typedef std::unordered_map<VertexLoaderUID, std::unique_ptr<VertexLoaderBase>> VertexLoaderMap;

namespace VertexLoaderManager
{
static VertexLoaderMap s_vertex_loader_map;
static NativeVertexFormatMap s_native_vertex_map;
static NativeVertexFormat* s_current_vtx_fmt;
u32 g_current_components;

NativeVertexFormat* GetCurrentVertexFormat()
{
  return s_current_vtx_fmt;
}

namespace
{
struct entry
{
  std::string text;
  u64 num_verts;
  bool operator < (const entry &other) const
  {
    return num_verts > other.num_verts;
  }
};

struct codeentry
{
  std::string name;
  std::string conf;
  u64 num_verts;
  std::string hash;
  bool operator < (const codeentry &other) const
  {
    return num_verts > other.num_verts;
  }
};
}

void AppendListToString(std::string *dest)
{
  std::vector<entry> entries;

  size_t total_size = 0;
  for (VertexLoaderMap::const_iterator iter = s_vertex_loader_map.begin(); iter != s_vertex_loader_map.end(); ++iter)
  {
    entry e;
    iter->second->AppendToString(&e.text);
    e.num_verts = iter->second->m_numLoadedVertices;
    entries.push_back(e);
    total_size += e.text.size() + 1;
  }
  sort(entries.begin(), entries.end());
  dest->reserve(dest->size() + total_size);
  for (std::vector<entry>::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
  {
    dest->append(iter->text);
  }
}

void Init()
{
  MarkAllDirty();
  for (VertexLoaderBase*& vertexLoader : g_main_cp_state.vertex_loaders)
    vertexLoader = nullptr;
  last_game_code = SConfig::GetInstance().GetGameID();
}

void Shutdown()
{
  s_vertex_loader_map.clear();
  s_native_vertex_map.clear();
}

void UpdateVertexArrayPointers()
{
  // Anything to update?
  if (!g_main_cp_state.bases_dirty)
    return;

  // Some games such as Burnout 2 can put invalid addresses into
  // the array base registers. (see issue 8591)
  // But the vertex arrays with invalid addresses aren't actually enabled.
  // Note: Only array bases 0 through 11 are used by the Vertex loaders.
  //       12 through 15 are used for loading data into xfmem.
  for (int i = 0; i < 12; i++)
  {
    // Only update the array base if the vertex description states we are going to use it.
    if (g_main_cp_state.vtx_desc.GetVertexArrayStatus(i) >= 0x2)
      cached_arraybases[i] = Memory::GetPointer(g_main_cp_state.array_bases[i]);
  }

  g_main_cp_state.bases_dirty = false;
}

inline NativeVertexFormat* GetNativeVertexFormat(const PortableVertexDeclaration& format)
{
  auto& native = s_native_vertex_map[format];
  if (!native)
  {
    native = g_vertex_manager->CreateNativeVertexFormat(format);
  }
  return native.get();
}

void MarkAllDirty()
{
  g_main_cp_state.attr_dirty = 0xff;
  g_main_cp_state.bases_dirty = true;
  g_preprocess_cp_state.attr_dirty = 0xff;
  g_preprocess_cp_state.bases_dirty = true;
}

NativeVertexFormat* GetOrCreateMatchingFormat(const PortableVertexDeclaration& decl)
{
  auto iter = s_native_vertex_map.find(decl);
  if (iter == s_native_vertex_map.end())
  {
    std::unique_ptr<NativeVertexFormat> fmt = g_vertex_manager->CreateNativeVertexFormat(decl);
    auto ipair = s_native_vertex_map.emplace(decl, std::move(fmt));
    iter = ipair.first;
  }

  return iter->second.get();
}

NativeVertexFormat* GetUberVertexFormat(const PortableVertexDeclaration& decl)
{
  // The padding in the structs can cause the memcmp() in the map to create duplicates.
  // Avoid this by initializing the padding to zero.
  PortableVertexDeclaration new_decl;
  std::memset(&new_decl, 0, sizeof(new_decl));
  new_decl.stride = decl.stride;

  auto MakeDummyAttribute = [](AttributeFormat& attr, EVTXComponentFormat type, int components) {
    attr.type = type;
    attr.components = components;
    attr.offset = 0;
    attr.enable = true;
  };
  auto CopyAttribute = [](AttributeFormat& attr, const AttributeFormat& src) {
    attr.type = src.type;
    attr.components = src.components;
    attr.offset = src.offset;
    attr.enable = src.enable;
  };

  if (decl.position.enable)
    CopyAttribute(new_decl.position, decl.position);
  else
    MakeDummyAttribute(new_decl.position, EVTXComponentFormat::FORMAT_FLOAT, 1);

  for (size_t i = 0; i < ArraySize(new_decl.normals); i++)
  {
    if (decl.normals[i].enable)
      CopyAttribute(new_decl.normals[i], decl.normals[i]);
    else
      MakeDummyAttribute(new_decl.normals[i], EVTXComponentFormat::FORMAT_FLOAT, 1);
  }
  for (size_t i = 0; i < ArraySize(new_decl.colors); i++)
  {
    if (decl.colors[i].enable)
      CopyAttribute(new_decl.colors[i], decl.colors[i]);
    else
      MakeDummyAttribute(new_decl.colors[i], EVTXComponentFormat::FORMAT_UBYTE, 4);
  }
  for (size_t i = 0; i < ArraySize(new_decl.texcoords); i++)
  {
    if (decl.texcoords[i].enable)
      CopyAttribute(new_decl.texcoords[i], decl.texcoords[i]);
    else
      MakeDummyAttribute(new_decl.texcoords[i], EVTXComponentFormat::FORMAT_FLOAT, 1);
  }

  CopyAttribute(new_decl.posmtx, decl.posmtx);

  return GetOrCreateMatchingFormat(new_decl);
}

inline VertexLoaderBase *GetOrAddLoader(const TVtxDesc &VtxDesc, const VAT &VtxAttr)
{
  VertexLoaderUID uid(VtxDesc, VtxAttr);
  VertexLoaderMap::iterator iter = s_vertex_loader_map.find(uid);
  if (iter == s_vertex_loader_map.end())
  {
    s_vertex_loader_map[uid] = VertexLoaderBase::CreateVertexLoader(VtxDesc, VtxAttr);
    VertexLoaderBase* loader = s_vertex_loader_map[uid].get();
    loader->m_native_vertex_format = GetNativeVertexFormat(loader->m_native_vtx_decl);
    VertexLoaderBase * fallback = loader->GetFallback();
    if (fallback)
    {
      fallback->m_native_vertex_format = GetNativeVertexFormat(fallback->m_native_vtx_decl);
    }
    INCSTAT(stats.numVertexLoaders);
    return loader;
  }
  return iter->second.get();
}

void GetVertexSizeAndComponents(const VertexLoaderParameters &parameters, u32 &vertexsize, u32 &components)
{
  if (parameters.needloaderrefresh)
  {
    s_cpu_loaders[parameters.vtx_attr_group] = GetOrAddLoader(*parameters.VtxDesc, *parameters.VtxAttr);
  }
  vertexsize = s_cpu_loaders[parameters.vtx_attr_group]->m_VertexSize;
  components = s_cpu_loaders[parameters.vtx_attr_group]->m_native_components;
}

inline void UpdateLoader(const VertexLoaderParameters &parameters)
{
  g_main_cp_state.vertex_loaders[parameters.vtx_attr_group] = GetOrAddLoader(*parameters.VtxDesc, *parameters.VtxAttr);
  g_main_cp_state.last_id = parameters.vtx_attr_group;
}

bool ConvertVertices(VertexLoaderParameters &parameters, u32 &readsize, u32 &writesize)
{
  if (parameters.needloaderrefresh)
  {
    UpdateLoader(parameters);
  }
  auto loader = g_main_cp_state.vertex_loaders[parameters.vtx_attr_group];
  if (!loader->EnvironmentIsSupported())
  {
    loader = loader->GetFallback();
  }
  readsize = parameters.count * loader->m_VertexSize;
  if (parameters.buf_size < readsize)
    return false;
  if (parameters.skip_draw)
  {
    return true;
  }
  // Lookup pointers for any vertex arrays.
  UpdateVertexArrayPointers();
  NativeVertexFormat *nativefmt = loader->m_native_vertex_format;
  // Flush if our vertex format is different from the currently set.
  if (s_current_vtx_fmt != nullptr && s_current_vtx_fmt != nativefmt)
  {
    g_vertex_manager->Flush();
  }
  s_current_vtx_fmt = nativefmt;
  g_current_components = loader->m_native_components;
  VertexShaderManager::SetVertexFormat(loader->m_native_components);
  g_vertex_manager->PrepareForAdditionalData(parameters.primitive, parameters.count, loader->m_native_stride);
  parameters.destination = g_vertex_manager->GetCurrentBufferPointer();
  s32 finalcount = loader->RunVertices(parameters);
  writesize = loader->m_native_stride * finalcount;
  IndexGenerator::AddIndices(parameters.primitive, finalcount);
  ADDSTAT(stats.thisFrame.numPrims, finalcount);
  INCSTAT(stats.thisFrame.numPrimitiveJoins);
  return true;
}

int GetVertexSize(const VertexLoaderParameters &parameters)
{
  if (parameters.needloaderrefresh)
  {
    UpdateLoader(parameters);
  }
  return g_main_cp_state.vertex_loaders[parameters.vtx_attr_group]->m_VertexSize;
}

}  // namespace
