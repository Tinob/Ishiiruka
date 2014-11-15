// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "D3DBase.h"
#include "PixelShaderCache.h"
#include "VertexManager.h"
#include "VertexShaderCache.h"

#include "VideoCommon/BPMemory.h"
#include "VideoCommon/Debugger.h"
#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/MainBase.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/RenderBase.h"
#include "Render.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoConfig.h"

// internal state for loading vertices
extern NativeVertexFormat *g_nativeVertexFmt;

namespace DX11
{

void VertexManager::CreateDeviceObjects()
{
	D3D11_BUFFER_DESC bufdesc = CD3D11_BUFFER_DESC(MAX_BUFFER_SIZE,
		D3D11_BIND_INDEX_BUFFER | D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
	m_vertexDrawOffset = 0;
	m_indexDrawOffset = 0;
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
	{
		m_buffers[i] = nullptr;
		CHECK(SUCCEEDED(D3D::device->CreateBuffer(&bufdesc, nullptr, D3D::ToAddr(m_buffers[i]))), "Failed to create buffer.");
		D3D::SetDebugObjectName((ID3D11DeviceChild*)m_buffers[i].get(), "Buffer of VertexManager");
	}
	m_currentBuffer = 0;
	m_bufferCursor = MAX_BUFFER_SIZE;
	m_lineAndPointShader.Init();
}

void VertexManager::DestroyDeviceObjects()
{
	m_lineAndPointShader.Shutdown();
	for (int i = 0; i < MAX_BUFFER_COUNT; i++)
	{
		m_buffers[i].reset();
	}
}

VertexManager::VertexManager()
{
	CreateDeviceObjects();
}

VertexManager::~VertexManager()
{
	DestroyDeviceObjects();
}

void VertexManager::PrepareDrawBuffers(u32 stride)
{
	D3D11_MAPPED_SUBRESOURCE map;
	u32 vertexBufferSize = u32(s_pCurBufferPointer - s_pBaseBufferPointer);
	u32 tindexbuffersize = IndexGenerator::GetTriangleindexLen() * sizeof(u16);
	u32 lindexbuffersize = IndexGenerator::GetLineindexLen() * sizeof(u16);
	u32 pindexbuffersize = IndexGenerator::GetPointindexLen() * sizeof(u16);
	u32 indexBufferSize = tindexbuffersize + lindexbuffersize + pindexbuffersize;
	u32 totalBufferSize = vertexBufferSize + indexBufferSize;
	u32 cursor = m_bufferCursor;
	u32 padding = m_bufferCursor % stride;
	if (padding)
	{
		cursor += stride - padding;
	}
	D3D11_MAP MapType = D3D11_MAP_WRITE_NO_OVERWRITE;
	if (cursor + totalBufferSize >= MAX_BUFFER_SIZE)
	{
		// Wrap around
		m_currentBuffer = (m_currentBuffer + 1) % MAX_BUFFER_COUNT;
		cursor = 0;
		MapType = D3D11_MAP_WRITE_DISCARD;
	}
	m_vertexDrawOffset = cursor;
	m_indexDrawOffset = cursor + vertexBufferSize;
	u32 lineDrawoffset = m_indexDrawOffset + tindexbuffersize;
	u32 pointDrawoffset = lineDrawoffset + lindexbuffersize;
	D3D::context->Map(m_buffers[m_currentBuffer].get(), 0, MapType, 0, &map);
	u8* mappedData = reinterpret_cast<u8*>(map.pData);
	memcpy(mappedData + m_vertexDrawOffset, s_pBaseBufferPointer, vertexBufferSize);
	if (tindexbuffersize)
	{
		memcpy(mappedData + m_indexDrawOffset, GetTriangleIndexBuffer(), tindexbuffersize);
	}
	if (lindexbuffersize)
	{
		memcpy(mappedData + lineDrawoffset, GetLineIndexBuffer(), lindexbuffersize);
	}
	if (pindexbuffersize)
	{
		memcpy(mappedData + pointDrawoffset, GetPointIndexBuffer(), pindexbuffersize);
	}
	D3D::context->Unmap(m_buffers[m_currentBuffer].get(), 0);
	m_bufferCursor = cursor + totalBufferSize;
	ADDSTAT(stats.thisFrame.bytesVertexStreamed, vertexBufferSize);
	ADDSTAT(stats.thisFrame.bytesIndexStreamed, indexBufferSize);
}

static const float LINE_PT_TEX_OFFSETS[8] = {
	0.f, 0.0625f, 0.125f, 0.25f, 0.5f, 1.f, 1.f, 1.f
};

void VertexManager::Draw(UINT stride)
{
	D3D::context->IASetVertexBuffers(0, 1, D3D::ToAddr(m_buffers[m_currentBuffer]), &stride, &m_vertexDrawOffset);
	D3D::context->IASetIndexBuffer(m_buffers[m_currentBuffer].get(), DXGI_FORMAT_R16_UINT, m_indexDrawOffset);
	u32 indexstride = 0;
	u32 indexcount = IndexGenerator::GetTriangleindexLen();
	if (IndexGenerator::GetNumTriangles() > 0)
	{
		auto pt = g_ActiveConfig.backend_info.bSupportsPrimitiveRestart ?
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP :
												D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		D3D::context->IASetPrimitiveTopology(pt);
		D3D::context->GSSetShader(nullptr, nullptr, 0);

		D3D::context->DrawIndexed(indexcount, indexstride, 0);
		INCSTAT(stats.thisFrame.numIndexedDrawCalls);
	}
	// Disable culling for lines and points
	if (IndexGenerator::GetNumLines() > 0 || IndexGenerator::GetNumPoints() > 0)
		((DX11::Renderer*)g_renderer)->ApplyCullDisable();
	if (IndexGenerator::GetNumLines() > 0)
	{
		indexstride += indexcount;
		float lineWidth = float(bpmem.lineptwidth.linesize) / 6.f;
		float texOffset = LINE_PT_TEX_OFFSETS[bpmem.lineptwidth.lineoff];
		float vpWidth = 2.0f * xfmem.viewport.wd;
		float vpHeight = -2.0f * xfmem.viewport.ht;

		bool texOffsetEnable[8];

		for (int i = 0; i < 8; ++i)
			texOffsetEnable[i] = bpmem.texcoords[i].s.line_offset;

		if (m_lineAndPointShader.SetLineShader(g_nativeVertexFmt->m_components, lineWidth,
			texOffset, vpWidth, vpHeight, texOffsetEnable))
		{
			D3D::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			indexcount = IndexGenerator::GetLineindexLen();
			D3D::context->DrawIndexed(indexcount, indexstride, 0);
			INCSTAT(stats.thisFrame.numIndexedDrawCalls);

			D3D::context->GSSetShader(nullptr, nullptr, 0);
		}
	}
	if (IndexGenerator::GetNumPoints() > 0)
	{
		indexstride += indexcount;
		float pointSize = float(bpmem.lineptwidth.pointsize) / 6.f;
		float texOffset = LINE_PT_TEX_OFFSETS[bpmem.lineptwidth.pointoff];
		float vpWidth = 2.0f * xfmem.viewport.wd;
		float vpHeight = -2.0f * xfmem.viewport.ht;

		bool texOffsetEnable[8];

		for (int i = 0; i < 8; ++i)
			texOffsetEnable[i] = bpmem.texcoords[i].s.point_offset;

		if (m_lineAndPointShader.SetPointShader(g_nativeVertexFmt->m_components, pointSize,
			texOffset, vpWidth, vpHeight, texOffsetEnable))
		{
			D3D::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			D3D::context->DrawIndexed(IndexGenerator::GetPointindexLen(), indexstride, 0);
			INCSTAT(stats.thisFrame.numIndexedDrawCalls);

			D3D::context->GSSetShader(nullptr, nullptr, 0);
		}
	}
	if (IndexGenerator::GetNumLines() > 0 || IndexGenerator::GetNumPoints() > 0)
		((DX11::Renderer*)g_renderer)->RestoreCull();
}

void VertexManager::PrepareShaders(u32 components, const XFMemory &xfr, const BPMemory &bpm, bool ongputhread)
{
	if (ongputhread)
	{
		if (!s_Shader_Refresh_Required)
		{
			return;
		}
		s_Shader_Refresh_Required = false;
	}
	bool useDstAlpha = !g_ActiveConfig.bDstAlphaPass && bpm.dstalpha.enable && bpm.blendmode.alphaupdate &&
		bpm.zcontrol.pixel_format == PEControl::RGBA6_Z24;
	VertexShaderCache::PrepareShader(components, xfr, bpm, ongputhread);
	PixelShaderCache::PrepareShader(useDstAlpha ? DSTALPHA_DUAL_SOURCE_BLEND : DSTALPHA_NONE, components, xfr, bpm, ongputhread);
}

void VertexManager::vFlush()
{
	u32 usedtextures = 0;
	for (u32 i = 0; i < (u32)bpmem.genMode.numtevstages + 1; ++i)
		if (bpmem.tevorders[i / 2].getEnable(i & 1))
			usedtextures |= 1 << bpmem.tevorders[i / 2].getTexMap(i & 1);

	if (bpmem.genMode.numindstages > 0)
		for (unsigned int i = 0; i < bpmem.genMode.numtevstages + 1; ++i)
			if (bpmem.tevind[i].IsActive() && bpmem.tevind[i].bt < bpmem.genMode.numindstages)
				usedtextures |= 1 << bpmem.tevindref.getTexMap(bpmem.tevind[i].bt);

	for (unsigned int i = 0; i < 8; i++)
	{
		if (usedtextures & (1 << i))
		{
			g_renderer->SetSamplerState(i & 3, i >> 2);
			const FourTexUnits &tex = bpmem.tex[i >> 2];
			const TextureCache::TCacheEntryBase* tentry = TextureCache::Load(i,
				(tex.texImage3[i & 3].image_base/* & 0x1FFFFF*/) << 5,
				tex.texImage0[i & 3].width + 1, tex.texImage0[i & 3].height + 1,
				tex.texImage0[i & 3].format, tex.texTlut[i & 3].tmem_offset << 9,
				tex.texTlut[i & 3].tlut_format,
				(tex.texMode0[i & 3].min_filter & 3) != 0,
				(tex.texMode1[i & 3].max_lod + 0xf) / 0x10,
				tex.texImage1[i & 3].image_type != 0);

			if (tentry)
			{
				// 0s are probably for no manual wrapping needed.
				PixelShaderManager::SetTexDims(i, tentry->native_width, tentry->native_height, 0, 0);
			}
			else
				ERROR_LOG(VIDEO, "error loading texture");
		}
	}

	if (!PixelShaderCache::TestShader())
	{
		return;
	}
	if (!VertexShaderCache::TestShader())
	{
		return;
	}
	// set global constants
	VertexShaderManager::SetConstants();
	PixelShaderManager::SetConstants();

	bool useDstAlpha = !g_ActiveConfig.bDstAlphaPass && bpmem.dstalpha.enable && bpmem.blendmode.alphaupdate &&
		bpmem.zcontrol.pixel_format == PEControl::RGBA6_Z24;

	unsigned int stride = g_nativeVertexFmt->GetVertexStride();
	PrepareDrawBuffers(stride);
	g_nativeVertexFmt->SetupVertexPointers();
	g_renderer->ApplyState(useDstAlpha);

	g_perf_query->EnableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);
	Draw(stride);
	g_perf_query->DisableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);

	GFX_DEBUGGER_PAUSE_AT(NEXT_FLUSH, true);

	g_renderer->RestoreState();
}

}  // namespace
