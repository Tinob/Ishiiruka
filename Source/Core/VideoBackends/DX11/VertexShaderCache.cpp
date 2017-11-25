// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Common/CommonTypes.h"
#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"

#include "Core/ConfigManager.h"
#include "Core/Host.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/HLSLCompiler.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/VertexShaderManager.h"

#include "VideoBackends/DX11/D3DShader.h"
#include "VideoBackends/DX11/D3DUtil.h"
#include "VideoBackends/DX11/VertexShaderCache.h"

namespace DX11 {

VertexShaderCache::VSCache* VertexShaderCache::s_vshaders;
VertexShaderCache::VUSCache VertexShaderCache::s_vuber_shaders;
const VertexShaderCache::VSCacheEntry *VertexShaderCache::s_last_entry;
const VertexShaderCache::VSCacheEntry *VertexShaderCache::s_last_uber_entry;
VertexShaderUid VertexShaderCache::s_last_uid;
UberShader::VertexUberShaderUid VertexShaderCache::s_last_uber_uid;

static HLSLAsyncCompiler *s_compiler;

static D3D::VertexShaderPtr s_simple_vertex_shader;
static D3D::VertexShaderPtr s_clear_vertex_shader;
static D3D::InputLayoutPtr s_simple_layout;
static D3D::InputLayoutPtr s_clear_layout;

LinearDiskCache<VertexShaderUid, u8> g_vs_disk_cache;
LinearDiskCache<UberShader::VertexUberShaderUid, u8> g_vus_disk_cache;

ID3D11VertexShader* VertexShaderCache::GetSimpleVertexShader()
{
  return s_simple_vertex_shader.get();
}
ID3D11VertexShader* VertexShaderCache::GetClearVertexShader()
{
  return s_clear_vertex_shader.get();
}
ID3D11InputLayout* VertexShaderCache::GetSimpleInputLayout()
{
  return s_simple_layout.get();
}
ID3D11InputLayout* VertexShaderCache::GetClearInputLayout()
{
  return s_clear_layout.get();
}

D3D::ConstantStreamBuffer* vscbuf = nullptr;

D3D::BufferDescriptor VertexShaderCache::GetConstantBuffer()
{
  // TODO: divide the global variables of the generated shaders into about 5 constant buffers to speed this up
  if (VertexShaderManager::IsDirty())
  {
    const size_t size = sizeof(float) * VertexShaderManager::ConstantBufferSize;
    vscbuf->AppendData((void*)VertexShaderManager::GetBuffer(), size);
    VertexShaderManager::Clear();
    ADDSTAT(stats.thisFrame.bytesUniformStreamed, size);
  }
  return vscbuf->GetDescriptor();
}

// this class will load the precompiled shaders into our cache
class VertexShaderCacheInserter : public LinearDiskCacheReader<VertexShaderUid, u8>
{
public:
  void Read(const VertexShaderUid &key, const u8 *value, u32 value_size)
  {
    VertexShaderCache::InsertByteCode(key, D3DBlob(value_size, value));
  }
};

class VertexUberShaderCacheInserter : public LinearDiskCacheReader<UberShader::VertexUberShaderUid, u8>
{
public:
  void Read(const UberShader::VertexUberShaderUid &key, const u8 *value, u32 value_size)
  {
    VertexShaderCache::InsertByteCode(key, D3DBlob(value_size, value));
  }
};

const char* simple_shader_code = R"hlsl(
struct VSOUTPUT
{
	float4 vPosition : SV_Position;
	float3 vTexCoord : TEXCOORD0;
	float vTexCoord1 : TEXCOORD1;
	float4 vTexCoord2 : TEXCOORD2;
	float4 vTexCoord3 : TEXCOORD3;
};
VSOUTPUT main(float4 inPosition : POSITION,float3 inTEX0 : TEXCOORD0, float3 inTEX1 : TEXCOORD1)
{
	VSOUTPUT OUT;
	OUT.vPosition = inPosition;
	OUT.vTexCoord = inTEX0;
	OUT.vTexCoord1 = inTEX1.z;
	OUT.vTexCoord2 = inTEX0.xyyx + (float4(-0.375f,-0.125f,-0.375f, 0.125f) * inTEX1.xyyx);
	OUT.vTexCoord3 = inTEX0.xyyx + (float4( 0.375f, 0.125f, 0.375f,-0.125f) * inTEX1.xyyx);
	return OUT;
})hlsl";

const char* clear_shader_code = R"hlsl(
struct VSOUTPUT
{
	float4 vPosition : SV_Position;
	float4 vColor0   : COLOR0;
};
VSOUTPUT main(float4 inPosition : POSITION,float4 inColor0: COLOR0)
{
	VSOUTPUT OUT;
	OUT.vPosition = inPosition;
	OUT.vColor0 = inColor0;
	return OUT;
})hlsl";


void VertexShaderCache::Init()
{
  s_compiler = &HLSLAsyncCompiler::getInstance();
  const D3D11_INPUT_ELEMENT_DESC simpleelems[3] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };
  const D3D11_INPUT_ELEMENT_DESC clearelems[2] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  bool use_partial_buffer_update = D3D::SupportPartialContantBufferUpdate();
  u32 cbsize = VertexShaderManager::ConstantBufferSize * sizeof(float) * (use_partial_buffer_update ? 1024 : 1); // is always multiple of 16
  vscbuf = new D3D::ConstantStreamBuffer(cbsize);
  ID3D11Buffer* buf = vscbuf->GetBuffer();

  CHECK(buf != nullptr, "Create vertex shader constant buffer (size=%u)", cbsize);
  D3D::SetDebugObjectName(buf, "vertex shader constant buffer used to emulate the GX pipeline");

  D3DBlob blob;
  D3D::CompileShader(D3D::ShaderType::Vertex, simple_shader_code, blob);
  D3D::device->CreateInputLayout(simpleelems, 3, blob.Data(), blob.Size(), D3D::ToAddr(s_simple_layout));
  s_simple_vertex_shader = D3D::CreateVertexShaderFromByteCode(blob);
  if (s_simple_layout == nullptr ||
    s_simple_vertex_shader == nullptr)
    PanicAlert("Failed to create simple vertex shader or input layout at %s %d\n", __FILE__, __LINE__);

  D3D::SetDebugObjectName(s_simple_layout.get(), "simple input layout");
  D3D::SetDebugObjectName(s_simple_vertex_shader.get(), "simple vertex shader");

  D3D::CompileShader(D3D::ShaderType::Vertex, clear_shader_code, blob);
  D3D::device->CreateInputLayout(clearelems, 2, blob.Data(), blob.Size(), D3D::ToAddr(s_clear_layout));
  s_clear_vertex_shader = D3D::CreateVertexShaderFromByteCode(blob);
  if (s_clear_layout == nullptr ||
    s_clear_vertex_shader == nullptr)
    PanicAlert("Failed to create clear vertex shader or input layout at %s %d\n", __FILE__, __LINE__);
  D3D::SetDebugObjectName(s_clear_vertex_shader.get(), "clear vertex shader");
  D3D::SetDebugObjectName(s_clear_layout.get(), "clear input layout");

  s_vshaders = nullptr;
  LoadFromDisk();
  if (g_ActiveConfig.CanPrecompileUberShaders())
  {
    CompileUberShaders();
  }
  if (g_ActiveConfig.bCompileShaderOnStartup && !g_ActiveConfig.bDisableSpecializedShaders)
  {
    CompileShaders();
  }
  s_last_entry = nullptr;
  s_last_uber_entry = nullptr;
  SETSTAT(stats.numVertexShadersCreated, 0);
  SETSTAT(stats.numVertexShadersAlive, static_cast<int>(s_vshaders->size()));
}

void VertexShaderCache::LoadFromDisk()
{
  if (s_vshaders)
  {
    s_vshaders->Persist([](VertexShaderUid &uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    s_vshaders->Clear([](VSCacheEntry& item)
    {
      item.Destroy();
    });
    delete s_vshaders;
    s_vshaders = nullptr;
  }
  for (auto& item : s_vuber_shaders)
    item.second.Destroy();
  s_vuber_shaders.clear();
  pKey_t gameid = (pKey_t)GetMurmurHash3(reinterpret_cast<const u8*>(SConfig::GetInstance().GetGameID().data()), (u32)SConfig::GetInstance().GetGameID().size(), 0);
  s_vshaders = VSCache::Create(
    gameid,
    VERTEXSHADERGEN_UID_VERSION,
    "Ishiiruka.vs",
    StringFromFormat("%s.vs", SConfig::GetInstance().GetGameID().c_str())
  );
  std::string cache_filename = GetDiskShaderCacheFileName(API_D3D11, "uvs", false, true);
  VertexUberShaderCacheInserter uinserter;
  g_vus_disk_cache.OpenAndRead(cache_filename, uinserter);
  cache_filename = GetDiskShaderCacheFileName(API_D3D11, "vs", true, true);
  VertexShaderCacheInserter inserter;
  g_vs_disk_cache.OpenAndRead(cache_filename, inserter);
}

static size_t shader_count = 0;

void VertexShaderCache::CompileShaders()
{
  pKey_t gameid = (pKey_t)GetMurmurHash3(reinterpret_cast<const u8*>(SConfig::GetInstance().GetGameID().data()), (u32)SConfig::GetInstance().GetGameID().size(), 0);
  shader_count = 0;
  const ShaderHostConfig& hostconfig = ShaderHostConfig::GetCurrent();
  s_vshaders->ForEachMostUsedByCategory(gameid,
    [&](const VertexShaderUid& it, size_t total)
  {
    VertexShaderUid item = it;
    item.ClearHASH();
    item.CalculateUIDHash();
    CompileVShader(item, hostconfig, true, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Vertex shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total));
    });
    
  },
    [](VSCacheEntry& entry)
  {
    return !entry.shader;
  }
  , true);
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void VertexShaderCache::CompileUberShaders()
{
  const ShaderHostConfig& hostconfig = ShaderHostConfig::GetCurrent();
  shader_count = 0;
  UberShader::EnumerateVertexUberShaderUids([&](const UberShader::VertexUberShaderUid& uid, size_t total) {
    CompileUberShader(uid, hostconfig, [total]() {
      shader_count++;
      Host_UpdateProgressDialog(GetStringT("Compiling Vertex Uber shaders...").c_str(),
        static_cast<int>(shader_count), static_cast<int>(total));
    });
  });
  s_compiler->WaitForFinish();
  Host_UpdateProgressDialog("", -1, -1);
}

void VertexShaderCache::Reload()
{
  s_compiler->WaitForFinish();
  g_vs_disk_cache.Sync();
  g_vs_disk_cache.Close();
  g_vus_disk_cache.Sync();
  g_vus_disk_cache.Close();
  LoadFromDisk();
  if (g_ActiveConfig.CanPrecompileUberShaders())
  {
    CompileUberShaders();
  }
  if (g_ActiveConfig.bCompileShaderOnStartup && !g_ActiveConfig.bDisableSpecializedShaders)
  {
    CompileShaders();
  }
  s_last_entry = nullptr;
  s_last_uid.ClearUID();
  s_last_uber_entry = nullptr;
  s_last_uber_uid.ClearUID();
}

void VertexShaderCache::Clear()
{
  if (s_vshaders)
  {
    s_vshaders->Persist([](VertexShaderUid &uid) {
      uid.ClearHASH();
      uid.CalculateUIDHash();
    });
    s_vshaders->Clear([](VSCacheEntry& item)
    {
      item.Destroy();
    });
  }
  for (auto& item : s_vuber_shaders)
    item.second.Destroy();
  s_vuber_shaders.clear();
  s_last_entry = nullptr;
  s_last_uid.ClearUID();
  s_last_uber_entry = nullptr;
  s_last_uber_uid.ClearUID();
}

void VertexShaderCache::Shutdown()
{
  if (s_compiler)
  {
    s_compiler->WaitForFinish();
  }
  if (vscbuf != nullptr)
  {
    delete vscbuf;
  }

  s_simple_vertex_shader.reset();
  s_clear_vertex_shader.reset();

  s_simple_layout.reset();
  s_clear_layout.reset();

  Clear();
  delete s_vshaders;
  s_vshaders = nullptr;

  g_vs_disk_cache.Sync();
  g_vs_disk_cache.Close();
  g_vus_disk_cache.Sync();
  g_vus_disk_cache.Close();
}

void VertexShaderCache::CompileUberShader(const UberShader::VertexUberShaderUid& uid, const ShaderHostConfig& hostconfig, std::function<void()> oncompilationfinished = {})
{
  VSCacheEntry* entry = &s_vuber_shaders[uid];
  s_last_uber_entry = entry;
  // Compile only when we have a new instance
  if (entry->initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }
  // Need to compile a new shader

  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [uid, hostconfig](ShaderCompilerWorkUnit* wunit)
  {
    UberShader::GenVertexShader(wunit->code, API_D3D11, hostconfig, uid.GetUidData());
  };

  wunit->entrypoint = "main";
  if (g_ActiveConfig.bEnableShaderDebug)
  {
    wunit->flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
  }
  else
  {
    wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  }
  wunit->target = D3D::VertexShaderVersionString();
  wunit->ResultHandler = [uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      ID3DBlob* shaderBuffer = wunit->shaderbytecode;
      const u8* bytecode = (const u8*)shaderBuffer->GetBufferPointer();
      u32 bytecodelen = (u32)shaderBuffer->GetBufferSize();
      g_vus_disk_cache.Append(uid, bytecode, bytecodelen);
      if (g_ActiveConfig.bEnableShaderDebug)
      {
        entry->code = std::string(wunit->code.data());
      }
      PushByteCode(D3DBlob(D3D::UniquePtr<ID3D10Blob>(wunit->shaderbytecode)), entry);
      wunit->shaderbytecode = nullptr;
    }
    else
    {
      static int num_failures = 0;
      std::string filename = StringFromFormat("%sbad_uvs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
      std::ofstream file;
      File::OpenFStream(file, filename, std::ios_base::out);
      file << ((const char *)wunit->code.data());
      file << ((const char *)wunit->error->GetBufferPointer());
      file.close();

      PanicAlert("Failed to compile vertex uber shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
        filename.c_str(),
        D3D::VertexShaderVersionString(),
        (char*)wunit->error->GetBufferPointer());
    }
  };
  s_compiler->CompileShaderAsync(wunit);
}

void VertexShaderCache::CompileVShader(const VertexShaderUid& uid, const ShaderHostConfig& hostconfig, bool forcecompile = false, std::function<void()> oncompilationfinished = {})
{
  VSCacheEntry* entry = &s_vshaders->GetOrAdd(uid);
  if (g_ActiveConfig.bDisableSpecializedShaders && !forcecompile)
  {
    return;
  }
  s_last_entry = entry;
  // Compile only when we have a new instance
  if (entry->initialized.test_and_set())
  {
    if (oncompilationfinished) oncompilationfinished();
    return;
  }

  ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit();
  wunit->GenerateCodeHandler = [uid, hostconfig](ShaderCompilerWorkUnit* wunit)
  {
    GenerateVertexShaderCode(wunit->code, uid.GetUidData(), hostconfig);
  };

  wunit->entrypoint = "main";
  if (g_ActiveConfig.bEnableShaderDebug)
  {
    wunit->flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
  }
  else
  {
    wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
  }
  wunit->target = D3D::VertexShaderVersionString();
  wunit->ResultHandler = [uid, entry, oncompilationfinished](ShaderCompilerWorkUnit* wunit)
  {
    if (oncompilationfinished) oncompilationfinished();
    if (SUCCEEDED(wunit->cresult))
    {
      g_vs_disk_cache.Append(uid, (const u8*)wunit->shaderbytecode->GetBufferPointer(), (u32)wunit->shaderbytecode->GetBufferSize());
      PushByteCode(D3DBlob(D3D::UniquePtr<ID3D10Blob>(wunit->shaderbytecode)), entry);
      wunit->shaderbytecode = nullptr;
      if (g_ActiveConfig.bEnableShaderDebug)
      {
        entry->code = std::string(wunit->code.data());
      }
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

void VertexShaderCache::PrepareShader(
  u32 components,
  const XFMemory
  &xfr,
  const BPMemory &bpm)
{
  if (g_ActiveConfig.bBackgroundShaderCompiling || g_ActiveConfig.bDisableSpecializedShaders)
  {
    auto uuid = UberShader::GetVertexUberShaderUid(components, xfr);
    if (!s_last_uber_entry || s_last_uber_uid != uuid)
    {
      s_last_uber_uid = uuid;
      CompileUberShader(uuid, ShaderHostConfig::GetCurrent());
    }
  }
  VertexShaderUid uid;
  GetVertexShaderUID(uid, components, xfr, bpm);
  s_compiler->ProcCompilationResults();
  if (s_last_entry && uid == s_last_uid)
  {
    return;
  }
  s_last_uid = uid;
  GFX_DEBUGGER_PAUSE_AT(NEXT_VERTEX_SHADER_CHANGE, true);
  CompileVShader(uid, ShaderHostConfig::GetCurrent());
}

void VertexShaderCache::SetActiveShader(D3DVertexFormat* current_vertex_format)
{
  ID3D11VertexShader* shader = nullptr;
  if (!g_ActiveConfig.bDisableSpecializedShaders && s_last_entry && s_last_entry->compiled)
  {
    current_vertex_format->SetInputLayout(s_last_entry->bytecode);
    shader = s_last_entry->shader.get();
  }
  else if (s_last_uber_entry)
  {
    D3DVertexFormat* uber_vertex_format = static_cast<D3DVertexFormat*>(
      VertexLoaderManager::GetUberVertexFormat(current_vertex_format->GetVertexDeclaration()));
    uber_vertex_format->SetInputLayout(s_last_uber_entry->bytecode);
    shader = s_last_uber_entry->shader.get();
  }
  D3D::stateman->SetVertexShader(shader);
}

bool VertexShaderCache::TestShader()
{
  if (g_ActiveConfig.bBackgroundShaderCompiling || g_ActiveConfig.bDisableSpecializedShaders)
  {
    return s_last_uber_entry && s_last_uber_entry->compiled;
  }
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
  return s_last_entry->shader != nullptr && s_last_entry->compiled;
}


void VertexShaderCache::PushByteCode(D3DBlob&& bcodeblob, VSCacheEntry* entry)
{
  entry->shader = std::move(D3D::CreateVertexShaderFromByteCode(bcodeblob));
  entry->compiled = true;
  entry->SetByteCode(std::move(bcodeblob));
  if (entry->shader)
  {
    // TODO: Somehow make the debug name a bit more specific
    D3D::SetDebugObjectName(entry->shader.get(), "a vertex shader of VertexShaderCache");
    INCSTAT(stats.numVertexShadersCreated);
    SETSTAT(stats.numVertexShadersAlive, static_cast<int>(s_vshaders->size()));
  }
}

void VertexShaderCache::InsertByteCode(const VertexShaderUid &uid, D3DBlob&& bcodeblob)
{
  VSCacheEntry* entry = &s_vshaders->GetOrAdd(uid);
  entry->initialized.test_and_set();
  PushByteCode(std::move(bcodeblob), entry);
}
void VertexShaderCache::InsertByteCode(const UberShader::VertexUberShaderUid &uid, D3DBlob&& bcodeblob)
{
  VSCacheEntry* entry = &s_vuber_shaders[uid];
  entry->initialized.test_and_set();
  PushByteCode(std::move(bcodeblob), entry);
}
}  // namespace DX11
