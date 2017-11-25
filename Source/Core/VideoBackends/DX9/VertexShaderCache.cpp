// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
#include "Common/Common.h"
#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"

#include "Core/ConfigManager.h"
#include "Core/Host.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/HLSLCompiler.h"

#include "VideoBackends/DX9/D3DBase.h"
#include "VideoBackends/DX9/D3DShader.h"
#include "VideoBackends/DX9/VertexShaderCache.h"


namespace DX9
{

ObjectUsageProfiler<VertexShaderUid, pKey_t, VertexShaderCache::VSCacheEntry, VertexShaderUid::ShaderUidHasher>* VertexShaderCache::s_vshaders = nullptr;
const VertexShaderCache::VSCacheEntry *VertexShaderCache::s_last_entry;
VertexShaderUid VertexShaderCache::s_last_uid;


static HLSLAsyncCompiler *s_compiler;
#define MAX_SSAA_SHADERS 2

static LPDIRECT3DVERTEXSHADER9 s_simple_vertex_shaders[MAX_SSAA_SHADERS];
static LPDIRECT3DVERTEXSHADER9 s_clear_vertex_shader;

LinearDiskCache<VertexShaderUid, u8> g_vs_disk_cache;

LPDIRECT3DVERTEXSHADER9 VertexShaderCache::GetSimpleVertexShader(int level)
{
  return s_simple_vertex_shaders[level ? 1 : 0];
}

LPDIRECT3DVERTEXSHADER9 VertexShaderCache::GetClearVertexShader()
{
  return s_clear_vertex_shader;
}

// this class will load the precompiled shaders into our cache
class VertexShaderCacheInserter : public LinearDiskCacheReader<VertexShaderUid, u8>
{
public:
  void Read(const VertexShaderUid &key, const u8 *value, u32 value_size)
  {
    VertexShaderUid uid = key;
    uid.ClearHASH();
    uid.CalculateUIDHash();
    VertexShaderCache::InsertByteCode(key, value, value_size);
  }
};

void VertexShaderCache::Init()
{
  s_compiler = &HLSLAsyncCompiler::getInstance();
  const char* code = "struct VSOUTPUT\n"
    "{\n"
    "float4 vPosition : POSITION;\n"
    "float2 vTexCoord : TEXCOORD0;\n"
    "float vTexCoord1 : TEXCOORD1;\n"
    "};\n"
    "VSOUTPUT main(float4 inPosition : POSITION,float2 inTEX0 : TEXCOORD0,float2 inTEX1 : TEXCOORD1,float inTEX2 : TEXCOORD2)\n"
    "{\n"
    "VSOUTPUT OUT;\n"
    "OUT.vPosition = inPosition;\n"
    "OUT.vTexCoord = inTEX0;\n"
    "OUT.vTexCoord1 = inTEX2;\n"
    "return OUT;\n"
    "}\0";

  s_simple_vertex_shaders[0] = D3D::CompileAndCreateVertexShader(code, (int)strlen(code));

  code = "struct VSOUTPUT\n"
    "{\n"
    "float4 vPosition   : POSITION;\n"
    "float4 vColor0   : COLOR0;\n"
    "};\n"
    "VSOUTPUT main(float4 inPosition : POSITION,float4 inColor0: COLOR0)\n"
    "{\n"
    "VSOUTPUT OUT;\n"
    "OUT.vPosition = inPosition;\n"
    "OUT.vColor0 = inColor0;\n"
    "return OUT;\n"
    "}\0";

  s_clear_vertex_shader = D3D::CompileAndCreateVertexShader(code, (int)strlen(code));
  code = "struct VSOUTPUT\n"
    "{\n"
    "float4 vPosition   : POSITION;\n"
    "float4 vTexCoord   : TEXCOORD0;\n"
    "float  vTexCoord1   : TEXCOORD1;\n"
    "float4 vTexCoord2   : TEXCOORD2;\n"
    "float4 vTexCoord3   : TEXCOORD3;\n"
    "};\n"
    "VSOUTPUT main(float4 inPosition : POSITION,float2 inTEX0 : TEXCOORD0,float2 inTEX1 : TEXCOORD1,float inTEX2 : TEXCOORD2)\n"
    "{\n"
    "VSOUTPUT OUT;"
    "OUT.vPosition = inPosition;\n"
    "OUT.vTexCoord  = inTEX0.xyyx;\n"
    "OUT.vTexCoord1 = inTEX2.x;\n"
    "OUT.vTexCoord2 = inTEX0.xyyx + (float4(-0.375f,-0.125f,-0.375f, 0.125f) * inTEX1.xyyx);\n"
    "OUT.vTexCoord3 = inTEX0.xyyx + (float4( 0.375f, 0.125f, 0.375f,-0.125f) * inTEX1.xyyx);\n"
    "return OUT;\n"
    "}\0";
  s_simple_vertex_shaders[1] = D3D::CompileAndCreateVertexShader(code, (int)strlen(code));

  Clear();

  s_vshaders = nullptr;

  LoadFromDisk();

  if (s_vshaders && g_ActiveConfig.bCompileShaderOnStartup)
  {
    CompileShaders();
  }
  s_last_entry = nullptr;
  SETSTAT(stats.numVertexShadersAlive, s_vshaders->size());
  SETSTAT(stats.numVertexShadersCreated, 0);
}

void VertexShaderCache::LoadFromDisk()
{
  if (s_vshaders)
  {
    s_vshaders->Persist([](VertexShaderUid &uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    s_vshaders->Clear([](auto& item)
    {
      item.Destroy();
    });
    delete s_vshaders;
    s_vshaders = nullptr;
  }
  pKey_t gameid = (pKey_t)GetMurmurHash3(reinterpret_cast<const u8*>(SConfig::GetInstance().GetGameID().data()), (u32)SConfig::GetInstance().GetGameID().size(), 0);
  s_vshaders = ObjectUsageProfiler<VertexShaderUid, pKey_t, VertexShaderCache::VSCacheEntry, VertexShaderUid::ShaderUidHasher>::Create(
    gameid,
    VERTEXSHADERGEN_UID_VERSION,
    "Ishiiruka.vs",
    StringFromFormat("%s.vs", SConfig::GetInstance().GetGameID().c_str())
  );
  std::string cache_filename = GetDiskShaderCacheFileName(API_D3D9, "vs", true, true);
  VertexShaderCacheInserter inserter;
  g_vs_disk_cache.OpenAndRead(cache_filename, inserter);
}

void VertexShaderCache::CompileShaders()
{
  pKey_t gameid = (pKey_t)GetMurmurHash3(reinterpret_cast<const u8*>(SConfig::GetInstance().GetGameID().data()), (u32)SConfig::GetInstance().GetGameID().size(), 0);
  size_t shader_count = 0;
  s_vshaders->ForEachMostUsedByCategory(gameid,
    [&](const VertexShaderUid& item, size_t total)
  {
    VertexShaderUid newitem = item;
    newitem.ClearHASH();
    newitem.CalculateUIDHash();
    CompileVShader(newitem);
    shader_count++;
    Host_UpdateProgressDialog(GetStringT("Compiling Vertex shaders...").c_str(),
      static_cast<int>(shader_count), static_cast<int>(total));
  },
    [](VSCacheEntry& entry)
  {
    return !entry.shader;
  }, true);
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void VertexShaderCache::Reload()
{
  s_compiler->WaitForFinish();
  g_vs_disk_cache.Sync();
  g_vs_disk_cache.Close();
  LoadFromDisk();
  CompileShaders();
  s_last_entry = nullptr;
  s_last_uid = {};
}

void VertexShaderCache::Clear()
{
  if (s_vshaders)
  {
    if (s_compiler)
    {
      s_compiler->WaitForFinish();
    }
    s_vshaders->Persist([](VertexShaderUid &uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    s_vshaders->Clear([](auto& item)
    {
      item.Destroy();
    });
  }
  s_last_entry = nullptr;
  s_last_uid = {};
}

void VertexShaderCache::Shutdown()
{
  if (s_compiler)
  {
    s_compiler->WaitForFinish();
  }
  for (int i = 0; i < MAX_SSAA_SHADERS; i++)
  {
    if (s_simple_vertex_shaders[i])
      s_simple_vertex_shaders[i]->Release();
    s_simple_vertex_shaders[i] = NULL;
  }

  if (s_clear_vertex_shader)
    s_clear_vertex_shader->Release();
  s_clear_vertex_shader = NULL;

  Clear();
  delete s_vshaders;
  s_vshaders = nullptr;
  g_vs_disk_cache.Sync();
  g_vs_disk_cache.Close();
}

void VertexShaderCache::CompileVShader(const VertexShaderUid& uid)
{
  VSCacheEntry *entry = &s_vshaders->GetOrAdd(uid);
  s_last_entry = entry;
  // Compile only when we have a new instance
  if (entry->initialized.test_and_set())
  {
    return;
  }
  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  const ShaderHostConfig& hostconfig = ShaderHostConfig::GetCurrent();
  wunit->GenerateCodeHandler = [uid, hostconfig](ShaderCompilerWorkUnit* wunit)
  {
    GenerateVertexShaderCode(wunit->code, uid.GetUidData(), hostconfig);
  };
  wunit->entrypoint = "main";
  wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  wunit->target = D3D::VertexShaderVersionString();
  wunit->ResultHandler = [uid, entry](ShaderCompilerWorkUnit* wunit)
  {
    if (SUCCEEDED(wunit->cresult))
    {
      ID3DBlob* shaderBuffer = wunit->shaderbytecode;
      const u8* bytecode = (const u8*)shaderBuffer->GetBufferPointer();
      u32 bytecodelen = (u32)shaderBuffer->GetBufferSize();
      g_vs_disk_cache.Append(uid, bytecode, bytecodelen);
      PushByteCode(uid, bytecode, bytecodelen, entry);
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_vs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char*)wunit->code.data());
      file << ((const char*)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile vertex shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::VertexShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void VertexShaderCache::PrepareShader(u32 components, const XFMemory &xfr, const BPMemory &bpm)
{
  VertexShaderUid uid;
  GetVertexShaderUID(uid, components, xfr, bpm);
  s_compiler->ProcCompilationResults();
  if (s_last_entry && uid == s_last_uid)
  {
    return;
  }
  s_last_uid = uid;
  GFX_DEBUGGER_PAUSE_AT(NEXT_VERTEX_SHADER_CHANGE, true);
  CompileVShader(uid);
}

bool VertexShaderCache::TestShader()
{
  int count = 0;
  while (!s_last_entry->compiled)
  {
    s_compiler->ProcCompilationResults();
    if (g_ActiveConfig.bFullAsyncShaderCompilation)
    {
      break;
    }
    Common::cYield(count++);
  }
  if (s_last_entry->shader && s_last_entry->compiled)
  {
    D3D::SetVertexShader(s_last_entry->shader);
    return true;
  }
  return false;
}

void VertexShaderCache::PushByteCode(const VertexShaderUid &uid, const u8 *bytecode, int bytecodelen, VertexShaderCache::VSCacheEntry* entry)
{
  entry->shader = D3D::CreateVertexShaderFromByteCode(bytecode, bytecodelen);
  entry->compiled = true;
  if (entry->shader)
  {
    INCSTAT(stats.numVertexShadersCreated);
    SETSTAT(stats.numVertexShadersAlive, (int)s_vshaders->size());
  }
}

void VertexShaderCache::InsertByteCode(const VertexShaderUid &uid, const u8 *bytecode, int bytecodelen)
{
  VSCacheEntry *entry = &s_vshaders->GetOrAdd(uid);
  entry->initialized.test_and_set();
  PushByteCode(uid, bytecode, bytecodelen, entry);
}

}  // namespace DX9
