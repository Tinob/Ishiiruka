// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/HLSLCompiler.h"
#include "D3DShader.h"
#include "Globals.h"
#include "VertexShaderCache.h"

#include "Core/ConfigManager.h"

// See comment near the bottom of this file
GC_ALIGNED16(float vsconstants[C_VENVCONST_END*4]);
bool vscbufchanged = true;

namespace DX11 {

VertexShaderCache::VSCache VertexShaderCache::vshaders;
const VertexShaderCache::VSCacheEntry *VertexShaderCache::last_entry;
VertexShaderUid VertexShaderCache::last_uid;
VertexShaderUid VertexShaderCache::external_last_uid;
UidChecker<VertexShaderUid,ShaderCode> VertexShaderCache::vertex_uid_checker;
static HLSLAsyncCompiler *Compiler;
static Common::SpinLock<true> vshadersLock;

static ID3D11VertexShader* SimpleVertexShader = NULL;
static ID3D11VertexShader* ClearVertexShader = NULL;
static ID3D11InputLayout* SimpleLayout = NULL;
static ID3D11InputLayout* ClearLayout = NULL;

LinearDiskCache<VertexShaderUid, u8> g_vs_disk_cache;

ID3D11VertexShader* VertexShaderCache::GetSimpleVertexShader() { return SimpleVertexShader; }
ID3D11VertexShader* VertexShaderCache::GetClearVertexShader() { return ClearVertexShader; }
ID3D11InputLayout* VertexShaderCache::GetSimpleInputLayout() { return SimpleLayout; }
ID3D11InputLayout* VertexShaderCache::GetClearInputLayout() { return ClearLayout; }

ID3D11Buffer* vscbuf = NULL;

ID3D11Buffer* &VertexShaderCache::GetConstantBuffer()
{
	// TODO: divide the global variables of the generated shaders into about 5 constant buffers to speed this up
	if (vscbufchanged)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		D3D::context->Map(vscbuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, vsconstants, sizeof(vsconstants));
		D3D::context->Unmap(vscbuf, 0);
		vscbufchanged = false;
		
		ADDSTAT(stats.thisFrame.bytesUniformStreamed, sizeof(vsconstants));
	}
	return vscbuf;
}

// this class will load the precompiled shaders into our cache
class VertexShaderCacheInserter : public LinearDiskCacheReader<VertexShaderUid, u8>
{
public:
	void Read(const VertexShaderUid &key, const u8 *value, u32 value_size)
	{
		D3DBlob* blob = new D3DBlob(value_size, value);
		VertexShaderCache::InsertByteCode(key, blob);
		blob->Release();

	}
};

const char simple_shader_code[] = {
	"struct VSOUTPUT\n"
	"{\n"
	"float4 vPosition : POSITION;\n"
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
	"float4 vPosition   : POSITION;\n"
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

	unsigned int cbsize = ((sizeof(vsconstants))&(~0xf))+0x10; // must be a multiple of 16
	D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(cbsize, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	HRESULT hr = D3D::device->CreateBuffer(&cbdesc, NULL, &vscbuf);
	CHECK(hr==S_OK, "Create vertex shader constant buffer (size=%u)", cbsize);
	D3D::SetDebugObjectName((ID3D11DeviceChild*)vscbuf, "vertex shader constant buffer used to emulate the GX pipeline");

	D3DBlob* blob;
	D3D::CompileVertexShader(simple_shader_code, &blob);
	D3D::device->CreateInputLayout(simpleelems, 2, blob->Data(), blob->Size(), &SimpleLayout);
	SimpleVertexShader = D3D::CreateVertexShaderFromByteCode(blob);
	if (SimpleLayout == NULL || SimpleVertexShader == NULL) PanicAlert("Failed to create simple vertex shader or input layout at %s %d\n", __FILE__, __LINE__);
	blob->Release();
	D3D::SetDebugObjectName((ID3D11DeviceChild*)SimpleVertexShader, "simple vertex shader");
	D3D::SetDebugObjectName((ID3D11DeviceChild*)SimpleLayout, "simple input layout");

	D3D::CompileVertexShader(clear_shader_code, &blob);
	D3D::device->CreateInputLayout(clearelems, 2, blob->Data(), blob->Size(), &ClearLayout);
	ClearVertexShader = D3D::CreateVertexShaderFromByteCode(blob);
	if (ClearLayout == NULL || ClearVertexShader == NULL) PanicAlert("Failed to create clear vertex shader or input layout at %s %d\n", __FILE__, __LINE__);
	blob->Release();
	D3D::SetDebugObjectName((ID3D11DeviceChild*)ClearVertexShader, "clear vertex shader");
	D3D::SetDebugObjectName((ID3D11DeviceChild*)ClearLayout, "clear input layout");

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

	SAFE_RELEASE(SimpleVertexShader);
	SAFE_RELEASE(ClearVertexShader);

	SAFE_RELEASE(SimpleLayout);
	SAFE_RELEASE(ClearLayout);

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
			D3DBlob* pbytecode = new D3DBlob(wunit->shaderbytecode);
			g_vs_disk_cache.Append(uid, pbytecode->Data(), pbytecode->Size());
			PushByteCode(uid, pbytecode, entry);
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


void VertexShaderCache::PushByteCode(const VertexShaderUid &uid, D3DBlob* bcodeblob, VertexShaderCache::VSCacheEntry* entry)
{
	entry->shader = D3D::CreateVertexShaderFromByteCode(bcodeblob);
	entry->compiled = true;
	entry->SetByteCode(bcodeblob);
	if (entry->shader)
	{
		// TODO: Somehow make the debug name a bit more specific
		D3D::SetDebugObjectName((ID3D11DeviceChild*)entry->shader, "a vertex shader of VertexShaderCache");
		INCSTAT(stats.numVertexShadersCreated);
		SETSTAT(stats.numVertexShadersAlive, (int)vshaders.size());
	}
}

void VertexShaderCache::InsertByteCode(const VertexShaderUid &uid, D3DBlob* bcodeblob)
{
	VSCacheEntry* entry = &vshaders[uid];
	entry->initialized.test_and_set();
	PushByteCode(uid, bcodeblob, entry);
}
// These are "callbacks" from VideoCommon and thus must be outside namespace DX11.
// This will have to be changed when we merge.

// maps the constant numbers to float indices in the constant buffer
void Renderer::SetVSConstant4f(unsigned int const_number, float f1, float f2, float f3, float f4)
{
	u32 idx = const_number * 4;
	vsconstants[idx++] = f1;
	vsconstants[idx++] = f2;
	vsconstants[idx++] = f3;
	vsconstants[idx] = f4;
	vscbufchanged = true;
}

void Renderer::SetVSConstant4fv(unsigned int const_number, const float* f)
{
	u32 idx = const_number * 4;
	memcpy(&vsconstants[idx], f, sizeof(float) * 4);
	vscbufchanged = true;
}

void Renderer::SetMultiVSConstant3fv(unsigned int const_number, unsigned int count, const float* f)
{
	u32 idx = const_number * 4;
	for (unsigned int i = 0; i < count; i++)
	{
		vsconstants[idx++] = *f++;
		vsconstants[idx++] = *f++;
		vsconstants[idx++] = *f++;
		vsconstants[idx++] = 0.f;		
	}
	vscbufchanged = true;
}

void Renderer::SetMultiVSConstant4fv(unsigned int const_number, unsigned int count, const float* f)
{
	u32 idx = const_number * 4;
	memcpy(&vsconstants[idx], f, sizeof(float) * 4 * count);
	vscbufchanged = true;
}

}  // namespace DX11
