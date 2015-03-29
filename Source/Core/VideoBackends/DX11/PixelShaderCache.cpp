// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"

#include "Core/ConfigManager.h"

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/HLSLCompiler.h"
#include "VideoCommon/PixelShaderManager.h"

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DShader.h"
#include "VideoBackends/DX11/Globals.h"
#include "VideoBackends/DX11/PixelShaderCache.h"
#include "VideoBackends/DX11/VertexShaderCache.h"

static bool s_previous_per_pixel_lighting = false;

namespace DX11
{

PixelShaderCache::PSCache PixelShaderCache::s_pixel_shaders;
const PixelShaderCache::PSCacheEntry* PixelShaderCache::s_last_entry;
PixelShaderUid PixelShaderCache::s_last_uid;
PixelShaderUid PixelShaderCache::s_external_last_uid;
UidChecker<PixelShaderUid,ShaderCode> PixelShaderCache::s_pixel_uid_checker;
static HLSLAsyncCompiler *s_compiler;
static Common::SpinLock<true> s_pixel_shaders_lock;

LinearDiskCache<PixelShaderUid, u8> g_ps_disk_cache;

D3D::PixelShaderPtr s_ColorMatrixProgram[2];
D3D::PixelShaderPtr s_ColorCopyProgram[2];
D3D::PixelShaderPtr s_DepthMatrixProgram[2];
D3D::PixelShaderPtr s_ClearProgram;
D3D::PixelShaderPtr s_rgba6_to_rgb8[2];
D3D::PixelShaderPtr s_rgb8_to_rgba6[2];
ID3D11Buffer* pscbuf;
ID3D11Buffer* pscbuf_alt;

const char clear_program_code[] = {
	"void main(\n"
	"out float4 ocol0 : SV_Target,\n"
	"in float4 pos : SV_Position,\n"
	"in float4 incol0 : COLOR0){\n"
	"ocol0 = incol0;\n"
	"}\n"
};

// TODO: Find some way to avoid having separate shaders for non-MSAA and MSAA...
const char color_copy_program_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2D Tex0 : register(t0);\n"
	"void main(\n"
	"out float4 ocol0 : SV_Target,\n"
	"in float4 pos : SV_Position,\n"
	"in float2 uv0 : TEXCOORD0){\n"
	"ocol0 = Tex0.Sample(samp0,uv0);\n"
	"}\n"
};

// TODO: Improve sampling algorithm!
const char color_copy_program_code_msaa[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2DMS<float4, %d> Tex0 : register(t0);\n"
	"void main(\n"
	"out float4 ocol0 : SV_Target,\n"
	"in float4 pos : SV_Position,\n"
	"in float2 uv0 : TEXCOORD0){\n"
	"int width, height, samples;\n"
	"Tex0.GetDimensions(width, height, samples);\n"
	"ocol0 = 0;\n"
	"for(int i = 0; i < samples; ++i)\n"
	"	ocol0 += Tex0.Load(int2(uv0.x*(width), uv0.y*(height)), i);\n"
	"ocol0 /= samples;\n"
	"}\n"
};

const char color_matrix_program_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2D Tex0 : register(t0);\n"
	"uniform float4 cColMatrix[7] : register(c0);\n"
	"void main(\n" 
	"out float4 ocol0 : SV_Target,\n"
	"in float4 pos : SV_Position,\n"
	" in float2 uv0 : TEXCOORD0){\n"
	"float4 texcol = Tex0.Sample(samp0,uv0);\n"
	"texcol = round(texcol * cColMatrix[5])*cColMatrix[6];\n"
	"ocol0 = float4(dot(texcol,cColMatrix[0]),dot(texcol,cColMatrix[1]),dot(texcol,cColMatrix[2]),dot(texcol,cColMatrix[3])) + cColMatrix[4];\n"
	"}\n"
};

const char color_matrix_program_code_msaa[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2DMS<float4, %d> Tex0 : register(t0);\n"
	"uniform float4 cColMatrix[7] : register(c0);\n"
	"void main(\n" 
	"out float4 ocol0 : SV_Target,\n"
	"in float4 pos : SV_Position,\n"
	" in float2 uv0 : TEXCOORD0){\n"
	"int width, height, samples;\n"
	"Tex0.GetDimensions(width, height, samples);\n"
	"float4 texcol = 0;\n"
	"for(int i = 0; i < samples; ++i)\n"
	"	texcol += Tex0.Load(int2(uv0.x*(width), uv0.y*(height)), i);\n"
	"texcol /= samples;\n"
	"texcol = round(texcol * cColMatrix[5])*cColMatrix[6];\n"
	"ocol0 = float4(dot(texcol,cColMatrix[0]),dot(texcol,cColMatrix[1]),dot(texcol,cColMatrix[2]),dot(texcol,cColMatrix[3])) + cColMatrix[4];\n"
	"}\n"
};

const char depth_matrix_program_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2D Tex0 : register(t0);\n"
	"uniform float4 cColMatrix[7] : register(c0);\n"
	"void main(\n"
	"out float4 ocol0 : SV_Target,\n"
	" in float4 pos : SV_Position,\n"
	" in float2 uv0 : TEXCOORD0){\n"
	"	float4 texcol = Tex0.Sample(samp0,uv0);\n"

	// 255.99998474121 = 16777215/16777216*256
	"	int workspace = int(round((1.0 - texcol.x) * 16777215.0f));\n"
	"	texcol.z = float(workspace & 255);\n"   // z component
	"	workspace = workspace >> 8;\n"
	"	texcol.y = float(workspace & 255);\n"   // y component
	"	workspace = workspace >> 8;\n"
	"	texcol.x = float(workspace & 255);\n"   // x component
	"	texcol.w = float(workspace & 240);\n"    // w component
	"	texcol = texcol / 255.0;\n"             // normalize components to [0.0..1.0]
	"	ocol0 = float4(dot(texcol,cColMatrix[0]),dot(texcol,cColMatrix[1]),dot(texcol,cColMatrix[2]),dot(texcol,cColMatrix[3])) + cColMatrix[4];\n"
	"}\n"
};

const char depth_matrix_program_msaa[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2DMS<float4, %d> Tex0 : register(t0);\n"
	"uniform float4 cColMatrix[7] : register(c0);\n"
	"void main(\n"
	"out float4 ocol0 : SV_Target,\n"
	" in float4 pos : SV_Position,\n"
	" in float2 uv0 : TEXCOORD0){\n"
	"	int width, height, samples;\n"
	"	Tex0.GetDimensions(width, height, samples);\n"
	"	float4 texcol = 0;\n"
	"	for(int i = 0; i < samples; ++i)\n"
	"		texcol += Tex0.Load(int2(uv0.x*(width), uv0.y*(height)), i);\n"
	"	texcol /= samples;\n"

	"	int workspace = int(round((1.0 - texcol.x) * 16777215.0f));\n"
	"	texcol.z = float(workspace & 255);\n"   // z component
	"	workspace = workspace >> 8;\n"
	"	texcol.y = float(workspace & 255);\n"   // y component
	"	workspace = workspace >> 8;\n"
	"	texcol.x = float(workspace & 255);\n"   // y component
	"	texcol.w = float(workspace & 240);\n"    // w component
	"	texcol = texcol / 255.0;\n"             // normalize components to [0.0..1.0]
	"	ocol0 = float4(dot(texcol,cColMatrix[0]),dot(texcol,cColMatrix[1]),dot(texcol,cColMatrix[2]),dot(texcol,cColMatrix[3])) + cColMatrix[4];\n"
	"}\n"
};

const char reint_rgba6_to_rgb8_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2D Tex0 : register(t0);\n"
	"void main(\n"
	"	out float4 ocol0 : SV_Target,\n"
	"	in float4 pos : SV_Position,\n"
	"	in float2 uv0 : TEXCOORD0)\n"
	"{\n"
	"	int4 src6 = round(Tex0.Sample(samp0,uv0) * 63.f);\n"
	"	int4 dst8;\n"
	"	dst8.r = (src6.r << 2) | (src6.g >> 4);\n"
	"	dst8.g = ((src6.g & 0xF) << 4) | (src6.b >> 2);\n"
	"	dst8.b = ((src6.b & 0x3) << 6) | src6.a;\n"
	"	dst8.a = 255;\n"
	"	ocol0 = (float4)dst8 / 255.f;\n"
	"}"
};

const char reint_rgba6_to_rgb8_msaa[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2DMS<float4, %d> Tex0 : register(t0);\n"
	"void main(\n"
	"	out float4 ocol0 : SV_Target,\n"
	"	in float4 pos : SV_Position,\n"
	"	in float2 uv0 : TEXCOORD0)\n"
	"{\n"
	"	int width, height, samples;\n"
	"	Tex0.GetDimensions(width, height, samples);\n"
	"	float4 texcol = 0;\n"
	"	for(int i = 0; i < samples; ++i)\n"
	"		texcol += Tex0.Load(int2(uv0.x*(width), uv0.y*(height)), i);\n"
	"	texcol /= samples;\n"
	"	int4 src6 = round(texcol * 63.f);\n"
	"	int4 dst8;\n"
	"	dst8.r = (src6.r << 2) | (src6.g >> 4);\n"
	"	dst8.g = ((src6.g & 0xF) << 4) | (src6.b >> 2);\n"
	"	dst8.b = ((src6.b & 0x3) << 6) | src6.a;\n"
	"	dst8.a = 255;\n"
	"	ocol0 = (float4)dst8 / 255.f;\n"
	"}"
};

const char reint_rgb8_to_rgba6_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2D Tex0 : register(t0);\n"
	"void main(\n"
	"	out float4 ocol0 : SV_Target,\n"
	"	in float4 pos : SV_Position,\n"
	"	in float2 uv0 : TEXCOORD0)\n"
	"{\n"
	"	int4 src8 = round(Tex0.Sample(samp0,uv0) * 255.f);\n"
	"	int4 dst6;\n"
	"	dst6.r = src8.r >> 2;\n"
	"	dst6.g = ((src8.r & 0x3) << 4) | (src8.g >> 4);\n"
	"	dst6.b = ((src8.g & 0xF) << 2) | (src8.b >> 6);\n"
	"	dst6.a = src8.b & 0x3F;\n"
	"	ocol0 = (float4)dst6 / 63.f;\n"
	"}\n"
};

const char reint_rgb8_to_rgba6_msaa_code[] = {
	"sampler samp0 : register(s0);\n"
	"Texture2DMS<float4, %d> Tex0 : register(t0);\n"
	"void main(\n"
	"	out float4 ocol0 : SV_Target,\n"
	"	in float4 pos : SV_Position,\n"
	"	in float2 uv0 : TEXCOORD0)\n"
	"{\n"
	"	int width, height, samples;\n"
	"	Tex0.GetDimensions(width, height, samples);\n"
	"	float4 texcol = 0;\n"
	"	for(int i = 0; i < samples; ++i)\n"
	"		texcol += Tex0.Load(int2(uv0.x*(width), uv0.y*(height)), i);\n"
	"	texcol /= samples;\n"
	"	int4 src8 = round(texcol * 255.f);\n"
	"	int4 dst6;\n"
	"	dst6.r = src8.r >> 2;\n"
	"	dst6.g = ((src8.r & 0x3) << 4) | (src8.g >> 4);\n"
	"	dst6.b = ((src8.g & 0xF) << 2) | (src8.b >> 6);\n"
	"	dst6.a = src8.b & 0x3F;\n"
	"	ocol0 = (float4)dst6 / 63.f;\n"
	"}\n"
};

ID3D11PixelShader* PixelShaderCache::ReinterpRGBA6ToRGB8(bool multisampled)
{
	if (!multisampled || D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count == 1)
	{
		if (!s_rgba6_to_rgb8[0])
		{
			s_rgba6_to_rgb8[0] = D3D::CompileAndCreatePixelShader(reint_rgba6_to_rgb8_code);
			CHECK(s_rgba6_to_rgb8[0], "Create RGBA6 to RGB8 pixel shader");
			D3D::SetDebugObjectName(s_rgba6_to_rgb8[0].get(), "RGBA6 to RGB8 pixel shader");
		}
		return s_rgba6_to_rgb8[0].get();
	}
	else if (!s_rgba6_to_rgb8[1])
	{
		// create MSAA shader for current AA mode
		char buf[1024];
		const int l = sprintf_s(buf, 1024, reint_rgba6_to_rgb8_msaa, D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count);

		s_rgba6_to_rgb8[1] = D3D::CompileAndCreatePixelShader(buf);

		CHECK(s_rgba6_to_rgb8[1], "Create RGBA6 to RGB8 MSAA pixel shader");
		D3D::SetDebugObjectName(s_rgba6_to_rgb8[1].get(), "RGBA6 to RGB8 MSAA pixel shader");
	}
	return s_rgba6_to_rgb8[1].get();
}

ID3D11PixelShader* PixelShaderCache::ReinterpRGB8ToRGBA6(bool multisampled)
{
	if (!multisampled || D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count == 1)
	{
		if (!s_rgb8_to_rgba6[0])
		{
			s_rgb8_to_rgba6[0] = D3D::CompileAndCreatePixelShader(reint_rgb8_to_rgba6_code);
			CHECK(s_rgb8_to_rgba6[0], "Create RGB8 to RGBA6 pixel shader");
			D3D::SetDebugObjectName(s_rgb8_to_rgba6[0].get(), "RGB8 to RGBA6 pixel shader");
		}
		return s_rgb8_to_rgba6[0].get();
	}
	else if (!s_rgb8_to_rgba6[1])
	{
		// create MSAA shader for current AA mode
		char buf[1024];
		const int l = sprintf_s(buf, 1024, reint_rgb8_to_rgba6_msaa_code, D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count);

		s_rgb8_to_rgba6[1] = D3D::CompileAndCreatePixelShader(buf);

		CHECK(s_rgb8_to_rgba6[1], "Create RGB8 to RGBA6 MSAA pixel shader");
		D3D::SetDebugObjectName(s_rgb8_to_rgba6[1].get(), "RGB8 to RGBA6 MSAA pixel shader");
	}
	return s_rgb8_to_rgba6[1].get();
}

ID3D11PixelShader* PixelShaderCache::GetColorCopyProgram(bool multisampled)
{
	if (!multisampled || D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count == 1) return s_ColorCopyProgram[0].get();
	else if (s_ColorCopyProgram[1]) return s_ColorCopyProgram[1].get();
	else
	{
		// create MSAA shader for current AA mode
		char buf[1024];
		int l = sprintf_s(buf, 1024, color_copy_program_code_msaa, D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count);
		s_ColorCopyProgram[1] = D3D::CompileAndCreatePixelShader(buf);
		CHECK(s_ColorCopyProgram[1]!=nullptr, "Create color copy MSAA pixel shader");
		D3D::SetDebugObjectName(s_ColorCopyProgram[1].get(), "color copy MSAA pixel shader");
		return s_ColorCopyProgram[1].get();
	}
}

ID3D11PixelShader* PixelShaderCache::GetColorMatrixProgram(bool multisampled)
{
	if (!multisampled || D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count == 1) return s_ColorMatrixProgram[0].get();
	else if (s_ColorMatrixProgram[1]) return s_ColorMatrixProgram[1].get();
	else
	{
		// create MSAA shader for current AA mode
		char buf[1024];
		int l = sprintf_s(buf, 1024, color_matrix_program_code_msaa, D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count);
		s_ColorMatrixProgram[1] = D3D::CompileAndCreatePixelShader(buf);
		CHECK(s_ColorMatrixProgram[1]!=nullptr, "Create color matrix MSAA pixel shader");
		D3D::SetDebugObjectName(s_ColorMatrixProgram[1].get(), "color matrix MSAA pixel shader");
		return s_ColorMatrixProgram[1].get();
	}
}

ID3D11PixelShader* PixelShaderCache::GetDepthMatrixProgram(bool multisampled)
{
	if (!multisampled || D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count == 1) return s_DepthMatrixProgram[0].get();
	else if (s_DepthMatrixProgram[1]) return s_DepthMatrixProgram[1].get();
	else
	{
		// create MSAA shader for current AA mode
		char buf[1024];
		int l = sprintf_s(buf, 1024, depth_matrix_program_msaa, D3D::GetAAMode(g_ActiveConfig.iMultisampleMode).Count);
		s_DepthMatrixProgram[1] = D3D::CompileAndCreatePixelShader(buf);
		CHECK(s_DepthMatrixProgram[1]!=nullptr, "Create depth matrix MSAA pixel shader");
		D3D::SetDebugObjectName(s_DepthMatrixProgram[1].get(), "depth matrix MSAA pixel shader");
		return s_DepthMatrixProgram[1].get();
	}
}

ID3D11PixelShader* PixelShaderCache::GetClearProgram()
{
	return s_ClearProgram.get();
}

ID3D11Buffer* &PixelShaderCache::GetConstantBuffer()
{
	bool lightingEnabled = xfmem.numChan.numColorChans > 0;
	bool enable_pl = g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting && lightingEnabled;
	auto &buf = enable_pl ? pscbuf : pscbuf_alt;
	if (PixelShaderManager::IsDirty() || s_previous_per_pixel_lighting != enable_pl)
	{
		int sz = enable_pl ? PixelShaderManager::ConstantBufferSize : C_PLIGHTS * 4;
		sz *= sizeof(float);
		s_previous_per_pixel_lighting = enable_pl;
		// TODO: divide the global variables of the generated shaders into about 5 constant buffers to speed this up
		D3D11_MAPPED_SUBRESOURCE map;
		D3D::context->Map(buf, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, PixelShaderManager::GetBuffer(), sz);
		D3D::context->Unmap(buf, 0);
		PixelShaderManager::Clear();
		ADDSTAT(stats.thisFrame.bytesUniformStreamed, sz);
	}
	return buf;
}

// this class will load the precompiled shaders into our cache
class PixelShaderCacheInserter : public LinearDiskCacheReader<PixelShaderUid, u8>
{
public:
	void Read(const PixelShaderUid &key, const u8 *value, u32 value_size)
	{
		PixelShaderCache::InsertByteCode(key, value, value_size);
	}
};

void PixelShaderCache::Init()
{
	s_compiler = &HLSLAsyncCompiler::getInstance();
	s_pixel_shaders_lock.unlock();
	unsigned int cbsize = PixelShaderManager::ConstantBufferSize * sizeof(float); // is always a multiple of 16	
	D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(cbsize, 
		D3D11_BIND_CONSTANT_BUFFER, 
		D3D11_USAGE_DYNAMIC, 
		D3D11_CPU_ACCESS_WRITE);
	D3D::device->CreateBuffer(&cbdesc, nullptr, &pscbuf);
	CHECK(pscbuf!=nullptr, "Create pixel shader constant buffer");
	D3D::SetDebugObjectName(pscbuf, "pixel shader constant buffer used to emulate the GX pipeline");
	cbsize = C_PLIGHTS * 4 * sizeof(float);
	cbdesc.ByteWidth = cbsize;
	D3D::device->CreateBuffer(&cbdesc, nullptr, &pscbuf_alt);
	CHECK(pscbuf != nullptr, "Create pixel shader constant buffer");
	D3D::SetDebugObjectName(pscbuf_alt, "pixel shader constant buffer used to emulate the GX pipeline");


	// used when drawing clear quads
	s_ClearProgram = D3D::CompileAndCreatePixelShader(clear_program_code);	
	CHECK(s_ClearProgram!=nullptr, "Create clear pixel shader");
	D3D::SetDebugObjectName(s_ClearProgram.get(), "clear pixel shader");

	// used when copying/resolving the color buffer
	s_ColorCopyProgram[0] = D3D::CompileAndCreatePixelShader(color_copy_program_code);
	CHECK(s_ColorCopyProgram[0]!=nullptr, "Create color copy pixel shader");
	D3D::SetDebugObjectName(s_ColorCopyProgram[0].get(), "color copy pixel shader");

	// used for color conversion
	s_ColorMatrixProgram[0] = D3D::CompileAndCreatePixelShader(color_matrix_program_code);
	CHECK(s_ColorMatrixProgram[0]!=nullptr, "Create color matrix pixel shader");
	D3D::SetDebugObjectName(s_ColorMatrixProgram[0].get(), "color matrix pixel shader");

	// used for depth copy
	s_DepthMatrixProgram[0] = D3D::CompileAndCreatePixelShader(depth_matrix_program_code);
	CHECK(s_DepthMatrixProgram[0]!=nullptr, "Create depth matrix pixel shader");
	D3D::SetDebugObjectName(s_DepthMatrixProgram[0].get(), "depth matrix pixel shader");

	Clear();

	if (!File::Exists(File::GetUserPath(D_SHADERCACHE_IDX)))
		File::CreateDir(File::GetUserPath(D_SHADERCACHE_IDX).c_str());

	SETSTAT(stats.numPixelShadersCreated, 0);
	SETSTAT(stats.numPixelShadersAlive, 0);

	char cache_filename[MAX_PATH];
	sprintf(cache_filename, "%sIDX11-%s-ps.cache", File::GetUserPath(D_SHADERCACHE_IDX).c_str(),
			SConfig::GetInstance().m_LocalCoreStartupParameter.m_strUniqueID.c_str());
	PixelShaderCacheInserter inserter;
	s_pixel_shaders_lock.lock();
	g_ps_disk_cache.OpenAndRead(cache_filename, inserter);
	s_pixel_shaders_lock.unlock();

	if (g_Config.bEnableShaderDebugging)
		Clear();

	s_last_entry = nullptr;
	PixelShaderManager::DisableDirtyRegions();
}

// ONLY to be used during shutdown.
void PixelShaderCache::Clear()
{
	s_pixel_shaders_lock.lock();
	for (PSCache::iterator iter = s_pixel_shaders.begin(); iter != s_pixel_shaders.end(); iter++)
		iter->second.Destroy();
	s_pixel_shaders.clear();
	s_pixel_shaders_lock.unlock();
	s_pixel_uid_checker.Invalidate();

	s_last_entry = nullptr;
}

// Used in Swap() when AA mode has changed
void PixelShaderCache::InvalidateMSAAShaders()
{
	s_ColorCopyProgram[1].reset();
	s_ColorMatrixProgram[1].reset();
	s_DepthMatrixProgram[1].reset();
	s_rgb8_to_rgba6[1].reset();
	s_rgba6_to_rgb8[1].reset();
}

void PixelShaderCache::Shutdown()
{
	if (s_compiler)
	{
		s_compiler->WaitForFinish();
	}
	SAFE_RELEASE(pscbuf);
	SAFE_RELEASE(pscbuf_alt);

	s_ClearProgram.reset();
	for (auto & p : s_ColorCopyProgram)
		p.reset();
	for (auto & p : s_ColorMatrixProgram)
		p.reset();
	for (auto & p : s_DepthMatrixProgram)
		p.reset();
	for (auto & p : s_rgba6_to_rgb8)
		p.reset();
	for (auto & p : s_rgb8_to_rgba6)
		p.reset();
	Clear();
	g_ps_disk_cache.Sync();
	g_ps_disk_cache.Close();
}

void PixelShaderCache::PrepareShader(DSTALPHA_MODE dstAlphaMode,
	u32 components,
	const XFMemory &xfr,
	const BPMemory &bpm, bool ongputhread)
{
	PixelShaderUid uid;
	GetPixelShaderUidD3D11(uid, dstAlphaMode, components, xfr, bpm);
	if (ongputhread)
	{
		s_compiler->ProcCompilationResults();
#if defined(_DEBUG) || defined(DEBUGFAST)
		if (g_ActiveConfig.bEnableShaderDebugging)
		{
			ShaderCode code;
			GeneratePixelShaderCodeD3D11(code, dstAlphaMode, components, xfr, bpm);
			s_pixel_uid_checker.AddToIndexAndCheck(code, uid, "Pixel", "p");
		}
#endif
		// Check if the shader is already set
		if (s_last_entry)
		{
			if (uid == s_last_uid)
			{
				return;
			}
		}
		s_last_uid = uid;
		GFX_DEBUGGER_PAUSE_AT(NEXT_PIXEL_SHADER_CHANGE, true);
	}
	else
	{
		if (s_external_last_uid == uid)
		{
			return;
		}
		s_external_last_uid = uid;
	}
	s_pixel_shaders_lock.lock();
	PSCacheEntry* entry = &s_pixel_shaders[uid];
	s_pixel_shaders_lock.unlock();
	if (ongputhread)
	{
		s_last_entry = entry;
	}
	// Compile only when we have a new instance
	if (entry->initialized.test_and_set())
	{
		return;
	}
	// Need to compile a new shader
	ShaderCode code;
	ShaderCompilerWorkUnit *wunit = s_compiler->NewUnit(PIXELSHADERGEN_BUFFERSIZE);
	code.SetBuffer(wunit->code.data());
	GeneratePixelShaderCodeD3D11(code, dstAlphaMode, components, xfr, bpm);
	wunit->codesize = (u32)code.BufferSize();
	wunit->entrypoint = "main";
	wunit->flags = D3DCOMPILE_SKIP_VALIDATION | D3DCOMPILE_OPTIMIZATION_LEVEL3;
	wunit->target = D3D::PixelShaderVersionString();
	wunit->ResultHandler = [uid, entry](ShaderCompilerWorkUnit* wunit)
	{
		if (SUCCEEDED(wunit->cresult))
		{
			ID3DBlob* shaderBuffer = wunit->shaderbytecode;
			const u8* bytecode = (const u8*)shaderBuffer->GetBufferPointer();
			u32 bytecodelen = (u32)shaderBuffer->GetBufferSize();
			g_ps_disk_cache.Append(uid, bytecode, bytecodelen);
			PushByteCode(bytecode, bytecodelen, entry);
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
			sprintf(szTemp, "%sbad_ps_%04i.txt", File::GetUserPath(D_DUMP_IDX).c_str(), num_failures++);
			std::ofstream file;
			OpenFStream(file, szTemp, std::ios_base::out);
			file << ((const char *)wunit->code.data());
			file << ((const char *)wunit->error->GetBufferPointer());
			file.close();

			PanicAlert("Failed to compile pixel shader!\nThis usually happens when trying to use Dolphin with an outdated GPU or integrated GPU like the Intel GMA series.\n\nIf you're sure this is Dolphin's error anyway, post the contents of %s along with this error message at the forums.\n\nDebug info (%s):\n%s",
				szTemp,
				D3D::PixelShaderVersionString(),
				(char*)wunit->error->GetBufferPointer());
		}
	};
	s_compiler->CompileShaderAsync(wunit);
}

bool PixelShaderCache::TestShader()
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
	return s_last_entry->shader != nullptr && s_last_entry->compiled;
}

void PixelShaderCache::PushByteCode(const void* bytecode, unsigned int bytecodelen, PixelShaderCache::PSCacheEntry* entry)
{
	entry->shader = std::move(D3D::CreatePixelShaderFromByteCode(bytecode, bytecodelen));
	entry->compiled = true;
	if (entry->shader != nullptr)
	{
		// TODO: Somehow make the debug name a bit more specific
		D3D::SetDebugObjectName(entry->shader.get(), "a pixel shader of PixelShaderCache");
		INCSTAT(stats.numPixelShadersCreated);
		SETSTAT(stats.numPixelShadersAlive, s_pixel_shaders.size());
	}
}

void PixelShaderCache::InsertByteCode(const PixelShaderUid &uid, const void* bytecode, unsigned int bytecodelen)
{
	PSCacheEntry* entry = &s_pixel_shaders[uid];
	entry->initialized.test_and_set();
	PushByteCode(bytecode, bytecodelen, entry);
}

}  // DX11
