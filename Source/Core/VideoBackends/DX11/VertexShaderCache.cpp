// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/HLSLCompiler.h"
#include "VideoCommon/VertexShaderManager.h"
#include "D3DShader.h"
#include "Globals.h"
#include "VertexShaderCache.h"

#include "Core/ConfigManager.h"

namespace DX11 {

VertexShaderCache::VSCache VertexShaderCache::vshaders;
const VertexShaderCache::VSCacheEntry *VertexShaderCache::last_entry;
VertexShaderUid VertexShaderCache::last_uid;
VertexShaderUid VertexShaderCache::external_last_uid;
UidChecker<VertexShaderUid,ShaderCode> VertexShaderCache::vertex_uid_checker;
static HLSLAsyncCompiler *Compiler;
static Common::SpinLock<true> vshadersLock;

static D3D::VertexShaderPtr SimpleVertexShader;
static D3D::VertexShaderPtr ClearVertexShader;
static D3D::InputLayoutPtr SimpleLayout;
static D3D::InputLayoutPtr ClearLayout;

LinearDiskCache<VertexShaderUid, u8> g_vs_disk_cache;

ID3D11VertexShader* VertexShaderCache::GetSimpleVertexShader() { return SimpleVertexShader.get(); }
ID3D11VertexShader* VertexShaderCache::GetClearVertexShader() { return ClearVertexShader.get(); }
ID3D11InputLayout* VertexShaderCache::GetSimpleInputLayout() { return SimpleLayout.get(); }
ID3D11InputLayout* VertexShaderCache::GetClearInputLayout() { return ClearLayout.get(); }

ID3D11Buffer* vscbuf = nullptr;

ID3D11Buffer* &VertexShaderCache::GetConstantBuffer()
{
	// TODO: divide the global variables of the generated shaders into about 5 constant buffers to speed this up
	if (VertexShaderManager::IsDirty())
	{
		D3D11_MAPPED_SUBRESOURCE map;
		D3D::context->Map(vscbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		const size_t size = sizeof(float) * VertexShaderManager::ConstantBufferSize;
		memcpy(map.pData, VertexShaderManager::GetBuffer(), size);
		D3D::context->Unmap(vscbuf, 0);
		VertexShaderManager::Clear();
		ADDSTAT(stats.thisFrame.bytesUniformStreamed, size);
	}
	return vscbuf;
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

const char simple_shader_code[] = {
	"struct VSOUTPUT\n"
	"{\n"
	"float4 vPosition : SV_Position;\n"
	"float2 vTexCoord : TEXCOORD0;\n"
	"float  vTexCoord1 : TEXCOORD1;\n"
	"};\n"
	"VSOUTPUT main(float4 inPosition : POSITION,float3 inTEX0 : TEXCOORD0)\n"
	"{\n"
	"VSOUTPUT OUT;\n"
	"OUT.vPosition = inPosition;\n"
	"OUT.vTexCoord = inTEX0.xy;\n"
	"OUT.vTexCoord1 = inTEX0.z;\n"
	"return OUT;\n"
	"}\n"
};

const char clear_shader_code[] = {
	"struct VSOUTPUT\n"
	"{\n"
	"float4 vPosition   : SV_Position;\n"
	"float4 vColor0   : COLOR0;\n"						   
	"};\n"
	"VSOUTPUT main(float4 inPosition : POSITION,float4 inColor0: COLOR0)\n"
	"{\n"
	"VSOUTPUT OUT;\n"
	"OUT.vPosition = inPosition;\n"
	"OUT.vColor0 = inColor0;\n"
	"return OUT;\n"
	"}\n"
};

void VertexShaderCache::Init()
{
	vshadersLock.unlock();
	Compiler = &HLSLAsyncCompiler::getInstance();
	const D3D11_INPUT_ELEMENT_DESC simpleelems[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
	};
	const D3D11_INPUT_ELEMENT_DESC clearelems[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	unsigned int cbsize = VertexShaderManager::ConstantBufferSize * sizeof(float); // is always multiple of 16
	D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(cbsize, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	HRESULT hr = D3D::device->CreateBuffer(&cbdesc, NULL, &vscbuf);
	CHECK(hr==S_OK, "Create vertex shader constant buffer (size=%u)", cbsize);
	D3D::SetDebugObjectName((ID3D11DeviceChild*)vscbuf, "vertex shader constant buffer used to emulate the GX pipeline");

	D3DBlob blob;
	D3D::CompileShader(D3D::ShaderType::Vertex, simple_shader_code, blob);
	D3D::device->CreateInputLayout(simpleelems, 2, blob.Data(), blob.Size(), D3D::ToAddr(SimpleLayout));
	SimpleVertexShader = D3D::CreateVertexShaderFromByteCode(blob);
	if (SimpleLayout == NULL || SimpleVertexShader == NULL) PanicAlert("Failed to create simple vertex shader or input layout at %s %d\n", __FILE__, __LINE__);	
	D3D::SetDebugObjectName((ID3D11DeviceChild*)SimpleVertexShader.get(), "simple vertex shader");
	D3D::SetDebugObjectName((ID3D11DeviceChild*)SimpleLayout.get(), "simple input layout");

	D3D::CompileShader(D3D::ShaderType::Vertex, clear_shader_code, blob);
	D3D::device->CreateInputLayout(clearelems, 2, blob.Data(), blob.Size(), D3D::ToAddr(ClearLayout));
	ClearVertexShader = D3D::CreateVertexShaderFromByteCode(blob);
	if (ClearLayout == NULL || ClearVertexShader == NULL) PanicAlert("Failed to create clear vertex shader or input layout at %s %d\n", __FILE__, __LINE__);
	D3D::SetDebugObjectName((ID3D11DeviceChild*)ClearVertexShader.get(), "clear vertex shader");
	D3D::SetDebugObjectName((ID3D11DeviceChild*)ClearLayout.get(), "clear input layout");

	Clear();

	if (!File::Exists(File::GetUserPath(D_SHADERCACHE_IDX)))
		File::CreateDir(File::GetUserPath(D_SHADERCACHE_IDX).c_str());

	SETSTAT(stats.numVertexShadersCreated, 0);
	SETSTAT(stats.numVertexShadersAlive, 0);

	char cache_filename[MAX_PATH];
	sprintf(cache_filename, "%sdx11-%s-vs.cache", File::GetUserPath(D_SHADERCACHE_IDX).c_str(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strUniqueID.c_str());
	VertexShaderCacheInserter inserter;
	vshadersLock.lock();
	g_vs_disk_cache.OpenAndRead(cache_filename, inserter);
	vshadersLock.unlock();

	if (g_Config.bEnableShaderDebugging)
		Clear();

	last_entry = NULL;
	VertexShaderManager::DisableDirtyRegions();
}

void VertexShaderCache::Clear()
{
	vshadersLock.lock();
	for (VSCache::iterator iter = vshaders.begin(); iter != vshaders.end(); ++iter)
		iter->second.Destroy();
	vshaders.clear();
	vshadersLock.unlock();
	vertex_uid_checker.Invalidate();

	last_entry = NULL;
}

void VertexShaderCache::Shutdown()
{
	Compiler->WaitForFinish();
	SAFE_RELEASE(vscbuf);

	SimpleVertexShader.reset();
	ClearVertexShader.reset();

	SimpleLayout.reset();
	ClearLayout.reset();

	Clear();
	g_vs_disk_cache.Sync();
	g_vs_disk_cache.Close();
}

void VertexShaderCache::PrepareShader(
	u32 components, 
	const XFRegisters 
	&xfr, 
	const BPMemory &bpm, 
	bool ongputhread)
{
	VertexShaderUid uid;
	GetVertexShaderUidD3D11(uid, components, xfr, bpm);
	if (ongputhread)
	{
		Compiler->ProcCompilationResults();
	#if defined(_DEBUG) || defined(DEBUGFAST)
		if (g_ActiveConfig.bEnableShaderDebugging)
		{
			ShaderCode code;
			GenerateVertexShaderCodeD3D11(code, components, xfr, bpm);
			vertex_uid_checker.AddToIndexAndCheck(code, uid, "Vertex", "v");
		}
	#endif
		if (last_entry)
		{
			if (uid == last_uid)
			{
				return;
			}
		}
		last_uid = uid;
		GFX_DEBUGGER_PAUSE_AT(NEXT_VERTEX_SHADER_CHANGE, true);
	}
	else
	{
		if (external_last_uid == uid)
		{
			return;
		}
		external_last_uid = uid;
	}
	vshadersLock.lock();
	VSCacheEntry* entry = &vshaders[uid];
	vshadersLock.unlock();
	if (ongputhread)
	{
		last_entry = entry;
	}
	// Compile only when we have a new instance
	if (entry->initialized.test_and_set())
	{
		return;
	}
	ShaderCode code;
	ShaderCompilerWorkUnit *wunit = Compiler->NewUnit(VERTEXSHADERGEN_BUFFERSIZE);
	code.SetBuffer(wunit->code.data());
	GenerateVertexShaderCodeD3D11(code, components, xfr, bpm);
	wunit->codesize = (u32)code.BufferSize();
	wunit->entrypoint = "main";
	wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3 | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY;
	wunit->target = D3D::VertexShaderVersionString();
	wunit->ResultHandler = [uid, entry](ShaderCompilerWorkUnit* wunit)
	{
		if (SUCCEEDED(wunit->cresult))
		{
			g_vs_disk_cache.Append(uid, (const u8*)wunit->shaderbytecode->GetBufferPointer(), (u32)wunit->shaderbytecode->GetBufferSize());
			PushByteCode(uid, D3DBlob(D3D::UniquePtr<ID3D10Blob>(wunit->shaderbytecode)), entry);
			wunit->shaderbytecode = nullptr;
#if defined(_DEBUG) || defined(DEBUGFAST)
			if (g_ActiveConfig.bEnableShaderDebugging)
			{
				entry->code = wunit->code.data();
			}
#endif
		}
		else
		{
			static int num_failures = 0;
			char szTemp[MAX_PATH];
			sprintf(szTemp, "%sbad_vs_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
			std::ofstream file;
			OpenFStream(file, szTemp, std::ios_base::out);
			file << ((const char*)wunit->code.data());
			file.close();

			PanicAlert("Failed to compile vertex shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
				szTemp,
				D3D::VertexShaderVersionString(),
				(char*)wunit->error->GetBufferPointer());
		}
	};
	Compiler->CompileShaderAsync(wunit);
}

bool VertexShaderCache::TestShader()
{
	int count = 0;
	while (!last_entry->compiled)
	{
		Compiler->ProcCompilationResults();
		if (g_ActiveConfig.bFullAsyncShaderCompilation)
		{
			break;
		}
		Common::cYield(count++);
	}
	return last_entry->shader != nullptr && last_entry->compiled;
}


void VertexShaderCache::PushByteCode(const VertexShaderUid &uid, D3DBlob&& bcodeblob, VSCacheEntry* entry)
{
	entry->shader = std::move(D3D::CreateVertexShaderFromByteCode(bcodeblob));
	entry->compiled = true;
	entry->SetByteCode(std::move(bcodeblob));
	if (entry->shader)
	{
		// TODO: Somehow make the debug name a bit more specific
		D3D::SetDebugObjectName((ID3D11DeviceChild*)entry->shader.get(), "a vertex shader of VertexShaderCache");
		INCSTAT(stats.numVertexShadersCreated);
		SETSTAT(stats.numVertexShadersAlive, (int)vshaders.size());
	}
}

void VertexShaderCache::InsertByteCode(const VertexShaderUid &uid, D3DBlob&& bcodeblob)
{
	VSCacheEntry* entry = &vshaders[uid];
	entry->initialized.test_and_set();
	PushByteCode(uid, std::move(bcodeblob), entry);
}
}  // namespace DX11
