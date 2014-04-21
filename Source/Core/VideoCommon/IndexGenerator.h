// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

// This is currently only used by the DX backend, but it may make sense to
// use it in the GL backend or a future DX10 backend too.

#ifndef _INDEXGENERATOR_H
#define _INDEXGENERATOR_H
#include "Common/CommonTypes.h"

class IndexGenerator
{
public:
	// Init
	static void Init();
	static void Start(u16 *Triangleptr,u16 *Lineptr,u16 *Pointptr);

	static void AddIndices(int primitive, u32 numVertices);

	// Interface
	static u32 GetNumTriangles() {return numT;}
	static u32 GetNumLines() {return numL;}
	static u32 GetNumPoints() {return numP;}

	// returns numprimitives
	static u32 GetNumVerts() {return index;}

	static u32 GetTriangleindexLen() {return (u32)(s_Triangle_Current - s_Triangle_Base);}
	static u32 GetLineindexLen() {return (u32)(s_Line_Current - s_Line_Base);}
	static u32 GetPointindexLen() {return (u32)(s_Point_Current - s_Point_Base);}
	
	static u32 GetRemainingIndices();
/*
	enum IndexPrimitiveType
	{
		Prim_None = 0,
		Prim_List,
		Prim_Strip,
		Prim_Fan
	};
*/
private:
	// Triangles
	template <bool pr> static void AddList(u32 numVerts);
	template <bool pr> static void AddStrip(u32 numVerts);
	template <bool pr> static void AddFan(u32 numVerts);
	template <bool pr> static void AddQuads(u32 numVerts);

	// Lines
	static void AddLineList(u32 numVerts);
	static void AddLineStrip(u32 numVerts);

	// Points
	static void AddPoints(u32 numVerts);

	template <bool pr> static u16* WriteTriangle(u16 *ptr, u32 index1, u32 index2, u32 index3);

	static u16 *s_Triangle_Current;
	static u16 *s_Triangle_Base;
	static u16 *s_Line_Current;
	static u16 *s_Line_Base;
	static u16 *s_Point_Current;
	static u16 *s_Point_Base;
	// TODO: redundant variables
	static u32 numT;
	static u32 numL;
	static u32 numP;
	static u32 index;
};

#endif  // _INDEXGENERATOR_H
