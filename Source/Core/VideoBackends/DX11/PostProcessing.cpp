// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/CommonPaths.h"
#include "Common/FileUtil.h"
#include "Common/StringUtil.h"

#include "VideoBackends/DX11/D3DTexture.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoBackends/DX11/FramebufferManager.h"
#include "VideoBackends/DX11/PostProcessing.h"

#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"

namespace DX11
{
struct paramsStruct
{
	// Time
	u32 Time;
	// Layer
	int Layer;
	// Gamma
	float native_gamma;
	float padding;
	float resolution[4];
};

struct STVertex { float x, y, z, u0, v0; };

static const char* vertex_shader_code = R"hlsl(
cbuffer ParamBuffer : register(b0) 
{
	uint Time;
	int Layer;
	float native_gamma;
	float padding;
	float4 resolution;
}

struct VSOUTPUT
{
	float4 vPosition : SV_Position;
	float2 vTexCoord : TEXCOORD0;
	float4 vTexCoord1 : TEXCOORD1;
	float4 vTexCoord2 : TEXCOORD2;
};
VSOUTPUT main(float4 inPosition : POSITION,float2 inTEX0 : TEXCOORD0)
{
	VSOUTPUT OUT;
	OUT.vPosition = inPosition;
	OUT.vTexCoord = inTEX0;
	OUT.vTexCoord1 = inTEX0.xyyx + (float4(-0.375f,-0.125f,-0.375f, 0.125f) * resolution.zwwz);
	OUT.vTexCoord2 = inTEX0.xyyx + (float4( 0.375f, 0.125f, 0.375f,-0.125f) * resolution.zwwz);
	return OUT;
})hlsl";

DX11PostProcessing::DX11PostProcessing()
	: m_initialized(false), m_vertexbuffer(0x4000)
{
	const D3D11_INPUT_ELEMENT_DESC simpleelems[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3DBlob blob;
	D3D::CompileShader(D3D::ShaderType::Vertex, vertex_shader_code, blob);
	D3D::device->CreateInputLayout(simpleelems, 2, blob.Data(), blob.Size(), D3D::ToAddr(m_layout));
	m_vshader = D3D::CreateVertexShaderFromByteCode(blob);
	if (m_layout == nullptr ||
		m_vshader == nullptr)
		PanicAlert("Failed to create post processing vertex shader or input layout at %s %d\n", __FILE__, __LINE__);

	D3D::SetDebugObjectName(m_layout.get(), "post processing input layout");
	D3D::SetDebugObjectName(m_vshader.get(), "post processing vertex shader");

	unsigned int cbsize = (sizeof(paramsStruct) + 15) & (~15); // must be always multiple of 16
	D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(cbsize, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	HRESULT hr = D3D::device->CreateBuffer(&cbdesc, nullptr, D3D::ToAddr(m_params));
	CHECK(hr == S_OK, "post processing constant buffer (size=%u)", cbsize);
	D3D::SetDebugObjectName(m_params.get(), "Post processing constant buffer");
	ID3D11SamplerState* samplers[] = { 
		D3D::GetLinearCopySampler(), 
		D3D::GetPointCopySampler()
	};
	D3D::context->PSSetSamplers(9, 2, samplers);
	m_vertex_buffer_observer = true;
	m_vertexbuffer.AddWrapObserver(&m_vertex_buffer_observer);
}

DX11PostProcessing::~DX11PostProcessing()
{
	m_pshader.reset();
	m_vshader.reset();
	m_layout.reset();
	m_params.reset();
	m_options.reset();
}

void DX11PostProcessing::BlitFromTexture(const TargetRectangle &src, const TargetRectangle &dst,
	void* src_texture_ptr, void* src_depth_texture_ptr, int src_width, int src_height, int layer, float gamma)
{
	D3DTexture2D* src_texture = ((D3DTexture2D*)src_texture_ptr);
	D3DTexture2D* src_texture_depth = ((D3DTexture2D*)src_depth_texture_ptr);
	ApplyShader();
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT((float)dst.left, (float)dst.top, (float)dst.GetWidth(), (float)dst.GetHeight());
	D3D::context->RSSetViewports(1, &vp);

	float sw = 1.0f / (float)src_width;
	float sh = 1.0f / (float)src_height;
	float u0 = ((float)src.left) * sw;
	float u1 = ((float)src.right) * sw;
	float v0 = ((float)src.top) * sh;
	float v1 = ((float)src.bottom) * sh;

	paramsStruct params;
	params.native_gamma = 1.0f / gamma;
	params.Layer = layer;
	params.Time = (u32)m_timer.GetTimeElapsed();
	params.resolution[0] = (float)src_width;
	params.resolution[1] = (float)src_height;
	params.resolution[2] = sw;
	params.resolution[3] = sh;

	STVertex coords[4] = {
		{ -1.0f, 1.0f, 0.0f, u0, v0 },
		{ 1.0f, 1.0f, 0.0f, u1, v0 },
		{ -1.0f, -1.0f, 0.0f, u0, v1 },
		{ 1.0f, -1.0f, 0.0f, u1, v1 },
	};
	if (m_vertex_buffer_observer || pu0 != u0 || pu1 != u1 || pv0 != v0 || pv1 != v1)
	{
		m_vertex_buffer_offset = m_vertexbuffer.AppendData(coords, sizeof(coords), sizeof(STVertex));
		pu0 = u0;
		pu1 = u1;
		pv0 = v0;
		pv1 = v1;
	}
	

	D3D11_MAPPED_SUBRESOURCE map;
	D3D::context->Map(m_params.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	memcpy(map.pData, &params, sizeof(params));
	D3D::context->Unmap(m_params.get(), 0);

	if (m_config.IsDirty() && m_config.HasOptions())
	{
		D3D::context->Map(m_options.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		u8* dst = (u8*)map.pData;
		u32 buffer_size = 0;
		for (auto& it : m_config.GetOptions())
		{
			u32 needed_size = 0;
			switch (it.second.m_type)
			{
			case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_BOOL:
				*((int*)dst) = it.second.m_bool_value;
				needed_size = sizeof(int);
				break;
			case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_INTEGER:				
				needed_size = (u32)(it.second.m_integer_values.size() * sizeof(int));
				memcpy(dst, it.second.m_integer_values.data(), needed_size);
				break;
			case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_FLOAT:
				needed_size = (u32)(it.second.m_float_values.size() * sizeof(int));
				memcpy(dst, it.second.m_float_values.data(), needed_size);
				break;
			}
			u32 remaining = ((buffer_size + 15) & (~15)) - buffer_size;
			if (remaining < needed_size)
			{
				// Padding needed to compensate contant buffer padding to 16 bytes
				buffer_size += remaining;
				dst += remaining;
			}
			buffer_size += needed_size;
			dst += needed_size;
			it.second.m_dirty = false;
		}
		D3D::context->Unmap(m_options.get(), 0);
		m_config.SetDirty(false);
	}
	D3D::stateman->SetVertexConstants(m_params.get());
	D3D::stateman->SetPixelConstants(m_params.get(), m_options.get());
	D3D::stateman->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	D3D::stateman->SetInputLayout(m_layout.get());
	D3D::stateman->SetVertexBuffer(m_vertexbuffer.GetBuffer(), sizeof(STVertex), 0);
	D3D::stateman->SetPixelShader(m_pshader.get());
	ID3D11ShaderResourceView* views[2] = {
		src_texture->GetSRV(),
		nullptr
	};
	
	if (src_texture_depth != nullptr)
	{
		views[1] = src_texture_depth->GetSRV();	
	}
	D3D::context->PSSetShaderResources(9, 2, views);
	D3D::stateman->SetVertexShader(m_vshader.get());
	D3D::stateman->SetGeometryShader(nullptr);

	D3D::stateman->Apply();
	D3D::context->Draw(4, m_vertex_buffer_offset);
	views[0] = nullptr;
	views[1] = nullptr;
	D3D::context->PSSetShaderResources(9, 2, views);
}

void DX11PostProcessing::ApplyShader()
{
	// shader didn't changed
	if (m_initialized && m_config.GetShader() == g_ActiveConfig.sPostProcessingShader)
		return;

	m_pshader.reset();

	// load shader code
	std::string code = m_config.LoadShader();
	code = LoadShaderOptions(code);

	m_pshader = D3D::CompileAndCreatePixelShader(code);

	// and compile it
	if (!m_pshader)
	{
		ERROR_LOG(VIDEO, "Failed to compile post-processing shader %s", m_config.GetShader().c_str());
		g_Config.sPostProcessingShader.clear();
		g_ActiveConfig.sPostProcessingShader.clear();
		code = LoadShaderOptions(m_config.LoadShader());
		m_pshader = D3D::CompileAndCreatePixelShader(code);
	}
	m_initialized = true;
}

static const std::string s_hlsl_header = R"hlsl(
// Required variables
// Shouldn't be accessed directly by the PP shader
// Texture sampler
sampler samp8 : register(s8);
sampler samp9 : register(s9);
sampler samp10 : register(s10);
Texture2D Tex8 : register(t8);
Texture2DArray Tex9 : register(t9);
Texture2DArray Tex10 : register(t10);
cbuffer ParamBuffer : register(b0) 
{
	uint Time;
	int layer;
	float native_gamma;
	float padding;
	float4 resolution;
}

// Globals
static float2 uv0;
static float4 uv1, uv2, fragment_pos;
// Interfacing functions
float2 GetFragmentCoord()
{
	return fragment_pos.xy;
}
float4 Sample(float2 location, int l)
{
	return Tex9.Sample(samp9, float3(location, l));
}
float4 SampleLocationOffset(float2 location, int2 offset)
{
	return Tex9.Sample(samp9, float3(location, layer), offset);
}
float SampleDepth(float2 location, int l)
{
	/*float Znear = 0.001;
	float Zfar = 1.0;
	float A  = (1 - ( Zfar / Znear ))/2;
	float B = (1 + ( Zfar / Znear ))/2;*/
	float A = -499.5;
	float B =  500.5;
	float depth = 1.0 - Tex10.Sample(samp10, float3(location, l)).x;
	depth = 1.0 / (A * depth + B);
	return depth;
}
float SampleDepthLoacationOffset(float2 location, int2 offset)
{
	float A = -499.5;
	float B =  500.5;
	float depth = 1.0 - Tex10.Sample(samp10, float3(location, layer), offset).x;
	depth = 1.0 / (A * depth + B);
	return depth;
}

float4 Sample() { return Sample(uv0, layer); }
float4 SampleOffset(int2 offset) { return SampleLocationOffset(uv0, offset); }
float SampleDepth() { return SampleDepth(uv0, layer); }
float SampleDepthOffset(int2 offset) { return SampleDepthLoacationOffset(uv0, offset); }
float4 SampleLocation(float2 location) { return Sample(location, layer); }
float SampleDepthLocation(float2 location) { return SampleDepth(location, layer); }
float4 SampleLayer(int l) { return Sample(uv0, l); }
float SampleDepthLayer(int l) { return SampleDepth(uv0, l); }
float4 SampleFontLocation(float2 location) { return Tex8.Sample(samp8, location); }

float4 ApplyGCGamma(float4 col)
{
	return pow(col, native_gamma);
}
float2 GetResolution()
{
	return resolution.xy;
}
float2 GetInvResolution()
{
	return resolution.zw;
}
float2 GetCoordinates()
{
	return uv0;
}
float2 GetSourceTextureSize()
{
	uint Width, Height, Elements, NumberOfLevels;
	Tex9.GetDimensions(0, Width, Height, Elements, NumberOfLevels);
	return float2(Width, Height);
}
uint GetTime()
{
	return Time;
}

#define SetOutput(color) ocol0 = color
#define mult(a, b) mul(b, a)
#define GetOption(x) (option_##x)
#define OptionEnabled(x) (option_##x != 0)
)hlsl";

static const std::string s_hlsl_entry = "void main(\n"
"out float4 ocol0 : SV_Target,\n"
"in float4 frag_pos : SV_Position,\n"
"in float2 _uv0 : TEXCOORD0,\n"
"in float4 _uv1 : TEXCOORD1,\n"
"in float4 _uv2 : TEXCOORD2)\n"
"{\n"
"fragment_pos = frag_pos;\n"
"uv0 = _uv0;\n"
"uv1 = _uv1;\n"
"uv2 = _uv2;\n";

static const std::string glslentry = "void main()";

std::string DX11PostProcessing::LoadShaderOptions(const std::string& code)
{
	m_options.reset();
	std::string hlsl_options = "";
	if (m_config.HasOptions())
	{
		hlsl_options = "cbuffer OptionBuffer : register(b1) {";
		u32 buffer_size = 0;
		for (const auto& it : m_config.GetOptions())
		{
			u32 needed_size = 0;
			if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_BOOL)
			{
				hlsl_options += StringFromFormat("int     option_%s;\n", it.first.c_str());
				needed_size = sizeof(int);
			}
			else if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_INTEGER)
			{
				u32 count = static_cast<u32>(it.second.m_integer_values.size());
				if (count < 2)
					hlsl_options += StringFromFormat("int     option_%s;\n", it.first.c_str());
				else
					hlsl_options += StringFromFormat("int%d   option_%s;\n", count, it.first.c_str());
				needed_size = sizeof(int) * count;
			}
			else if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_FLOAT)
			{
				u32 count = static_cast<u32>(it.second.m_float_values.size());
				if (count < 2)
					hlsl_options += StringFromFormat("float   option_%s;\n", it.first.c_str());
				else
					hlsl_options += StringFromFormat("float%d option_%s;\n", count, it.first.c_str());
				needed_size = sizeof(float) * count;
			}
			u32 remaining = ((buffer_size + 15) & (~15)) - buffer_size;
			if (remaining < needed_size)
			{
				// Padding needed to compensate contant buffer padding to 16 bytes
				buffer_size += remaining;
			}
			buffer_size += needed_size;
		}
		hlsl_options += "}\n";
		if (buffer_size > 0)
		{
			buffer_size = (buffer_size + 15) & (~15);
			D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(buffer_size, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
			HRESULT hr = D3D::device->CreateBuffer(&cbdesc, nullptr, D3D::ToAddr(m_options));
			CHECK(hr == S_OK, "post processing options constant buffer (size=%u)", buffer_size);
			D3D::SetDebugObjectName(m_options.get(), "Post processing options constant buffer");
		}
		else
		{
			hlsl_options = "";
		}
	}
	
	size_t entryPointStart = code.find(glslentry);
	if (entryPointStart == std::string::npos)
	{
		return "";
	}
	size_t entryPointEnd = code.find("{", entryPointStart);
	if (entryPointEnd == std::string::npos)
	{
		return "";
	}
	std::string newcode = code.substr(0, entryPointStart) + s_hlsl_entry + code.substr(entryPointEnd + 1);
	return s_hlsl_header + hlsl_options + newcode;
}

}  // namespace OGL