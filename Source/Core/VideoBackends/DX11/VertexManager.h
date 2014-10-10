// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "VideoCommon/VertexManagerBase.h"
#include "VideoBackends/DX11/LineAndPointGeometryShader.h"

namespace DX11
{

class VertexManager : public ::VertexManager
{
public:
	VertexManager();
	~VertexManager();

	NativeVertexFormat* CreateNativeVertexFormat();
	void CreateDeviceObjects();
	void DestroyDeviceObjects();
	void PrepareShaders(u32 components, 
		const XFRegisters &xfr, 
		const BPMemory &bpm, 
		bool fromgputhread = true);
private:
	
	void PrepareDrawBuffers(u32 stride);
	void Draw(u32 stride);
	// temp
	void vFlush();

	u32 m_vertex_buffer_cursor;
	u32 m_vertex_draw_offset;
	u32 m_index_buffer_cursor;
	u32 m_current_vertex_buffer;
	u32 m_current_index_buffer;
	u32 m_triangle_draw_index;
	u32 m_line_draw_index;
	u32 m_point_draw_index;
	typedef ID3D11Buffer* PID3D11Buffer;
	PID3D11Buffer* m_index_buffers;
	PID3D11Buffer* m_vertex_buffers;

	LineAndPointGeometryShader m_lineAndPointShader;
};

}  // namespace