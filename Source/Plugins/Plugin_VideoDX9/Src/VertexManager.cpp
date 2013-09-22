// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common.h"
#include "FileUtil.h"

#include "D3DBase.h"
#include "Fifo.h"
#include "Statistics.h"
#include "VertexManager.h"
#include "OpcodeDecoding.h"
#include "IndexGenerator.h"
#include "VertexShaderManager.h"
#include "VertexShaderCache.h"
#include "PixelShaderManager.h"
#include "PixelShaderCache.h"
#include "NativeVertexFormat.h"
#include "TextureCache.h"
#include "main.h"

#include "BPStructs.h"
#include "XFStructs.h"
#include "Debugger.h"
#include "VideoConfig.h"

// internal state for loading vertices
extern NativeVertexFormat *g_nativeVertexFmt;
namespace DX9
{
// This are the initially requeted size for the buffers expresed in elements
const u32 IBUFFER_SIZE = VertexManager::MAXIBUFFERSIZE * sizeof(u16) * 8;
const u32 VBUFFER_SIZE = VertexManager::MAXVBUFFERSIZE;
const u32 MAX_VBUFFER_COUNT = 2;
// Register count for line/point offset storage
const u32 LINE_PT_OFFSETS_PARAMS_LEN = C_VENVCONST_END - C_PLOFFSETPARAMS;

#define PLO_ZERO 0

#define PLO_TEX_MASK_POINT_0_3 1
#define PLO_TEX_MASK_POINT_4_7 2
#define PLO_TEX_MASK_LINE_0_3 3
#define PLO_TEX_MASK_LINE_4_7 4

#define PLO_TEX_POINT_X 5
#define PLO_TEX_POINT_Y 6
#define PLO_TEX_POINT_XY 7

#define PLO_TEX_LINE 8

#define PLO_POS_POINT_LEFT_TOP 5
#define PLO_POS_POINT_LEFT_BOTTOM 6
#define PLO_POS_POINT_RIGHT_TOP 7
#define PLO_POS_POINT_RIGHT_BOTTOM 8

#define PLO_POS_LINE_NEGATIVE_X 9
#define PLO_POS_LINE_NEGATIVE_Y 10
#define PLO_POS_LINE_POSITIVE_X 11
#define PLO_POS_LINE_POSITIVE_Y 12


typedef struct Float_3
{
	float x,y,z;
}Float_3;

typedef struct Float_4
{
	Float_4(float px, float py, float pz, float pw)
	{
		x = px;
		y = py;
		z = pz;
		w = pw;
	}
	Float_4()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}
	float x,y,z,w;
}Float_4;

typedef struct U8_4
{
	u8 x,y,z,w;
}U8_4;

static const float LINE_PT_TEX_OFFSETS[8] = {
	0.f, 
	0.0625f, 
	0.125f, 
	0.25f, 
	0.5f, 
	1.f, 
	1.f, 
	1.f
};
static Float_4 s_line_pt_params[LINE_PT_OFFSETS_PARAMS_LEN];

inline void DumpBadShaders()
{
#if defined(_DEBUG) || defined(DEBUGFAST)
	// TODO: Reimplement!
	/*	std::string error_shaders;
	error_shaders.append(VertexShaderCache::GetCurrentShaderCode());
	error_shaders.append(PixelShaderCache::GetCurrentShaderCode());
	char filename[512] = "bad_shader_combo_0.txt";
	int which = 0;
	while (File::Exists(filename))
	{
	which++;
	sprintf(filename, "bad_shader_combo_%i.txt", which);
	}
	File::WriteStringToFile(true, error_shaders, filename);
	PanicAlert("DrawIndexedPrimitiveUP failed. Shaders written to %s", filename);*/
#endif
}

#define CHECK(hr, Message, ...) if (FAILED(hr)) { PanicAlert(__FUNCTION__ "Failed in %s at line %d: " Message, __FILE__, __LINE__, __VA_ARGS__); return;}

void VertexManager::CreateDeviceObjects()
{
	HRESULT hr = S_OK;
	m_buffers_count = 0;
	m_vertex_buffers = NULL;
	m_index_buffers = NULL;
	D3DCAPS9 DeviceCaps = D3D::GetCaps();
	u32 devicevMaxBufferSize =  DeviceCaps.MaxPrimitiveCount * 3 * DeviceCaps.MaxStreamStride;
	//Calculate Device Dependant size
	m_vertex_buffer_size = (VBUFFER_SIZE > devicevMaxBufferSize) ? devicevMaxBufferSize : VBUFFER_SIZE;
	m_index_buffer_size = (IBUFFER_SIZE > DeviceCaps.MaxVertexIndex) ? DeviceCaps.MaxVertexIndex : IBUFFER_SIZE;
	//if device caps are not enough for Vbuffer then fail
	if (m_index_buffer_size < MAXIBUFFERSIZE || m_vertex_buffer_size < MAXVBUFFERSIZE) return;

	m_vertex_buffers = new LPDIRECT3DVERTEXBUFFER9[MAX_VBUFFER_COUNT];
	m_index_buffers = new LPDIRECT3DINDEXBUFFER9[MAX_VBUFFER_COUNT];

	bool Fail = false;
	for (m_current_vertex_buffer = 0; m_current_vertex_buffer < MAX_VBUFFER_COUNT; m_current_vertex_buffer++)
	{
		m_vertex_buffers[m_current_vertex_buffer] = NULL;
		m_index_buffers[m_current_vertex_buffer] = NULL;
	}
	for (m_current_vertex_buffer = 0; m_current_vertex_buffer < MAX_VBUFFER_COUNT; m_current_vertex_buffer++)
	{
		hr = D3D::dev->CreateVertexBuffer( m_vertex_buffer_size,  D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &m_vertex_buffers[m_current_vertex_buffer], NULL );
		CHECK(hr,"Create vertex buffer ", m_current_vertex_buffer);
		hr = D3D::dev->CreateIndexBuffer( m_index_buffer_size * sizeof(u16),  D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_index_buffers[m_current_vertex_buffer], NULL );		
	}
	m_buffers_count = m_current_vertex_buffer;
	m_current_vertex_buffer = 0;
	m_current_index_buffer = 0;
	m_index_buffer_cursor = m_index_buffer_size;
	m_vertex_buffer_cursor = m_vertex_buffer_size;
	m_current_stride = 0;
}

void VertexManager::DestroyDeviceObjects()
{
	D3D::SetStreamSource( 0, NULL, 0, 0);
	D3D::SetIndices(NULL);
	for (int i = 0; i < MAX_VBUFFER_COUNT; i++)
	{
		if(m_vertex_buffers)
		{
			if (m_vertex_buffers[i])
			{
				m_vertex_buffers[i]->Release();
				m_vertex_buffers[i] = NULL;
			}
		}
		if(m_index_buffers)
		{
			if (m_index_buffers[i])
			{
				m_index_buffers[i]->Release();
				m_index_buffers[i] = NULL;
			}
		}
	}
	if(m_vertex_buffers)
		delete [] m_vertex_buffers;
	if(m_index_buffers)
		delete [] m_index_buffers;
	m_vertex_buffers = NULL;
	m_index_buffers = NULL;
}

void VertexManager::PrepareDrawBuffers()
{
	if (!m_buffers_count)
	{
		return;
	}
	u8* p_vertices_base;
	u8* p_vertices;
	u16* p_indices;
	u16* line_indices =  GetLineIndexBuffer();
	u16* point_indices =  GetPointIndexBuffer();
	u16 current_index  = m_num_verts;
	u32 data_size =  m_num_verts * m_stride;
	u32 total_data_size = m_total_num_verts * m_stride;
	if(m_total_index_len)
	{
		DWORD LockMode = D3DLOCK_NOOVERWRITE;
		m_vertex_buffer_cursor--;
		m_vertex_buffer_cursor = m_vertex_buffer_cursor - (m_vertex_buffer_cursor % m_stride) + m_stride;
		if (m_vertex_buffer_cursor > m_vertex_buffer_size - total_data_size)
		{
			LockMode = D3DLOCK_DISCARD;
			m_vertex_buffer_cursor = 0;
			m_current_vertex_buffer = (m_current_vertex_buffer + 1) % m_buffers_count;		
		}	
		if(FAILED(m_vertex_buffers[m_current_vertex_buffer]->Lock(m_vertex_buffer_cursor, total_data_size,(VOID**)(&p_vertices_base), LockMode))) 
		{
			DestroyDeviceObjects();
			return;
		}
		LockMode = D3DLOCK_NOOVERWRITE;
		if (m_index_buffer_cursor > m_index_buffer_size - m_total_index_len)
		{
			LockMode = D3DLOCK_DISCARD;
			m_index_buffer_cursor = 0;
			m_current_index_buffer = (m_current_index_buffer + 1) % m_buffers_count;		
		}	
		if(FAILED(m_index_buffers[m_current_index_buffer]->Lock(m_index_buffer_cursor * sizeof(u16), m_total_index_len * sizeof(u16), (VOID**)(&p_indices), LockMode ))) 
		{
			DestroyDeviceObjects();
			return;
		}
		if(m_triangle_index_len)
		{		
			memcpy(p_indices, GetTriangleIndexBuffer(), m_triangle_index_len * sizeof(u16));
			p_indices += m_triangle_index_len;
		}
		memcpy(p_vertices_base, s_pBaseBufferPointer, data_size);
		p_vertices = p_vertices_base + data_size;
		if (!m_line_emulation_required && m_line_index_len)
		{
			memcpy(p_indices, line_indices, m_line_index_len * sizeof(u16));
			p_indices += m_line_index_len;
		}
		else if(m_line_index_len)
		{
			for (u32 i = 0; i < (m_line_index_len - 1); i += 2)
			{
				// Get Line Indices
				u16 first_index = line_indices[i];
				u16 second_index = line_indices[i + 1];
				// Get the position in the stream o f the first vertex
				u32 currentstride = first_index * m_stride;
				// Get The first vertex Position data
				Float_3* base_vertex_0 =  (Float_3*)(s_pBaseBufferPointer + currentstride);
				// Get The blendindices data
				U8_4* blendindices_vertex_0 =  (U8_4*)(p_vertices_base + currentstride + m_stride - sizeof(U8_4));				
				// Get The first vertex Position data
				currentstride = second_index * m_stride;
				Float_3* base_vertex_1 =  (Float_3*)(s_pBaseBufferPointer + currentstride);				
				U8_4* blendindices_vertex_1 =  (U8_4*)(p_vertices_base + currentstride + m_stride - sizeof(U8_4));

				// Calculate line orientation
				// mostly a hack because we are in object space but is better than nothing
				float dx = base_vertex_1->x - base_vertex_0->x;
				float dy = base_vertex_1->y - base_vertex_0->y;
				bool horizontal = fabs(dx) > fabs(dy);
				bool positive =  horizontal ? dx > 0 : dy > 0;				
				
				// setup offset index acording to line orientation
				u8 idx0 = horizontal ? 
					(positive ? PLO_POS_LINE_NEGATIVE_Y : PLO_POS_LINE_POSITIVE_Y):
					(positive ? PLO_POS_LINE_POSITIVE_X : PLO_POS_LINE_NEGATIVE_X);
				u8 idx1 = horizontal ? 
					(positive ? PLO_POS_LINE_POSITIVE_Y : PLO_POS_LINE_NEGATIVE_Y): 
					(positive ? PLO_POS_LINE_NEGATIVE_X : PLO_POS_LINE_POSITIVE_X);

				memcpy(p_vertices, base_vertex_0, m_stride);
				p_vertices += m_stride;
				U8_4* blendindices_vertex_2 = (U8_4*)(p_vertices - sizeof(U8_4));
				memcpy(p_vertices, base_vertex_1, m_stride);
				p_vertices += m_stride;
				U8_4* blendindices_vertex_3 = (U8_4*)(p_vertices - sizeof(U8_4));
				
				// Setup Blend Indices
				blendindices_vertex_0->y = PLO_TEX_MASK_LINE_0_3;
				blendindices_vertex_0->z = idx0;
				blendindices_vertex_0->w = PLO_ZERO;

				blendindices_vertex_1->y = PLO_TEX_MASK_LINE_0_3;
				blendindices_vertex_1->z = idx0;
				blendindices_vertex_1->w = PLO_ZERO;
				
				blendindices_vertex_2->y = PLO_TEX_MASK_LINE_0_3;
				blendindices_vertex_2->z = idx1;
				blendindices_vertex_2->w = PLO_TEX_LINE;

				blendindices_vertex_3->y = PLO_TEX_MASK_LINE_0_3;
				blendindices_vertex_3->z = idx1;
				blendindices_vertex_3->w = PLO_TEX_LINE;
				

				// Setup new triangle indices
				*p_indices = first_index;
				p_indices++;
				
				*p_indices = current_index;
				current_index++;
				p_indices++;
				
				*p_indices = current_index;
				p_indices++;
				
				*p_indices = current_index;
				current_index++;
				p_indices++;
				
				*p_indices = second_index;
				p_indices++;
				
				*p_indices = first_index;
				p_indices++;
			}
		}
		if (!m_point_emulation_required && m_point_index_len)
		{
			memcpy(p_indices, point_indices, m_point_index_len * sizeof(u16));
		}
		else if(m_point_index_len)
		{
			for (u32 i = 0; i < m_point_index_len; i++)
			{
				// Get point indes
				u16 pointindex = point_indices[i];
				// Calculate stream Position
				int currentstride = pointindex * m_stride;
				// Get data Pointer for vertex replication
				u8* base_vertex =  s_pBaseBufferPointer + currentstride;
				U8_4* blendindices_vertex_0 =  (U8_4*)(p_vertices_base + currentstride + m_stride - sizeof(U8_4));
				
				// Generate Extra vertices
				memcpy(p_vertices, base_vertex, m_stride);
				p_vertices += m_stride;
				U8_4* blendindices_vertex_1 = (U8_4*)(p_vertices - sizeof(U8_4));
				memcpy(p_vertices, base_vertex, m_stride);
				p_vertices += m_stride;
				U8_4* blendindices_vertex_2 = (U8_4*)(p_vertices - sizeof(U8_4));
				memcpy(p_vertices, base_vertex, m_stride);
				p_vertices += m_stride;
				U8_4* blendindices_vertex_3 = (U8_4*)(p_vertices - sizeof(U8_4));

				// Setup Blen Indices
				blendindices_vertex_0->y = PLO_TEX_MASK_POINT_0_3;
				blendindices_vertex_0->z = PLO_POS_POINT_LEFT_TOP;
				blendindices_vertex_0->w = PLO_ZERO;

				blendindices_vertex_1->y = PLO_TEX_MASK_POINT_0_3;
				blendindices_vertex_1->z = PLO_POS_POINT_LEFT_BOTTOM;
				blendindices_vertex_1->w = PLO_TEX_POINT_X;
				
				blendindices_vertex_2->y = PLO_TEX_MASK_POINT_0_3;
				blendindices_vertex_2->z = PLO_POS_POINT_RIGHT_TOP;
				blendindices_vertex_2->w = PLO_TEX_POINT_Y;

				blendindices_vertex_3->y = PLO_TEX_MASK_POINT_0_3;
				blendindices_vertex_3->z = PLO_POS_POINT_RIGHT_BOTTOM;
				blendindices_vertex_3->w = PLO_TEX_POINT_XY;

				// Setup new triangle indices
				*p_indices = pointindex; // Left Top
				p_indices++;
				
				*p_indices = current_index; // Left Bottom
				current_index++;
				p_indices++;
				
				*p_indices = current_index; // Right Top
				p_indices++;
				
				*p_indices = current_index; // Right Top
				p_indices++;
				
				*p_indices = current_index - 1; // Left Bottom
				p_indices++;
				current_index++;
				*p_indices = current_index; // Right Bottom
				p_indices++;
				current_index++;
			}
		}
		m_vertex_buffers[m_current_vertex_buffer]->Unlock();
		m_index_buffers[m_current_index_buffer]->Unlock();
	}
	if(m_current_stride != m_stride || m_vertex_buffer_cursor == 0)
	{
		m_current_stride = m_stride;
		D3D::SetStreamSource( 0, m_vertex_buffers[m_current_vertex_buffer], 0, m_current_stride);
	}
	if (m_index_buffer_cursor == 0)
	{
		D3D::SetIndices(m_index_buffers[m_current_index_buffer]);
	}

	ADDSTAT(stats.thisFrame.bytesVertexStreamed, total_data_size);
	ADDSTAT(stats.thisFrame.bytesIndexStreamed, m_total_index_len);
}

void VertexManager::Draw()
{
	int StartIndex = m_index_buffer_cursor;
	int basevertex = m_vertex_buffer_cursor / m_stride;
	if (m_triangles > 0)
	{
		if (FAILED(D3D::dev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			basevertex,
			0, 
			m_total_num_verts,
			StartIndex, 
			m_triangles )))
		{
			DumpBadShaders();
		}
		StartIndex += IndexGenerator::GetTriangleindexLen();
		INCSTAT(stats.thisFrame.numIndexedDrawCalls);
	}
	if (m_lines > 0)
	{
		if (FAILED(D3D::dev->DrawIndexedPrimitive(
			D3DPT_LINELIST, 
			basevertex,
			0, 
			m_total_num_verts,
			StartIndex, 
			m_lines)))
		{
			DumpBadShaders();
		}
		StartIndex += m_line_index_len;
		INCSTAT(stats.thisFrame.numIndexedDrawCalls);
	}
	if (m_points > 0)
	{
		//DrawIndexedPrimitive does not support point list so we have to draw them using DrawPrimitive
		if (m_triangles == 0 && m_lines == 0)
		{
			// we can draw the entire buffer in a single call because we have points only
			if (FAILED(D3D::dev->DrawPrimitive(
					D3DPT_POINTLIST, 
					basevertex,
					m_points)))
				{
					DumpBadShaders();
				}
				INCSTAT(stats.thisFrame.numDrawCalls);
		}
		else
		{
			// Try to merge points to reduce draw calls
			u16* PointIndexBuffer = GetPointIndexBuffer();
			u32 i  = 0;        
			do
			{
				u32 count = i + 1;
				while (count < m_points && PointIndexBuffer[count - 1] + 1 == PointIndexBuffer[count])
				{
					count++;
				}
				if (FAILED(D3D::dev->DrawPrimitive(
					D3DPT_POINTLIST, 
					basevertex + PointIndexBuffer[i],
					count - i)))
				{
					DumpBadShaders();
				}
				INCSTAT(stats.thisFrame.numDrawCalls);
				i = count;
			} while (i < m_points);
		}
	}
}


void VertexManager::SetPLRasterOffsets()
{
	// calculate base values for the offset parameters
	float vpWidth = 2.0f * xfregs.viewport.wd;
	float vpHeight = -2.0f * xfregs.viewport.ht;
	vpWidth = vpWidth > 0 ? vpWidth : 640.f;
	vpHeight = vpHeight > 0 ? vpHeight : 480.f;
	vpWidth = 1.0f / vpWidth;
	vpHeight = 1.0f / vpHeight;
	float psize = float(bpmem.lineptwidth.pointsize) / 6.0f;
	float psizex = psize * vpWidth;
	float psizey = psize * vpHeight;
	float ptxoff = LINE_PT_TEX_OFFSETS[bpmem.lineptwidth.pointoff];
	float lsize = float(bpmem.lineptwidth.linesize) / 6.0f;
	float lsizex = lsize * vpWidth;
	float lsizey = lsize * vpHeight;
	float ltxoff = LINE_PT_TEX_OFFSETS[bpmem.lineptwidth.lineoff];
	float* offsets_pointer = (float*)s_line_pt_params;
	// Fill texture offset Mask
	for (int i = 0; i < 8; i++)
	{
		offsets_pointer[i + 4] = bpmem.texcoords[i].s.point_offset ? 1.0f : 0.0f;
		offsets_pointer[i + 12] = bpmem.texcoords[i].s.line_offset ? 1.0f : 0.0f;
	}
	s_line_pt_params[PLO_POS_POINT_LEFT_TOP] = Float_4(-psizex,  psizey, ptxoff, 0.f);
	s_line_pt_params[PLO_POS_POINT_LEFT_BOTTOM] = Float_4(-psizex, -psizey, 0.f, ptxoff);
	s_line_pt_params[PLO_POS_POINT_RIGHT_TOP] = Float_4(psizex,  psizey, ptxoff, ptxoff);
	s_line_pt_params[PLO_POS_POINT_RIGHT_BOTTOM] = Float_4(psizex, -psizey, ltxoff, 0.f);
	s_line_pt_params[PLO_POS_LINE_NEGATIVE_X].x = -lsizex;
	s_line_pt_params[PLO_POS_LINE_NEGATIVE_Y].y = -lsizey;
	s_line_pt_params[PLO_POS_LINE_POSITIVE_X].x = lsizex;
	s_line_pt_params[PLO_POS_LINE_POSITIVE_Y].y = lsizey;

	g_renderer->SetMultiVSConstant4fv(C_PLOFFSETPARAMS, LINE_PT_OFFSETS_PARAMS_LEN, offsets_pointer);
	
}

void VertexManager::vFlush()
{
	// initialize all values for the current flush
	u32 available_index = 65535;
	m_triangle_index_len = IndexGenerator::GetTriangleindexLen();
	m_line_index_len = IndexGenerator::GetLineindexLen();
	m_point_index_len = IndexGenerator::GetPointindexLen();
	m_num_verts = IndexGenerator::GetNumVerts();
	m_triangles = IndexGenerator::GetNumTriangles();
	m_lines = IndexGenerator::GetNumLines();
	m_points = IndexGenerator::GetNumPoints();
	m_stride = g_nativeVertexFmt->GetVertexStride();
	m_point_emulation_required = false;
	m_line_emulation_required = false;
	// Initialize totals using the triangle values as a base
	m_total_index_len = m_triangle_index_len;
	m_total_num_verts = m_num_verts;
	// As we can potentialy generate more indices than what is supported
	// chek for the index lenght. 
	available_index -= m_total_index_len;
	if (m_line_index_len)
	{
		// we can only skip line emulation if the lines have 1 pixel size and no texturing
		float fratio = Renderer::EFBToScaledXf(1.f);
		float lsize = bpmem.lineptwidth.linesize * fratio / 6.0f;
		m_line_emulation_required = m_line_emulation_required || lsize > 1.0f;
	}

	u32 usedtextures = 0;
	for (u32 i = 0; i < (u32)bpmem.genMode.numtevstages + 1; ++i)
		if (bpmem.tevorders[i / 2].getEnable(i & 1))
			usedtextures |= 1 << bpmem.tevorders[i/2].getTexMap(i & 1);

	if (bpmem.genMode.numindstages > 0)
		for (u32 i = 0; i < bpmem.genMode.numtevstages + 1; ++i)
			if (bpmem.tevind[i].IsActive() && bpmem.tevind[i].bt < bpmem.genMode.numindstages)
				usedtextures |= 1 << bpmem.tevindref.getTexMap(bpmem.tevind[i].bt);

	for (u32 i = 0; i < 8; ++i)
	{
		if (usedtextures & (1 << i))
		{
			// check if we use textures in points or lines to enable emulation
			// we only need to enable emulation for points if the offset is greater than zero
			if (bpmem.lineptwidth.pointoff)
			{
				m_point_emulation_required = m_point_emulation_required || (bpmem.texcoords[i].s.point_offset != 0);
			}
			// any textured lines need emulation
			m_line_emulation_required = m_line_emulation_required || (bpmem.texcoords[i].s.line_offset != 0);
			u32 stage = i & 3;
			u32 texindex = i >> 2;
			g_renderer->SetSamplerState(stage, texindex);
			FourTexUnits &tex = bpmem.tex[texindex];
			bool from_tmem = tex.texImage1[stage].image_type != 0;
			TextureCache::TCacheEntryBase* tentry = TextureCache::Load(i, 
				(tex.texImage3[stage].image_base/* & 0x1FFFFF*/) << 5,
				tex.texImage0[stage].width + 1, tex.texImage0[stage].height + 1,
				tex.texImage0[stage].format, tex.texTlut[stage].tmem_offset<<9, 
				tex.texTlut[stage].tlut_format,
				(tex.texMode0[stage].min_filter & 3),
				(tex.texMode1[stage].max_lod + 0xf) / 0x10,
				from_tmem);

			if (tentry)
			{
				// 0s are probably for no manual wrapping needed.
				PixelShaderManager::SetTexDims(i, tentry->native_width, tentry->native_height, 0, 0);
			}
			else
				ERROR_LOG(VIDEO, "Error loading texture");
		}
	}
	if (m_line_index_len)
	{
		if (m_line_emulation_required)
		{
			// We need line emulation so transform the values to allow lines to triangle translation
			// if we generate more indices than what is supported discard the rest ( i know is not the best solution)
			m_line_index_len = m_line_index_len * 3 > available_index ? available_index / 3 : m_line_index_len;
			// we will generate 2 extra vertex for each line (the same amount of original indices)
			m_total_num_verts += m_line_index_len;
			// for each line, 6 indices will be generated ( 3 times the amount of original indices )
			m_total_index_len += m_line_index_len * 3;
			// reduce the amount of available indices
			available_index -= m_line_index_len * 3;
			// Generating 2 triangles for each line
			m_triangles += m_lines * 2;
			// Reset lines as we are going to emulate them
			m_lines = 0;
		}
		else
		{
			// Render lines normally
			m_total_index_len += m_line_index_len;
		}
	}
	
	if (m_point_index_len)
	{
		if (m_point_emulation_required)
		{
			// We need point emulation so setup values to allow point to triangle translation
			// if we generate more indices than what is supported discard the rest
			m_point_index_len = m_point_index_len * 6 > available_index ? available_index / 6 : m_point_index_len;
			// 3 extra vertices will be generated for each point
			m_total_num_verts += m_point_index_len * 3;
			// 6 indices for each point
			m_total_index_len += m_point_index_len * 6;
			// 2 triangles
			m_triangles += m_points * 2;
			// Reset point to emulate them properly
			m_points = 0;
		}
		else
		{
			// Render points normally
			m_total_index_len += m_point_index_len;
		}
	}
	

	// set global constants
	VertexShaderManager::SetConstants();
	if ((m_line_index_len && m_line_emulation_required) || (m_point_index_len && m_point_emulation_required))
	{
		// if we use emulation setup the offsets for the vertex shaders
		SetPLRasterOffsets();
	}	
	PixelShaderManager::SetConstants(g_nativeVertexFmt->m_components);	
	bool useDstAlpha = !g_ActiveConfig.bDstAlphaPass && bpmem.dstalpha.enable && bpmem.blendmode.alphaupdate &&
		bpmem.zcontrol.pixel_format == PIXELFMT_RGBA6_Z24;
	bool useDualSource = useDstAlpha && g_ActiveConfig.backend_info.bSupportsDualSourceBlend;
	DSTALPHA_MODE AlphaMode = useDualSource ? DSTALPHA_DUAL_SOURCE_BLEND : DSTALPHA_NONE;	

	if (!PixelShaderCache::SetShader(AlphaMode ,g_nativeVertexFmt->m_components))
	{
		GFX_DEBUGGER_PAUSE_LOG_AT(NEXT_ERROR,true,{printf("Fail to set pixel shader\n");});
		goto shader_fail;
	}
	if (!VertexShaderCache::SetShader(g_nativeVertexFmt->m_components))
	{
		GFX_DEBUGGER_PAUSE_LOG_AT(NEXT_ERROR,true,{printf("Fail to set vertex shader\n");});
		goto shader_fail;

	}
	PrepareDrawBuffers();
	g_nativeVertexFmt->SetupVertexPointers();
	g_perf_query->EnableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);
	Draw();
	g_perf_query->DisableQuery(bpmem.zcontrol.early_ztest ? PQG_ZCOMP_ZCOMPLOC : PQG_ZCOMP);
	if (useDstAlpha && !useDualSource)
	{
		if (!PixelShaderCache::SetShader(DSTALPHA_ALPHA_PASS, g_nativeVertexFmt->m_components))
		{
			GFX_DEBUGGER_PAUSE_LOG_AT(NEXT_ERROR,true,{printf("Fail to set pixel shader\n");});
			goto shader_fail;
		}
		// update alpha only
		g_renderer->ApplyState(true);
		Draw();		
		g_renderer->RestoreState();
	}
	GFX_DEBUGGER_PAUSE_AT(NEXT_FLUSH, true);

shader_fail:
	m_index_buffer_cursor += m_total_index_len;
	m_vertex_buffer_cursor += (m_total_num_verts) * m_stride;	
}

}
