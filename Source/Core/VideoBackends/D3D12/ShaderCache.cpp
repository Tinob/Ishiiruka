// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
#include <unordered_map>

#include "Common/LinearDiskCache.h"

#include "Core/ConfigManager.h"
#include "Core/Host.h"

#include "VideoBackends/D3D12/D3DBlob.h"
#include "VideoBackends/D3D12/D3DCommandListManager.h"
#include "VideoBackends/D3D12/D3DShader.h"
#include "VideoBackends/D3D12/ShaderCache.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/HLSLCompiler.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/ObjectUsageProfiler.h"

namespace DX12
{

// Primitive topology type is always triangle, unless the GS stage is used. This is consumed
// by the PSO created in Renderer::ApplyState.
static D3D12_PRIMITIVE_TOPOLOGY_TYPE s_current_primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
static pKey_t gameid = 0;
struct ByteCodeCacheEntry
{
  D3D12_SHADER_BYTECODE m_shader_bytecode;
  bool m_compiled;
  std::atomic_flag m_initialized;

  ByteCodeCacheEntry() : m_compiled(false), m_shader_bytecode({})
  {
    m_initialized.clear();
  }
  void Release()
  {
    if (m_shader_bytecode.pShaderBytecode)
      delete[] m_shader_bytecode.pShaderBytecode;
    m_shader_bytecode.pShaderBytecode = nullptr;
  }
};

static ByteCodeCacheEntry s_pass_entry;

using PsBytecodeCache = ObjectUsageProfiler<PixelShaderUid, pKey_t, ByteCodeCacheEntry, PixelShaderUid::ShaderUidHasher>;
using VsBytecodeCache = ObjectUsageProfiler<VertexShaderUid, pKey_t, ByteCodeCacheEntry, VertexShaderUid::ShaderUidHasher>;
using TsBytecodeCache = ObjectUsageProfiler<TessellationShaderUid, pKey_t, std::pair<ByteCodeCacheEntry, ByteCodeCacheEntry>, TessellationShaderUid::ShaderUidHasher>;
using GsBytecodeCache = std::unordered_map<GeometryShaderUid, ByteCodeCacheEntry, GeometryShaderUid::ShaderUidHasher>;
using PUSBytecodeCache = std::unordered_map<UberShader::PixelUberShaderUid, ByteCodeCacheEntry, UberShader::PixelUberShaderUid::ShaderUidHasher>;
using VUSBytecodeCache = std::unordered_map<UberShader::VertexUberShaderUid, ByteCodeCacheEntry, UberShader::VertexUberShaderUid::ShaderUidHasher>;

PsBytecodeCache* ps_bytecode_cache;
VsBytecodeCache* vs_bytecode_cache;
TsBytecodeCache* ts_bytecode_cache;
GsBytecodeCache gs_bytecode_cache;
PUSBytecodeCache pus_bytecode_cache;
VUSBytecodeCache vus_bytecode_cache;

static std::vector<D3DBlob*> s_static_blob_list;
static std::vector<D3DBlob*> s_host_blob_list;

static LinearDiskCache<TessellationShaderUid, u8> s_hs_disk_cache;
static LinearDiskCache<TessellationShaderUid, u8> s_ds_disk_cache;
static LinearDiskCache<GeometryShaderUid, u8> s_gs_disk_cache;
static LinearDiskCache<PixelShaderUid, u8> s_ps_disk_cache;
static LinearDiskCache<VertexShaderUid, u8> s_vs_disk_cache;
static LinearDiskCache<UberShader::PixelUberShaderUid, u8> s_pus_disk_cache;
static LinearDiskCache<UberShader::VertexUberShaderUid, u8> s_vus_disk_cache;

static ByteCodeCacheEntry* s_last_domain_shader_bytecode;
static ByteCodeCacheEntry* s_last_hull_shader_bytecode;
static ByteCodeCacheEntry* s_last_geometry_shader_bytecode;
static ByteCodeCacheEntry* s_last_pixel_shader_bytecode;
static ByteCodeCacheEntry* s_last_vertex_shader_bytecode;
static ByteCodeCacheEntry* s_last_pixel_uber_shader_bytecode;
static ByteCodeCacheEntry* s_last_vertex_uber_shader_bytecode;

static GeometryShaderUid s_last_geometry_shader_uid;
static PixelShaderUid s_last_pixel_shader_uid;
static VertexShaderUid s_last_vertex_shader_uid;
static TessellationShaderUid s_last_tessellation_shader_uid;
static UberShader::PixelUberShaderUid s_last_pixel_uber_shader_uid;
static UberShader::VertexUberShaderUid s_last_vertex_uber_shader_uid;
static bool s_use_pixel_uber_shader = false;
static bool s_use_vertex_uber_shader = false;

static HLSLAsyncCompiler *s_compiler;

class PixelShaderCacheInserter final : public LinearDiskCacheReader<PixelShaderUid, u8>
{
public:
  void Read(const PixelShaderUid &key, const u8* value, u32 value_size)
  {
    PixelShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    D3DBlob* blob = new D3DBlob(value_size, value);
    ShaderCache::InsertPSByteCode(uid, blob);
  }
};

class PixelUberShaderCacheInserter final : public LinearDiskCacheReader<UberShader::PixelUberShaderUid, u8>
{
public:
  void Read(const UberShader::PixelUberShaderUid &key, const u8* value, u32 value_size)
  {
    D3DBlob* blob = new D3DBlob(value_size, value);
    UberShader::PixelUberShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    ShaderCache::InsertPUSByteCode(uid, blob);
  }
};

class VertexShaderCacheInserter final : public LinearDiskCacheReader<VertexShaderUid, u8>
{
public:
  void Read(const VertexShaderUid &key, const u8* value, u32 value_size)
  {
    VertexShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    D3DBlob* blob = new D3DBlob(value_size, value);
    ShaderCache::InsertVSByteCode(uid, blob);
  }
};

class VertexUberShaderCacheInserter final : public LinearDiskCacheReader<UberShader::VertexUberShaderUid, u8>
{
public:
  void Read(const UberShader::VertexUberShaderUid &key, const u8* value, u32 value_size)
  {
    D3DBlob* blob = new D3DBlob(value_size, value);
    UberShader::VertexUberShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    ShaderCache::InsertVUSByteCode(uid, blob);
  }
};

class GeometryShaderCacheInserter : public LinearDiskCacheReader<GeometryShaderUid, u8>
{
public:
  void Read(const GeometryShaderUid &key, const u8* value, u32 value_size)
  {
    GeometryShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    D3DBlob* blob = new D3DBlob(value_size, value);
    ShaderCache::InsertGSByteCode(uid, blob);
  }
};

class DShaderCacheInserter final : public LinearDiskCacheReader<TessellationShaderUid, u8>
{
public:
  void Read(const TessellationShaderUid &key, const u8* value, u32 value_size)
  {
    TessellationShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    D3DBlob* blob = new D3DBlob(value_size, value);
    ShaderCache::InsertDSByteCode(uid, blob);
  }
};

class HShaderCacheInserter final : public LinearDiskCacheReader<TessellationShaderUid, u8>
{
public:
  void Read(const TessellationShaderUid &key, const u8* value, u32 value_size)
  {
    TessellationShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    D3DBlob* blob = new D3DBlob(value_size, value);
    ShaderCache::InsertHSByteCode(uid, blob);
  }
};

bool ShaderCache::UsePixelUberShader()
{
  return s_use_pixel_uber_shader;
}
bool ShaderCache::UseVertexUberShader()
{
  return s_use_vertex_uber_shader;
}

void ShaderCache::Init()
{
  s_compiler = &HLSLAsyncCompiler::getInstance();
  s_pass_entry.m_compiled = true;
  s_pass_entry.m_initialized.test_and_set();
  ps_bytecode_cache = nullptr;
  vs_bytecode_cache = nullptr;
  ts_bytecode_cache = nullptr;

  std::string title_unique_id = SConfig::GetInstance().GetGameID();


  gameid = (pKey_t)GetMurmurHash3(reinterpret_cast<const u8*>(SConfig::GetInstance().GetGameID().data()), (u32)SConfig::GetInstance().GetGameID().size(), 0);

  ts_bytecode_cache = TsBytecodeCache::Create(
    gameid,
    TESSELLATIONSHADERGEN_UID_VERSION,
    "Ishiiruka.ts",
    StringFromFormat("%s.ts", title_unique_id.c_str())
  );

  LoadFromDisk();
  LoadHostBasedFromDisk();

  // Clear out disk cache when debugging shaders to ensure stale ones don't stick around..
  SETSTAT(stats.numGeometryShadersAlive, static_cast<int>(gs_bytecode_cache.size()));
  SETSTAT(stats.numGeometryShadersCreated, 0);
  SETSTAT(stats.numPixelShadersAlive, static_cast<int>(ps_bytecode_cache->size()));
  SETSTAT(stats.numPixelShadersCreated, 0);
  SETSTAT(stats.numVertexShadersAlive, static_cast<int>(vs_bytecode_cache->size()));
  SETSTAT(stats.numVertexShadersCreated, 0);

  if (g_ActiveConfig.bCompileShaderOnStartup && !g_ActiveConfig.bDisableSpecializedShaders)
  {
    CompileShaders();
    CompileHostBasedShaders();
  }
  if (g_ActiveConfig.CanPrecompileUberShaders())
  {
    CompileUberShaders();
  }
  // This class intentionally shares its shader cache files with DX11, as the shaders are (right now) identical.
  // Reduces unnecessary compilation when switching between APIs.
  s_last_domain_shader_bytecode = &s_pass_entry;
  s_last_hull_shader_bytecode = &s_pass_entry;
  s_last_geometry_shader_bytecode = &s_pass_entry;
  s_last_pixel_shader_bytecode = nullptr;
  s_last_vertex_shader_bytecode = nullptr;
  s_last_pixel_uber_shader_bytecode = nullptr;
  s_last_vertex_uber_shader_bytecode = nullptr;

  s_last_geometry_shader_uid = {};
  s_last_pixel_shader_uid = {};
  s_last_vertex_shader_uid = {};
  s_last_tessellation_shader_uid = {};
  s_use_pixel_uber_shader = false;
  s_use_vertex_uber_shader = false;
}

static size_t shader_count = 0;

void ShaderCache::CompileUberShaders()
{
  shader_count = 0;
  UberShader::EnumerateVertexUberShaderUids([&](const UberShader::VertexUberShaderUid& uid, size_t total) {
    HandleVUSUIDChange(uid, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Vertex Uber shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total));
    });
  });
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
  shader_count = 0;
  UberShader::EnumeratePixelUberShaderUids([&](const UberShader::PixelUberShaderUid& uid, size_t total) {
    HandlePUSUIDChange(uid, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Pixel Uber shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total));
    });
  });
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void ShaderCache::CompileShaders()
{
  ts_bytecode_cache->ForEachMostUsedByCategory(gameid,
    [&](const TessellationShaderUid& it, size_t total)
  {
    TessellationShaderUid item = it;
    item.ClearHASH();
    item.CalculateUIDHash();
    HandleTSUIDChange(item, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Tessellation shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total * 2));
    });
  },
    [](std::pair<ByteCodeCacheEntry, ByteCodeCacheEntry>& entry)
  {
    return !entry.first.m_shader_bytecode.pShaderBytecode;
  }
  , true);
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void ShaderCache::CompileHostBasedShaders()
{
  shader_count = 0;
  ps_bytecode_cache->ForEachMostUsedByCategory(gameid,
    [&](const PixelShaderUid& it, size_t total)
  {
    PixelShaderUid item = it;
    item.ClearHASH();
    item.CalculateUIDHash();
    HandlePSUIDChange(item, true, [total]() {
      shader_count++;
      if ((shader_count & 7) == 0)
      {
        Host_UpdateProgressDialog(GetStringT("Compiling Pixel shaders...").c_str(),
          static_cast<int>(shader_count), static_cast<int>(total));
      }
    });
    
  },
    [](ByteCodeCacheEntry& entry)
  {
    return !entry.m_shader_bytecode.pShaderBytecode;
  }
  , true);
  s_compiler->WaitForFinish();
  shader_count = 0;
  vs_bytecode_cache->ForEachMostUsedByCategory(gameid,
    [&](const VertexShaderUid& it, size_t total)
  {
    VertexShaderUid item = it;
    item.ClearHASH();
    item.CalculateUIDHash();
    HandleVSUIDChange(item, true, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Vertex shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total));
    });
  },
    [](ByteCodeCacheEntry& entry)
  {
    return !entry.m_shader_bytecode.pShaderBytecode;
  }
  , true);
  s_compiler->WaitForFinish();
  shader_count = 0;
  EnumerateGeometryShaderUids([&](const GeometryShaderUid& it, size_t total)
  {
    GeometryShaderUid item = it;
    item.ClearHASH();
    item.CalculateUIDHash();
    HandleGSUIDChange(item, [total]() {
      shader_count++;
      if ((shader_count & 3) == 0)
      {
        Host_UpdateProgressDialog(GetStringT("Compiling Geometry shaders...").c_str(),
          static_cast<int>(shader_count), static_cast<int>(total));
      }
    });
    
  });
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void ShaderCache::LoadHostBasedFromDisk()
{
  if (vs_bytecode_cache)
  {
    vs_bytecode_cache->Persist([](VertexShaderUid& uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    delete vs_bytecode_cache;
    vs_bytecode_cache = nullptr;
  }
  if (ps_bytecode_cache)
  {
    ps_bytecode_cache->Persist([](PixelShaderUid &uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    delete ps_bytecode_cache;
    ps_bytecode_cache = nullptr;
  }
  pus_bytecode_cache.clear();
  vus_bytecode_cache.clear();
  gs_bytecode_cache.clear();

  for (auto& iter : s_host_blob_list)
    SAFE_RELEASE(iter);

  std::string title_unique_id = SConfig::GetInstance().GetGameID();

  vs_bytecode_cache = VsBytecodeCache::Create(
    gameid,
    VERTEXSHADERGEN_UID_VERSION,
    "Ishiiruka.vs",
    StringFromFormat("%s.vs", title_unique_id.c_str())
  );

  ps_bytecode_cache = PsBytecodeCache::Create(
    gameid,
    PIXELSHADERGEN_UID_VERSION,
    "Ishiiruka.ps",
    StringFromFormat("%s.ps", title_unique_id.c_str())
  );
  std::string pus_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "ups", false, true);
  std::string vus_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "uvs", false, true);
  std::string ps_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "ps", true, true);
  std::string vs_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "vs", true, true);
  std::string gs_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "gs", false, true);

  PixelUberShaderCacheInserter pus_inserter;
  s_pus_disk_cache.OpenAndRead(pus_cache_filename, pus_inserter);

  VertexUberShaderCacheInserter vus_inserter;
  s_vus_disk_cache.OpenAndRead(vus_cache_filename, vus_inserter);


  PixelShaderCacheInserter ps_inserter;
  s_ps_disk_cache.OpenAndRead(ps_cache_filename, ps_inserter);

  VertexShaderCacheInserter vs_inserter;
  s_vs_disk_cache.OpenAndRead(vs_cache_filename, vs_inserter);

  GeometryShaderCacheInserter gs_inserter;
  s_gs_disk_cache.OpenAndRead(gs_cache_filename, gs_inserter);
}

void ShaderCache::LoadFromDisk()
{
  std::string ds_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "ds", false, false);
  std::string hs_cache_filename = GetDiskShaderCacheFileName(API_D3D11, "hs", false, false);


  DShaderCacheInserter ds_inserter;
  s_ds_disk_cache.OpenAndRead(ds_cache_filename, ds_inserter);

  HShaderCacheInserter hs_inserter;
  s_hs_disk_cache.OpenAndRead(hs_cache_filename, hs_inserter);
}

void ShaderCache::Reload()
{
  if (s_compiler)
  {
    s_compiler->WaitForFinish();
  }
  s_gs_disk_cache.Sync();
  s_gs_disk_cache.Close();
  s_ps_disk_cache.Sync();
  s_ps_disk_cache.Close();
  s_vs_disk_cache.Sync();
  s_vs_disk_cache.Close();
  s_pus_disk_cache.Sync();;
  s_pus_disk_cache.Close();
  s_vus_disk_cache.Sync();
  s_vus_disk_cache.Close();
  LoadHostBasedFromDisk();
  if (g_ActiveConfig.CanPrecompileUberShaders())
  {
    CompileUberShaders();
  }
  if (g_ActiveConfig.bCompileShaderOnStartup && !g_ActiveConfig.bDisableSpecializedShaders)
  {
    CompileHostBasedShaders();
  }
  s_last_domain_shader_bytecode = &s_pass_entry;
  s_last_hull_shader_bytecode = &s_pass_entry;
  s_last_geometry_shader_bytecode = &s_pass_entry;
  s_last_pixel_shader_bytecode = nullptr;
  s_last_vertex_shader_bytecode = nullptr;
  s_last_pixel_uber_shader_bytecode = nullptr;
  s_last_vertex_uber_shader_bytecode = nullptr;

  s_last_geometry_shader_uid = {};
  s_last_pixel_shader_uid = {};
  s_last_vertex_shader_uid = {};
  s_last_tessellation_shader_uid = {};
  s_use_pixel_uber_shader = false;
  s_use_vertex_uber_shader = false;
}

void ShaderCache::Clear()
{

}

void ShaderCache::Shutdown()
{
  if (s_compiler)
  {
    s_compiler->WaitForFinish();
  }

  for (auto& iter : s_static_blob_list)
    SAFE_RELEASE(iter);

  s_static_blob_list.clear();

  for (auto& iter : s_host_blob_list)
    SAFE_RELEASE(iter);

  s_host_blob_list.clear();

  vs_bytecode_cache->Persist([](VertexShaderUid &uid) {
    uid.ClearHASH();
    uid.CalculateUIDHash();
  });
  delete vs_bytecode_cache;
  vs_bytecode_cache = nullptr;

  ts_bytecode_cache->Persist([](TessellationShaderUid &uid) {
    uid.ClearHASH();
    uid.CalculateUIDHash();
  });
  delete ts_bytecode_cache;
  ts_bytecode_cache = nullptr;

  ps_bytecode_cache->Persist([](PixelShaderUid &uid) {
    uid.ClearHASH();
    uid.CalculateUIDHash();
  });
  delete ps_bytecode_cache;
  ps_bytecode_cache = nullptr;

  s_ds_disk_cache.Sync();
  s_ds_disk_cache.Close();
  s_hs_disk_cache.Sync();
  s_hs_disk_cache.Close();
  s_gs_disk_cache.Sync();
  s_gs_disk_cache.Close();
  s_ps_disk_cache.Sync();
  s_ps_disk_cache.Close();
  s_vs_disk_cache.Sync();
  s_vs_disk_cache.Close();
  s_pus_disk_cache.Sync();
  s_pus_disk_cache.Close();
  s_vus_disk_cache.Sync();
  s_vus_disk_cache.Close();
}

static void PushHostByteCode(ByteCodeCacheEntry* entry, D3DBlob* shaderBuffer)
{
  s_host_blob_list.push_back(shaderBuffer);
  entry->m_shader_bytecode.pShaderBytecode = shaderBuffer->Data();
  entry->m_shader_bytecode.BytecodeLength = shaderBuffer->Size();
  entry->m_compiled = true;
}

static void PushStaticByteCode(ByteCodeCacheEntry* entry, D3DBlob* shaderBuffer)
{
  s_static_blob_list.push_back(shaderBuffer);
  entry->m_shader_bytecode.pShaderBytecode = shaderBuffer->Data();
  entry->m_shader_bytecode.BytecodeLength = shaderBuffer->Size();
  entry->m_compiled = true;
}

void ShaderCache::SetCurrentPrimitiveTopology(PrimitiveType gs_primitive_type)
{
  switch (gs_primitive_type)
  {
  case PrimitiveType::Triangles:
    s_current_primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    break;
  case PrimitiveType::Lines:
    s_current_primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
    break;
  case PrimitiveType::Points:
    s_current_primitive_topology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
    break;
  default:
    CHECK(0, "Invalid primitive type.");
    break;
  }
}

void ShaderCache::HandleGSUIDChange(const GeometryShaderUid &gs_uid, std::function<void()> oncompilationfinished = {})
{
  if (gs_uid.GetUidData().IsPassthrough())
  {
    s_last_geometry_shader_bytecode = &s_pass_entry;
    return;
  }

  ByteCodeCacheEntry* entry = &gs_bytecode_cache[gs_uid];
  s_last_geometry_shader_bytecode = entry;

  if (entry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }

  // Need to compile a new shader
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [gs_uid](ShaderCompilerWorkUnit* wunit)
  {
    GenerateGeometryShaderCode(wunit->code, gs_uid.GetUidData(), ShaderHostConfig::GetCurrent());
  };

  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  wunit->target = D3D::GeometryShaderVersionString();

  wunit->ResultHandler = [gs_uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_gs_disk_cache.Append(gs_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushHostByteCode(entry, shaderBuffer);
      SETSTAT(stats.numGeometryShadersAlive, static_cast<int>(ps_bytecode_cache->size()));
      INCSTAT(stats.numGeometryShadersCreated);
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_gs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile geometry shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::GeometryShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void ShaderCache::HandlePSUIDChange(const PixelShaderUid &ps_uid, bool forcecompile = false, std::function<void()> oncompilationfinished = {})
{
  ByteCodeCacheEntry* entry = &ps_bytecode_cache->GetOrAdd(ps_uid);
  if (g_ActiveConfig.bDisableSpecializedShaders && !forcecompile)
  {
    return;
  }
  s_last_pixel_shader_bytecode = entry;
  if (entry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  // Need to compile a new shader
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [ps_uid](ShaderCompilerWorkUnit* wunit)
  {
    GeneratePixelShaderCode(wunit->code, ps_uid.GetUidData(), ShaderHostConfig::GetCurrent());
  };

  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  wunit->target = D3D::PixelShaderVersionString();
  wunit->ResultHandler = [ps_uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_ps_disk_cache.Append(ps_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushHostByteCode(entry, shaderBuffer);
      wunit->shaderbytecode->Release();
      wunit->shaderbytecode = nullptr;
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_ps_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile pixel shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::PixelShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void ShaderCache::HandlePUSUIDChange(const UberShader::PixelUberShaderUid &ps_uid, std::function<void()> oncompilationfinished = {})
{
  ByteCodeCacheEntry* entry = &pus_bytecode_cache[ps_uid];
  s_last_pixel_uber_shader_bytecode = entry;
  if (entry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  // Need to compile a new shader
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [ps_uid](ShaderCompilerWorkUnit* wunit)
  {
    UberShader::GenPixelShader(wunit->code, API_D3D11, ShaderHostConfig::GetCurrent(), ps_uid.GetUidData());
  };

  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  wunit->target = D3D::PixelShaderVersionString();
  wunit->ResultHandler = [ps_uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_pus_disk_cache.Append(ps_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushHostByteCode(entry, shaderBuffer);
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_pus_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile pixel uber shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::PixelShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void ShaderCache::HandleVSUIDChange(const VertexShaderUid& vs_uid, bool forcecompile = false, std::function<void()> oncompilationfinished = {})
{
  ByteCodeCacheEntry* entry = &vs_bytecode_cache->GetOrAdd(vs_uid);
  if (g_ActiveConfig.bDisableSpecializedShaders && !forcecompile)
  {
    return;
  }
  s_last_vertex_shader_bytecode = entry;
  // Compile only when we have a new instance
  if (entry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [vs_uid](ShaderCompilerWorkUnit* wunit)
  {
    GenerateVertexShaderCode(wunit->code, vs_uid.GetUidData(), ShaderHostConfig::GetCurrent());
  };

  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
  wunit->target = D3D::VertexShaderVersionString();
  wunit->ResultHandler = [vs_uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_vs_disk_cache.Append(vs_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushHostByteCode(entry, shaderBuffer);
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_vs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char*)wunit->code.data());
      file.close();

      PanicAlert("Failed to compile vertex shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::VertexShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void ShaderCache::HandleVUSUIDChange(const UberShader::VertexUberShaderUid& vs_uid, std::function<void()> oncompilationfinished = {})
{
  ByteCodeCacheEntry* entry = &vus_bytecode_cache[vs_uid];
  s_last_vertex_uber_shader_bytecode = entry;
  // Compile only when we have a new instance
  if (entry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [vs_uid](ShaderCompilerWorkUnit* wunit)
  {
    UberShader::GenVertexShader(wunit->code, API_D3D11, ShaderHostConfig::GetCurrent(), vs_uid.GetUidData());
  };

  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
  wunit->target = D3D::VertexShaderVersionString();
  wunit->ResultHandler = [vs_uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_vus_disk_cache.Append(vs_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushHostByteCode(entry, shaderBuffer);
      wunit->shaderbytecode->Release();
      wunit->shaderbytecode = nullptr;
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_vus_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char*)wunit->code.data());
      file.close();

      PanicAlert("Failed to compile vertex Uber shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::VertexShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void ShaderCache::HandleTSUIDChange(const TessellationShaderUid& ts_uid, std::function<void()> oncompilationfinished = {})
{
  std::pair<ByteCodeCacheEntry, ByteCodeCacheEntry>& entry = ts_bytecode_cache->GetOrAdd(ts_uid);
  ByteCodeCacheEntry* dentry = &entry.first;
  ByteCodeCacheEntry* hentry = &entry.second;
  if (dentry->m_compiled && hentry->m_compiled)
  {
    s_last_domain_shader_bytecode = dentry;
    s_last_hull_shader_bytecode = hentry;
  }
  else
  {
    s_last_tessellation_shader_uid = {};
    s_last_domain_shader_bytecode = &s_pass_entry;
    s_last_hull_shader_bytecode = &s_pass_entry;
  }
  if (dentry->m_initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  hentry->m_initialized.test_and_set();

  // Need to compile a new shader
  ShaderCode code;
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  ShaderCompilerWorkUnit *wunitd = s_compiler->NewUnit();
  GenerateTessellationShaderCode(wunit->code, API_D3D11, ts_uid.GetUidData());
  wunitd->code.copy(wunit->code);

  wunit->entrypoint = "HS_TFO";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_SKIP_OPTIMIZATION;
  wunit->target = D3D::HullShaderVersionString();

  wunitd->entrypoint = "DS_TFO";
  wunitd->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  wunitd->target = D3D::DomainShaderVersionString();

  wunitd->ResultHandler = [ts_uid, dentry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_ds_disk_cache.Append(ts_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushStaticByteCode(dentry, shaderBuffer);
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_ds_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile domain shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::DomainShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };

  wunit->ResultHandler = [ts_uid, hentry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      D3DBlob* shaderBuffer = new D3DBlob(wunit->shaderbytecode);
      s_hs_disk_cache.Append(ts_uid, shaderBuffer->Data(), shaderBuffer->Size());
      PushStaticByteCode(hentry, shaderBuffer);
      wunit->shaderbytecode->Release();
      wunit->shaderbytecode = nullptr;
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_hs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile hull shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::HullShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };

  s_compiler->CompileShaderAsync(wunit);
  s_compiler->CompileShaderAsync(wunitd);
}

void ShaderCache::PrepareShaders(PIXEL_SHADER_RENDER_MODE render_mode,
  PrimitiveType gs_primitive_type,
  u32 components,
  const XFMemory &xfr,
  const BPMemory &bpm)
{
  SetCurrentPrimitiveTopology(gs_primitive_type);
  if (g_ActiveConfig.bBackgroundShaderCompiling || g_ActiveConfig.bDisableSpecializedShaders)
  {
    auto vusid = UberShader::GetVertexUberShaderUid(components, xfr);
    if (!s_last_vertex_uber_shader_bytecode || s_last_vertex_uber_shader_uid != vusid)
    {
      D3D::command_list_mgr->SetCommandListDirtyState(COMMAND_LIST_STATE_PSO, true);
      s_last_vertex_uber_shader_uid = vusid;
      HandleVUSUIDChange(vusid);
    }
    auto pusid = UberShader::GetPixelUberShaderUid(components, xfr, bpm);
    if (!s_last_pixel_uber_shader_bytecode || s_last_pixel_uber_shader_uid != pusid)
    {
      s_last_pixel_uber_shader_uid = pusid;
      D3D::command_list_mgr->SetCommandListDirtyState(COMMAND_LIST_STATE_PSO, true);
      HandlePUSUIDChange(pusid);
    }
  }
  GeometryShaderUid gs_uid;
  GetGeometryShaderUid(gs_uid, gs_primitive_type, xfr, components);
  PixelShaderUid ps_uid;
  GetPixelShaderUID(ps_uid, render_mode, components, xfr, bpm);
  VertexShaderUid vs_uid;
  GetVertexShaderUID(vs_uid, components, xfr, bpm);
  TessellationShaderUid ts_uid;
  bool tessellationenabled = false;
  if (gs_primitive_type == PrimitiveType::Triangles
    && g_ActiveConfig.TessellationEnabled()
    && xfr.projection.type == GX_PERSPECTIVE
    && (g_ActiveConfig.bForcedLighting || g_ActiveConfig.PixelLightingEnabled(xfr, components)))
  {
    GetTessellationShaderUID(ts_uid, xfr, bpm, components);
    tessellationenabled = true;
  }

  bool gs_changed = false;
  bool ps_changed = false;
  bool vs_changed = false;
  bool ts_changed = false;

  gs_changed = gs_uid != s_last_geometry_shader_uid;
  ps_changed = ps_uid != s_last_pixel_shader_uid;
  vs_changed = vs_uid != s_last_vertex_shader_uid;
  ts_changed = tessellationenabled && ts_uid != s_last_tessellation_shader_uid;

  if (!gs_changed && !ps_changed && !vs_changed && !ts_changed)
  {
    return;
  }

  if (gs_changed)
  {
    s_last_geometry_shader_uid = gs_uid;
  }

  if (ps_changed)
  {
    s_last_pixel_shader_uid = ps_uid;
  }

  if (vs_changed)
  {
    s_last_vertex_shader_uid = vs_uid;
  }
  if (ts_changed)
  {
    s_last_tessellation_shader_uid = ts_uid;
  }
  // A Uid has changed, so the PSO will need to be reset at next ApplyState.
  D3D::command_list_mgr->SetCommandListDirtyState(COMMAND_LIST_STATE_PSO, true);

  if (vs_changed)
  {
    HandleVSUIDChange(vs_uid);
  }

  if (ts_changed)
  {
    HandleTSUIDChange(ts_uid);
  }
  else
  {
    s_last_domain_shader_bytecode = &s_pass_entry;
    s_last_hull_shader_bytecode = &s_pass_entry;
  }

  if (gs_changed)
  {
    HandleGSUIDChange(gs_uid);
  }

  if (ps_changed)
  {
    HandlePSUIDChange(ps_uid);
  }
}

bool ShaderCache::TestShaders()
{
  s_compiler->ProcCompilationResults();  
  if (g_ActiveConfig.bBackgroundShaderCompiling || g_ActiveConfig.bDisableSpecializedShaders)
  {
    s_use_pixel_uber_shader = g_ActiveConfig.bDisableSpecializedShaders
      || s_last_pixel_shader_bytecode == nullptr
      || !s_last_pixel_shader_bytecode->m_compiled;
    s_use_vertex_uber_shader = g_ActiveConfig.bDisableSpecializedShaders
      || s_last_vertex_shader_bytecode == nullptr
      || !s_last_vertex_shader_bytecode->m_compiled;
    return true;
  }
  else
  {
    s_use_pixel_uber_shader = false;
    s_use_vertex_uber_shader = false;
  }
  bool shaders_available = !(s_last_geometry_shader_bytecode == nullptr
    || s_last_pixel_shader_bytecode == nullptr
    || s_last_vertex_shader_bytecode == nullptr);
  if (shaders_available)
  {
    int count = 0;
    while (!(s_last_geometry_shader_bytecode->m_compiled
      && s_last_pixel_shader_bytecode->m_compiled
      && s_last_vertex_shader_bytecode->m_compiled))
    {
      s_compiler->ProcCompilationResults();
      if (g_ActiveConfig.bFullAsyncShaderCompilation)
      {
        break;
      }
      Common::cYield(count++);
    }
    shaders_available = s_last_geometry_shader_bytecode->m_compiled
      && s_last_pixel_shader_bytecode->m_compiled
      && s_last_vertex_shader_bytecode->m_compiled;
  }
  return shaders_available;
}

void ShaderCache::InsertDSByteCode(const TessellationShaderUid & uid, D3DBlob * bytecode_blob)
{
  s_static_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = &ts_bytecode_cache->GetOrAdd(uid).first;
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

void ShaderCache::InsertHSByteCode(const TessellationShaderUid & uid, D3DBlob * bytecode_blob)
{
  s_static_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = &ts_bytecode_cache->GetOrAdd(uid).second;
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

void ShaderCache::InsertGSByteCode(const GeometryShaderUid& uid, D3DBlob * bytecode_blob)
{
  s_host_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry& entry = gs_bytecode_cache[uid];
  entry.m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry.m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry.m_compiled = true;
  entry.m_initialized.test_and_set();
}

void ShaderCache::InsertVSByteCode(const VertexShaderUid& uid, D3DBlob* bytecode_blob)
{
  s_host_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = entry = &vs_bytecode_cache->GetOrAdd(uid);
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

void ShaderCache::InsertVUSByteCode(const UberShader::VertexUberShaderUid& uid, D3DBlob* bytecode_blob)
{
  s_host_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = entry = &vus_bytecode_cache[uid];
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

void ShaderCache::InsertPSByteCode(const PixelShaderUid& uid, D3DBlob* bytecode_blob)
{
  s_host_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = entry = &ps_bytecode_cache->GetOrAdd(uid);
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

void ShaderCache::InsertPUSByteCode(const UberShader::PixelUberShaderUid& uid, D3DBlob* bytecode_blob)
{
  s_host_blob_list.push_back(bytecode_blob);
  ByteCodeCacheEntry* entry = entry = &pus_bytecode_cache[uid];
  entry->m_shader_bytecode.pShaderBytecode = bytecode_blob->Data();
  entry->m_shader_bytecode.BytecodeLength = bytecode_blob->Size();
  entry->m_compiled = true;
  entry->m_initialized.test_and_set();
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE ShaderCache::GetCurrentPrimitiveTopology()
{
  return s_current_primitive_topology;
}

D3D12_SHADER_BYTECODE ShaderCache::GetActiveDomainShaderBytecode()
{
  return s_current_primitive_topology == D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
    && s_last_hull_shader_bytecode != nullptr && s_last_hull_shader_bytecode->m_compiled
    && s_last_domain_shader_bytecode->m_compiled ? s_last_domain_shader_bytecode->m_shader_bytecode : s_pass_entry.m_shader_bytecode;
}
D3D12_SHADER_BYTECODE ShaderCache::GetActiveHullShaderBytecode()
{
  return s_current_primitive_topology == D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE
    && s_last_hull_shader_bytecode->m_compiled
    && s_last_domain_shader_bytecode->m_compiled ? s_last_hull_shader_bytecode->m_shader_bytecode : s_pass_entry.m_shader_bytecode;
}
D3D12_SHADER_BYTECODE ShaderCache::GetActiveGeometryShaderBytecode()
{
  return s_last_geometry_shader_bytecode->m_shader_bytecode;
}
D3D12_SHADER_BYTECODE ShaderCache::GetActivePixelShaderBytecode()
{
  return s_last_pixel_shader_bytecode->m_shader_bytecode;
}
D3D12_SHADER_BYTECODE ShaderCache::GetActiveVertexShaderBytecode()
{
  return s_last_vertex_shader_bytecode->m_shader_bytecode;
}

D3D12_SHADER_BYTECODE ShaderCache::GetActivePixelUberShaderBytecode()
{
  return s_last_pixel_uber_shader_bytecode->m_shader_bytecode;
}
D3D12_SHADER_BYTECODE ShaderCache::GetActiveVertexUberShaderBytecode()
{
  return s_last_vertex_uber_shader_bytecode->m_shader_bytecode;
}

const GeometryShaderUid& ShaderCache::GetActiveGeometryShaderUid()
{
  return s_last_geometry_shader_uid;
}
const PixelShaderUid& ShaderCache::GetActivePixelShaderUid()
{
  return s_last_pixel_shader_uid;
}
const VertexShaderUid& ShaderCache::GetActiveVertexShaderUid()
{
  return s_last_vertex_shader_uid;
}
const TessellationShaderUid& ShaderCache::GetActiveTessellationShaderUid()
{
  return s_last_tessellation_shader_uid;
}

const UberShader::PixelUberShaderUid& ShaderCache::GetActivePixelUberShaderUid()
{
  return s_last_pixel_uber_shader_uid;
}
const UberShader::VertexUberShaderUid& ShaderCache::GetActiveVertexUberShaderUid()
{
  return s_last_vertex_uber_shader_uid;
}

static const D3D12_SHADER_BYTECODE empty = { 0 };

D3D12_SHADER_BYTECODE ShaderCache::GetDomainShaderFromUid(const TessellationShaderUid& uid)
{
  auto it = ts_bytecode_cache->GetInfoIfexists(uid);
  if (it != nullptr)
    return it->first.m_shader_bytecode;

  return empty;
}
D3D12_SHADER_BYTECODE ShaderCache::GetHullShaderFromUid(const TessellationShaderUid& uid)
{
  auto it = ts_bytecode_cache->GetInfoIfexists(uid);
  if (it != nullptr)
    return it->second.m_shader_bytecode;

  return empty;
}
D3D12_SHADER_BYTECODE ShaderCache::GetGeometryShaderFromUid(const GeometryShaderUid& uid)
{
  auto it = gs_bytecode_cache.find(uid);
  if (it != gs_bytecode_cache.end())
    return it->second.m_shader_bytecode;

  return empty;
}
D3D12_SHADER_BYTECODE ShaderCache::GetPixelShaderFromUid(const PixelShaderUid& uid)
{
  auto it = ps_bytecode_cache->GetInfoIfexists(uid);
  if (it != nullptr)
    return it->m_shader_bytecode;

  return empty;
}

D3D12_SHADER_BYTECODE ShaderCache::GetVertexShaderFromUid(const VertexShaderUid& uid)
{
  auto it = vs_bytecode_cache->GetInfoIfexists(uid);
  if (it != nullptr)
    return it->m_shader_bytecode;

  return empty;
}

D3D12_SHADER_BYTECODE ShaderCache::GetPixelUberShaderFromUid(const UberShader::PixelUberShaderUid& uid)
{
  auto it = pus_bytecode_cache.find(uid);
  if (it != pus_bytecode_cache.end())
    return it->second.m_shader_bytecode;

  return empty;
}

D3D12_SHADER_BYTECODE ShaderCache::GetVertexUberShaderFromUid(const UberShader::VertexUberShaderUid& uid)
{
  auto it = vus_bytecode_cache.find(uid);
  if (it != vus_bytecode_cache.end())
    return it->second.m_shader_bytecode;

  return empty;
}
}
