// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "VideoCommon/CPMemory.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexManagerBase.h"

#include "VideoBackends/DX9/VertexShaderCache.h"
#include "VideoBackends/DX9/PixelShaderCache.h"

namespace DX9
{

class VertexManager : public ::VertexManager
{
public:
	NativeVertexFormat* CreateNativeVertexFormat();
	void GetElements(NativeVertexFormat* format, D3DVERTEXELEMENT9** elems, int* num);
	void CreateDeviceObjects();
	void DestroyDeviceObjects();
	void PrepareShaders(u32 components, const XFMemory &xfr, const BPMemory &bpm, bool ongputhread = true);
private:
	u32 m_vertex_buffer_cursor;
	u32 m_vertex_buffer_size;
	u32 m_index_buffer_cursor;
	u32 m_index_buffer_size;
	u32 m_buffers_count;
	u32 m_current_vertex_buffer;
	u32 m_current_stride;
	u32 m_stride;
	u32 m_current_index_buffer;
	bool m_line_emulation_required;
	bool m_point_emulation_required;
	u32 m_total_index_len;
	u32 m_total_num_verts;
	u32 m_num_verts;
	u32 m_triangle_index_len;
	u32 m_line_index_len;
	u32 m_point_index_len;	
	u32 m_triangles;
	u32 m_lines;
	u32 m_points;

	LPDIRECT3DVERTEXBUFFER9 *m_vertex_buffers;
	LPDIRECT3DINDEXBUFFER9 *m_index_buffers; 
	void PrepareDrawBuffers();
	void Draw();
	void SetPLRasterOffsets();
	// temp
	void vFlush();
};

}