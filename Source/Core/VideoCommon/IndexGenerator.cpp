// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cstddef>

#include "Common/Common.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/IndexGenerator.h"

//Init
u16 *IndexGenerator::s_Triangle_Current;
u16 *IndexGenerator::s_Triangle_Base;
u16 *IndexGenerator::s_Line_Current;
u16 *IndexGenerator::s_Line_Base;
u16 *IndexGenerator::s_Point_Current;
u16 *IndexGenerator::s_Point_Base;
u32 IndexGenerator::numT;
u32 IndexGenerator::numL;
u32 IndexGenerator::numP;
u32 IndexGenerator::index;

static const u16 s_primitive_restart = -1;

static void (*primitive_table[8])(u32);

void IndexGenerator::Init()
{
	if(g_Config.backend_info.bSupportsPrimitiveRestart)
	{
		primitive_table[0] = IndexGenerator::AddQuads<true>;
		primitive_table[2] = IndexGenerator::AddList<true>;
		primitive_table[3] = IndexGenerator::AddStrip<true>;
		primitive_table[4] = IndexGenerator::AddFan<true>;
	}
	else
	{
		primitive_table[0] = IndexGenerator::AddQuads<false>;
		primitive_table[2] = IndexGenerator::AddList<false>;
		primitive_table[3] = IndexGenerator::AddStrip<false>;
		primitive_table[4] = IndexGenerator::AddFan<false>;
	}
	primitive_table[1] = NULL;
	primitive_table[5] = &IndexGenerator::AddLineList;
	primitive_table[6] = &IndexGenerator::AddLineStrip;
	primitive_table[7] = &IndexGenerator::AddPoints;
}

void IndexGenerator::Start(u16* Triangleptr, u16* Lineptr, u16* Pointptr)
{
	s_Triangle_Current = Triangleptr;
	s_Line_Current = Lineptr;
	s_Point_Current = Pointptr;
	s_Triangle_Base = Triangleptr;
	s_Line_Base = Lineptr;
	s_Point_Base = Pointptr;
	index = 0;
	numT = 0;
	numL = 0;
	numP = 0;
}

void IndexGenerator::AddIndices(int primitive, u32 numVerts)
{
	primitive_table[primitive](numVerts);
	index += numVerts;
}

// Triangles
template <bool pr> __forceinline u16* IndexGenerator::WriteTriangle(u16* ptr, u32 index1, u32 index2, u32 index3)
{
	*ptr++ = index1;
	*ptr++ = index2;
	*ptr++ = index3;
	if(pr)
		*ptr++ = s_primitive_restart;	
	++numT;
	return ptr;
}

template <bool pr> void IndexGenerator::AddList(u32 const numVerts)
{
	u32 i = index + 2;
	u32 top = (index + numVerts);
	u16* ptr = s_Triangle_Current;
	while (i < top)
	{
		ptr = WriteTriangle<pr>(ptr, i - 2, i - 1, i);
		i += 3;
	}
	s_Triangle_Current = ptr;
}

template <bool pr> void IndexGenerator::AddStrip(u32 const numVerts)
{
	u16* ptr = s_Triangle_Current;
	u32 top = (index + numVerts);
	if(pr)
	{
		for (u32 i = index; i < top; ++i)
		{
			*ptr++ = i;
		}
		*ptr++ = s_primitive_restart;
		numT += numVerts - 2;
	}
	else
	{
		u32 i = index + 2;		
		bool wind = false;
		while (i < top)
		{
			ptr = WriteTriangle<pr>(
				ptr,
				i - 2,
				i - !wind,
				i - wind);
			wind ^= true;
			++i;
		}
	}
	s_Triangle_Current = ptr;
}

/**
 * FAN simulator:
 * 
 *   2---3
 *  / \ / \
 * 1---0---4
 * 
 * would generate this triangles:
 * 012, 023, 034
 * 
 * rotated (for better striping):
 * 120, 302, 034
 * 
 * as odd ones have to winded, following strip is fine:
 * 12034
 * 
 * so we use 6 indices for 3 triangles
 */

template <bool pr> void IndexGenerator::AddFan(u32 numVerts)
{
	u32 i = index + 2;
	u32 top = (index + numVerts);
	u16* ptr = s_Triangle_Current;
	if(pr)
	{
		while (i + 3 <= top)
		{
			*ptr++ = i - 1;
			*ptr++ = i + 0;
			*ptr++ = index;
			*ptr++ = i + 1;
			*ptr++ = i + 2;
			*ptr++ = s_primitive_restart;
			numT += 3;
			i += 3;
		}
		
		while (i + 2 <= top)
		{
			*ptr++ = i - 1;
			*ptr++ = i + 0;
			*ptr++ = index;
			*ptr++ = i + 1;
			*ptr++ = s_primitive_restart;
			numT += 2;
			i += 2;
		}
	}

	while (i < top)
	{
		ptr = WriteTriangle<pr>(ptr, index, i - 1, i);
		++i;
	}
	s_Triangle_Current = ptr;
}

/*
 * QUAD simulator
 * 
 * 0---1   4---5
 * |\  |   |\  |
 * | \ |   | \ |
 * |  \|   |  \|
 * 3---2   7---6
 * 
 * 012,023, 456,467 ...
 * or 120,302, 564,746
 * or as strip: 1203, 5647
 * 
 * Warning: 
 * A simple triangle has to be rendered for three vertices.
 * ZWW do this for sun rays
 */
template <bool pr> void IndexGenerator::AddQuads(u32 numVerts)
{
	u32 i = index + 3;
	u32 top = (index + numVerts);
	u16* ptr = s_Triangle_Current;
	while (i < top)
	{
		if(pr)
		{
			*ptr++ = i - 2;
			*ptr++ = i - 1;
			*ptr++ = i - 3;
			*ptr++ = i - 0;
			*ptr++ = s_primitive_restart;
			numT += 2;
		}
		else
		{
			ptr = WriteTriangle<pr>(ptr, i - 3, i - 2, i - 1);
			ptr = WriteTriangle<pr>(ptr, i - 3, i - 1, i - 0);
		}
		i += 4;
	}

	// three vertices remaining, so render a triangle
	if(i == top)
	{
		ptr = WriteTriangle<pr>(ptr, top - 3, top - 2, top - 1);
	}
	s_Triangle_Current = ptr;
}

// Lines
void IndexGenerator::AddLineList(u32 numVerts)
{
	u32 i = index + 1;
	u32 top = (index + numVerts);
	u16* ptr = s_Line_Current;
	while (i < top)
	{
		*ptr++ = i - 1;
		*ptr++ = i;
		i += 2;
	}
	numL += (numVerts >> 1);
	s_Line_Current = ptr;
}

// shouldn't be used as strips as LineLists are much more common
// so converting them to lists
void IndexGenerator::AddLineStrip(u32 numVerts)
{
	u32 i = index + 1;
	u32 top = (index + numVerts);
	u16* ptr = s_Line_Current;
	while (i < top)
	{
		*ptr++ = i - 1;
		*ptr++ = i;
		++i;
	}
	numL += numVerts - 1;
	s_Line_Current = ptr;
}

// Points
void IndexGenerator::AddPoints(u32 numVerts)
{
	u32 i = index;
	u32 top = (index + numVerts);
	u16 *ptr = s_Point_Current;
	while (i < top)
	{
		*ptr++ = i;
		++i;
	}
	numP += numVerts;
	s_Point_Current = ptr;
}


u32 IndexGenerator::GetRemainingIndices()
{
	u32 max_index = 65534; // -1 is reserved for primitive restart (ogl + dx11)
	return max_index - index;
}
