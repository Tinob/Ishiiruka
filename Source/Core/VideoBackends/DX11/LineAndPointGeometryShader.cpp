// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <string>

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DShader.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoBackends/DX11/LineAndPointGeometryShader.h"
#include "VideoCommon/VertexShaderGen.h"

namespace DX11
{

	static const char HEADER_AND_PARAMS_GS_COMMON[] = R"HLSL(
cbuffer cbParams : register(b0)
{
	struct // Should match LineGSParams above
	{
		float LineWidth;
		float PointSize;
		float TexOffset;
		uint padding;
		float VpWidth;
		float VpHeight;
		uint padding1;
		uint padding2;
		float TexOffsetEnable[8];
	} Params;
}
)HLSL";

	static const char LINE_GS_COMMON[] = R"HLSL(
[maxvertexcount(4)]
void main(line VS_OUTPUT input[2], inout TriangleStream<VS_OUTPUT> outStream)
{
	// Pretend input[0] is on the bottom and input[1] is on top.
	// We generate vertices to the left and right.

	VS_OUTPUT l0 = input[0];
	VS_OUTPUT r0 = l0;
	VS_OUTPUT l1 = input[1];
	VS_OUTPUT r1 = l1;

	// GameCube/Wii's line drawing algorithm is a little quirky. It does not
	// use the correct line caps. Instead, the line caps are vertical or
	// horizontal depending the slope of the line.

	float2 offset;
	float2 to = abs(input[1].pos.xy - input[0].pos.xy);
	// FIXME: What does real hardware do when line is at a 45-degree angle?
	// FIXME: Lines aren't drawn at the correct width. See Twilight Princess map.
	if (Params.VpHeight*to.y > Params.VpWidth*to.x) {
		// Line is more tall. Extend geometry left and right.
		// Lerp Params.LineWidth/2 from [0..VpWidth] to [-1..1]
		offset = float2(Params.LineWidth/Params.VpWidth, 0);
	} else {
		// Line is more wide. Extend geometry up and down.
		// Lerp Params.LineWidth/2 from [0..VpHeight] to [1..-1]
		offset = float2(0, -Params.LineWidth/Params.VpHeight);
	}

	l0.pos.xy -= offset * input[0].pos.w;
	r0.pos.xy += offset * input[0].pos.w;
	l1.pos.xy -= offset * input[1].pos.w;
	r1.pos.xy += offset * input[1].pos.w;

#ifndef NUM_TEXCOORDS
#error NUM_TEXCOORDS not defined
#endif

	// Apply TexOffset to all tex coordinates in the vertex.
	// They can each be enabled seperately.
#if NUM_TEXCOORDS >= 1
	r0.tex0.x += Params.TexOffset * Params.TexOffsetEnable[0];
	r1.tex0.x += Params.TexOffset * Params.TexOffsetEnable[0];
#endif
#if NUM_TEXCOORDS >= 2
	r0.tex1.x += Params.TexOffset * Params.TexOffsetEnable[1];
	r1.tex1.x += Params.TexOffset * Params.TexOffsetEnable[1];
#endif
#if NUM_TEXCOORDS >= 3
	r0.tex2.x += Params.TexOffset * Params.TexOffsetEnable[2];
	r1.tex2.x += Params.TexOffset * Params.TexOffsetEnable[2];
#endif
#if NUM_TEXCOORDS >= 4
	r0.tex3.x += Params.TexOffset * Params.TexOffsetEnable[3];
	r1.tex3.x += Params.TexOffset * Params.TexOffsetEnable[3];
#endif
#if NUM_TEXCOORDS >= 5
	r0.tex4.x += Params.TexOffset * Params.TexOffsetEnable[4];
	r1.tex4.x += Params.TexOffset * Params.TexOffsetEnable[4];
#endif
#if NUM_TEXCOORDS >= 6
	r0.tex5.x += Params.TexOffset * Params.TexOffsetEnable[5];
	r1.tex5.x += Params.TexOffset * Params.TexOffsetEnable[5];
#endif
#if NUM_TEXCOORDS >= 7
	r0.tex6.x += Params.TexOffset * Params.TexOffsetEnable[6];
	r1.tex6.x += Params.TexOffset * Params.TexOffsetEnable[6];
#endif
#if NUM_TEXCOORDS >= 8
	r0.tex7.x += Params.TexOffset * Params.TexOffsetEnable[7];
	r1.tex7.x += Params.TexOffset * Params.TexOffsetEnable[7];
#endif

	outStream.Append(l0);
	outStream.Append(r0);
	outStream.Append(l1);
	outStream.Append(r1);
}
;
)HLSL";

	static const char POINT_GS_COMMON[] = R"HLSL(
[maxvertexcount(4)]
void main(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> outStream)
{
	VS_OUTPUT ptLL = input[0];
	VS_OUTPUT ptLR = ptLL;
	VS_OUTPUT ptUL = ptLL;
	VS_OUTPUT ptUR = ptLL;

	// Offset from center to upper right vertex
	// Lerp Params.PointSize/2 from [0,0..VpWidth,VpHeight] to [-1,1..1,-1]
	float2 offset = float2(Params.PointSize/Params.VpWidth, -Params.PointSize/Params.VpHeight) * input[0].pos.w;

	ptLL.pos.xy += float2(-1,-1) * offset;
	ptLR.pos.xy += float2(1,-1) * offset;
	ptUL.pos.xy += float2(-1,1) * offset;
	ptUR.pos.xy += offset;

	float2 texOffset = float2(Params.TexOffset, Params.TexOffset);

#ifndef NUM_TEXCOORDS
#error NUM_TEXCOORDS not defined
#endif

	// Apply TexOffset to all tex coordinates in the vertex
	// FIXME: The game may be able to enable TexOffset for some coords and
	// disable for others, but where is that information stored?
#if NUM_TEXCOORDS >= 1
	ptLL.tex0.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[0];
	ptLR.tex0.xy += texOffset * Params.TexOffsetEnable[0];
	ptUR.tex0.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[0];
#endif
#if NUM_TEXCOORDS >= 2
	ptLL.tex1.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[1];
	ptLR.tex1.xy += texOffset * Params.TexOffsetEnable[1];
	ptUR.tex1.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[1];
#endif
#if NUM_TEXCOORDS >= 3
	ptLL.tex2.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[2];
	ptLR.tex2.xy += texOffset * Params.TexOffsetEnable[2];
	ptUR.tex2.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[2];
#endif
#if NUM_TEXCOORDS >= 4
	ptLL.tex3.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[3];
	ptLR.tex3.xy += texOffset * Params.TexOffsetEnable[3];
	ptUR.tex3.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[3];
#endif
#if NUM_TEXCOORDS >= 5
	ptLL.tex4.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[4];
	ptLR.tex4.xy += texOffset * Params.TexOffsetEnable[4];
	ptUR.tex4.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[4];
#endif
#if NUM_TEXCOORDS >= 6
	ptLL.tex5.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[5];
	ptLR.tex5.xy += texOffset * Params.TexOffsetEnable[5];
	ptUR.tex5.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[5];
#endif
#if NUM_TEXCOORDS >= 7
	ptLL.tex6.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[6];
	ptLR.tex6.xy += texOffset * Params.TexOffsetEnable[6];
	ptUR.tex6.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[6];
#endif
#if NUM_TEXCOORDS >= 8
	ptLL.tex7.xy += float2(0,1) * texOffset * Params.TexOffsetEnable[7];
	ptLR.tex7.xy += texOffset * Params.TexOffsetEnable[7];
	ptUR.tex7.xy += float2(1,0) * texOffset * Params.TexOffsetEnable[7];
#endif

	outStream.Append(ptLL);
	outStream.Append(ptLR);
	outStream.Append(ptUL);
	outStream.Append(ptUR);
}
)HLSL";

	void LineAndPointGeometryShader::Init()
	{
		m_ready = false;

		HRESULT hr;

		// Create constant buffer for uploading data to geometry shader

		D3D11_BUFFER_DESC bd = CD3D11_BUFFER_DESC(sizeof(GSParams), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DEFAULT, 0);
		hr = D3D::device->CreateBuffer(&bd, nullptr, D3D::ToAddr(m_paramsBuffer));
		CHECK(SUCCEEDED(hr), "create line geometry shader params buffer");
		D3D::SetDebugObjectName(m_paramsBuffer.get(), "line geometry shader params buffer");
		memset(&m_shadowParamsBuffer, 0, sizeof(GSParams));
		m_ready = true;
	}

	void LineAndPointGeometryShader::Shutdown()
	{
		m_ready = false;

		m_lineShaders.clear();
		m_pointShaders.clear();
		m_paramsBuffer.reset();
	}

	bool LineAndPointGeometryShader::SetLineShader(u32 components, float lineWidth,
		float texOffset, float vpWidth, float vpHeight, const bool* texOffsetEnable)
	{
		if (!m_ready)
			return false;

		auto shader = GetShader(components, true);

		if (!shader)
			return false;

		UpdateConstantBuffer(lineWidth, m_shadowParamsBuffer.PointSize, texOffset, vpWidth, vpHeight, texOffsetEnable);
		DEBUG_LOG(VIDEO, "Line params: width %f, texOffset %f, vpWidth %f, vpHeight %f",
			lineWidth, texOffset, vpWidth, vpHeight);

		D3D::stateman->SetGeometryShader(shader);
		D3D::stateman->SetGeometryConstants(m_paramsBuffer.get());
		return true;
	}

	bool LineAndPointGeometryShader::SetPointShader(u32 components, float pointSize,
		float texOffset, float vpWidth, float vpHeight, const bool* texOffsetEnable)
	{
		if (!m_ready)
			return false;

		auto shader = GetShader(components, false);

		if (!shader)
			return false;

		UpdateConstantBuffer(m_shadowParamsBuffer.LineWidth, pointSize, texOffset, vpWidth, vpHeight, texOffsetEnable);
		DEBUG_LOG(VIDEO, "Point params: size %f, texOffset %f, vpWidth %f, vpHeight %f",
			pointSize, texOffset, vpWidth, vpHeight);

		D3D::stateman->SetGeometryShader(shader);
		D3D::stateman->SetGeometryConstants(m_paramsBuffer.get());
		return true;
	}

	ID3D11GeometryShader* LineAndPointGeometryShader::GetShader(u32 components, bool line)
	{
		if (!m_ready)
			return nullptr;

		auto & map = line ? m_lineShaders : m_pointShaders;
		auto type = line ? "Line" : "Point";

		// Make sure geometry shader for "components" is available
		auto it = map.find(components);
		if (it != map.end())
			return it->second.get();

		// Generate new shader. Warning: not thread-safe.
		char buffer[16384];
		ShaderCode code;
		code.SetBuffer(buffer);
		GenerateVSOutputStructForGSD3D11(code, xfmem);
		code.Write("\n%s", HEADER_AND_PARAMS_GS_COMMON);
		code.Write("\n%s", line ? LINE_GS_COMMON : POINT_GS_COMMON);

		INFO_LOG(VIDEO, "Compiling %s geometry shader for components 0x%.08X (num texcoords %d)", type,
			components, xfmem.numTexGen.numTexGens);

		auto numTexCoordsStr = std::to_string(xfmem.numTexGen.numTexGens);
		D3D_SHADER_MACRO macros[] = {
				{ "NUM_TEXCOORDS", numTexCoordsStr.c_str() },
				{ nullptr, nullptr }
		};
		D3D::GeometryShaderPtr newShader = D3D::CompileAndCreateGeometryShader(code.GetBuffer(), macros);
		if (!newShader)
		{
			WARN_LOG(VIDEO, "%s geometry shader for components 0x%.08X failed to compile", type, components);
			// Add dummy shader to prevent trying to compile again
			map.emplace(components, nullptr);
			return nullptr;
		}

		it = map.emplace(components, std::move(newShader)).first;
		return it->second.get();
	}

	void LineAndPointGeometryShader::UpdateConstantBuffer(float lineWidth, float pointSize, float texOffset, float vpWidth, float vpHeight, const bool* texOffsetEnable) {
		GSParams newParms;
		newParms.LineWidth = lineWidth;
		newParms.PointSize = pointSize;
		newParms.TexOffset = texOffset;
		newParms.VpWidth = vpWidth;
		newParms.VpHeight = vpHeight;
		for (int i = 0; i < 8; ++i)
			newParms.TexOffsetEnable[i] = texOffsetEnable[i] ? 1.f : 0.f;
		newParms.ClearPadding();

		if (memcmp(&newParms, &m_shadowParamsBuffer, sizeof(GSParams))) {
			D3D::context->UpdateSubresource(m_paramsBuffer.get(), 0, nullptr, &newParms, sizeof(GSParams), 0);
			m_shadowParamsBuffer = newParms;
		}
	}

}
