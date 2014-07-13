// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#include "VideoCommon/VertexLoader_BBox.h"
#include "VideoCommon/PixelEngine.h"
#include "VideoCommon/XFMemory.h"
// bbox variables
// bbox must read vertex position, so convert it to this buffer
static float s_bbox_vertex_buffer[3];
static u8 *s_bbox_pCurBufferPointer_orig;
static int s_bbox_primitive;
static struct Point
{
	s32 x;
	s32 y;
	float z;
} s_bbox_points[3];
static u8 s_bbox_currPoint;
static u8 s_bbox_loadedPoints;
static const u8 s_bbox_primitivePoints[8] = { 3, 0, 3, 3, 3, 2, 2, 1 };

void LOADERDECL VertexLoader_BBox::UpdateBoundingBoxPrepare()
{
	if (!PixelEngine::bbox_active)
		return;

	// set our buffer as videodata buffer, so we will get a copy of the vertex positions
	// this is a big hack, but so we can use the same converting function then without bbox
	s_bbox_pCurBufferPointer_orig = g_PipelineState.GetWritePosition();
	g_PipelineState.SetWritePosition((u8*)s_bbox_vertex_buffer);
}

inline bool UpdateBoundingBoxVars()
{
	switch (s_bbox_primitive)
	{
		// Quads: fill 0,1,2 (check),1 (check, clear, repeat)
	case 0:
		++s_bbox_loadedPoints;
		if (s_bbox_loadedPoints == 3)
		{
			s_bbox_currPoint = 1;
			return true;
		}
		if (s_bbox_loadedPoints == 4)
		{
			s_bbox_loadedPoints = 0;
			s_bbox_currPoint = 0;
			return true;
		}
		++s_bbox_currPoint;
		return false;

		// Triangles: 0,1,2 (check, clear, repeat)
	case 2:
		++s_bbox_loadedPoints;
		if (s_bbox_loadedPoints == 3)
		{
			s_bbox_loadedPoints = 0;
			s_bbox_currPoint = 0;
			return true;
		}
		++s_bbox_currPoint;
		return false;

		// Triangle strip: 0, 1, 2 (check), 0 (check), 1, (check), 2 (check, repeat checking 0, 1, 2)
	case 3:
		if (++s_bbox_currPoint == 3)
			s_bbox_currPoint = 0;

		if (s_bbox_loadedPoints == 2)
			return true;

		++s_bbox_loadedPoints;
		return false;

		// Triangle fan: 0,1,2 (check), 1 (check), 2 (check, repeat checking 1,2)
	case 4:
		s_bbox_currPoint ^= s_bbox_currPoint ? 3 : 1;

		if (s_bbox_loadedPoints == 2)
			return true;

		++s_bbox_loadedPoints;
		return false;

		// Lines: 0,1 (check, clear, repeat)
	case 5:
		++s_bbox_loadedPoints;
		if (s_bbox_loadedPoints == 2)
		{
			s_bbox_loadedPoints = 0;
			s_bbox_currPoint = 0;
			return true;
		}
		++s_bbox_currPoint;
		return false;

		// Line strip: 0,1 (check), 0 (check), 1 (check, repeat checking 0,1)
	case 6:
		s_bbox_currPoint ^= 1;

		if (s_bbox_loadedPoints == 1)
			return true;

		++s_bbox_loadedPoints;
		return false;

		// Points: 0 (check, clear, repeat)
	case 7:
		return true;

		// This should not happen!
	default:
		return false;
	}
}

void LOADERDECL VertexLoader_BBox::UpdateBoundingBox()
{
	if (!PixelEngine::bbox_active)
		return;

	// Reset videodata pointer
	g_PipelineState.SetWritePosition(s_bbox_pCurBufferPointer_orig);

	// Copy vertex pointers
	memcpy(g_PipelineState.GetWritePosition(), s_bbox_vertex_buffer, 12);
	g_PipelineState.WriteSkip(12);

	// We must transform the just loaded point by the current world and projection matrix - in software
	float transformed[3];
	float screenPoint[3];

	// We need to get the raw projection values for the bounding box calculation
	// to work properly. That means, no projection hacks!
	const float * const orig_point = s_bbox_vertex_buffer;
	const float * const world_matrix = (float*)xfmem + g_PipelineState.curposmtx * 4;
	const float * const proj_matrix = xfregs.projection.rawProjection;

	// Transform by world matrix
	// Only calculate what we need, discard the rest
	transformed[0] = orig_point[0] * world_matrix[0] + orig_point[1] * world_matrix[1] + orig_point[2] * world_matrix[2] + world_matrix[3];
	transformed[1] = orig_point[0] * world_matrix[4] + orig_point[1] * world_matrix[5] + orig_point[2] * world_matrix[6] + world_matrix[7];

	// Transform by projection matrix
	switch (xfregs.projection.type)
	{
		// Perspective projection, we must divide by w
	case GX_PERSPECTIVE:
		transformed[2] = orig_point[0] * world_matrix[8] + orig_point[1] * world_matrix[9] + orig_point[2] * world_matrix[10] + world_matrix[11];
		screenPoint[0] = (transformed[0] * proj_matrix[0] + transformed[2] * proj_matrix[1]) / (-transformed[2]);
		screenPoint[1] = (transformed[1] * proj_matrix[2] + transformed[2] * proj_matrix[3]) / (-transformed[2]);
		screenPoint[2] = ((transformed[2] * proj_matrix[4] + proj_matrix[5]) * (1.0f - (float) 1e-7)) / (-transformed[2]);
		break;

		// Orthographic projection
	case GX_ORTHOGRAPHIC:
		screenPoint[0] = transformed[0] * proj_matrix[0] + proj_matrix[1];
		screenPoint[1] = transformed[1] * proj_matrix[2] + proj_matrix[3];

		// We don't really have to care about z here
		screenPoint[2] = -0.2f;
		break;

	default:
		ERROR_LOG(VIDEO, "Unknown projection type: %d", xfregs.projection.type);
		screenPoint[0] = screenPoint[1] = screenPoint[2] = 1;
	}

	// Convert to screen space and add the point to the list - round like the real hardware
	s_bbox_points[s_bbox_currPoint].x = (((s32)(0.5f + (16.0f * (screenPoint[0] * xfregs.viewport.wd + (xfregs.viewport.xOrig - 342.0f))))) + 3) >> 4;
	s_bbox_points[s_bbox_currPoint].y = (((s32)(0.5f + (16.0f * (screenPoint[1] * xfregs.viewport.ht + (xfregs.viewport.yOrig - 342.0f))))) + 3) >> 4;
	s_bbox_points[s_bbox_currPoint].z = screenPoint[2];

	// Update point list for primitive
	bool check_bbox = UpdateBoundingBoxVars();

	// If we do not have enough points to check the bounding box yet, we are done for now
	if (!check_bbox)
		return;

	// How many points does our primitive have?
	const u8 numPoints = s_bbox_primitivePoints[s_bbox_primitive];

	// If the primitive is a point, update the bounding box now
	if (numPoints == 1)
	{
		Point & p = s_bbox_points[0];

		// Point is out of bounds
		if (p.x < 0 || p.x > 607 || p.y < 0 || p.y > 479 || p.z >= 0.0f)
			return;

		// Point is in bounds. Update bounding box if necessary and return
		PixelEngine::bbox[0] = (p.x < PixelEngine::bbox[0]) ? p.x : PixelEngine::bbox[0];
		PixelEngine::bbox[1] = (p.x > PixelEngine::bbox[1]) ? p.x : PixelEngine::bbox[1];
		PixelEngine::bbox[2] = (p.y < PixelEngine::bbox[2]) ? p.y : PixelEngine::bbox[2];
		PixelEngine::bbox[3] = (p.y > PixelEngine::bbox[3]) ? p.y : PixelEngine::bbox[3];

		return;
	}

	// Now comes the fun part. We must clip the triangles/lines to the viewport - also in software
	Point & p0 = s_bbox_points[0], &p1 = s_bbox_points[1], &p2 = s_bbox_points[2];

	// Check for z-clip. This crude method is required for Mickey's Magical Mirror, at least
	if ((p0.z > 0.0f) || (p1.z > 0.0f) || ((numPoints == 3) && (p2.z > 0.0f)))
		return;

	// Check points for bounds
	u8 b0 = ((p0.x > 0) ? 1 : 0) | ((p0.y > 0) ? 2 : 0) | ((p0.x > 607) ? 4 : 0) | ((p0.y > 479) ? 8 : 0);
	u8 b1 = ((p1.x > 0) ? 1 : 0) | ((p1.y > 0) ? 2 : 0) | ((p1.x > 607) ? 4 : 0) | ((p1.y > 479) ? 8 : 0);

	// Let's be practical... If we only have a line, setting b2 to 3 saves an "if"-clause later on
	u8 b2 = 3;

	// Otherwise if we have a triangle, we need to check the third point
	if (numPoints == 3)
		b2 = ((p2.x > 0) ? 1 : 0) | ((p2.y > 0) ? 2 : 0) | ((p2.x > 607) ? 4 : 0) | ((p2.y > 479) ? 8 : 0);

	// These are the internal bbox vars
	s32 left = 608, right = -1, top = 480, bottom = -1;

	// If the polygon is inside viewport, let's update the bounding box and be done with it
	if ((b0 == 3) && (b0 == b1) && (b0 == b2))
	{
		// Line
		if (numPoints == 2)
		{
			left = (p0.x < p1.x) ? p0.x : p1.x;
			top = (p0.y < p1.y) ? p0.y : p1.y;
			right = (p0.x > p1.x) ? p0.x : p1.x;
			bottom = (p0.y > p1.y) ? p0.y : p1.y;
		}

		// Triangle
		else
		{
			left = (p0.x < p1.x) ? (p0.x < p2.x) ? p0.x : p2.x : (p1.x < p2.x) ? p1.x : p2.x;
			top = (p0.y < p1.y) ? (p0.y < p2.y) ? p0.y : p2.y : (p1.y < p2.y) ? p1.y : p2.y;
			right = (p0.x > p1.x) ? (p0.x > p2.x) ? p0.x : p2.x : (p1.x > p2.x) ? p1.x : p2.x;
			bottom = (p0.y > p1.y) ? (p0.y > p2.y) ? p0.y : p2.y : (p1.y > p2.y) ? p1.y : p2.y;
		}

		// Update bounding box
		PixelEngine::bbox[0] = (left   < PixelEngine::bbox[0]) ? left : PixelEngine::bbox[0];
		PixelEngine::bbox[1] = (right  > PixelEngine::bbox[1]) ? right : PixelEngine::bbox[1];
		PixelEngine::bbox[2] = (top    < PixelEngine::bbox[2]) ? top : PixelEngine::bbox[2];
		PixelEngine::bbox[3] = (bottom > PixelEngine::bbox[3]) ? bottom : PixelEngine::bbox[3];

		return;
	}

	// If it is not inside, then either it is completely outside, or it needs clipping.
	// Check the primitive's lines
	u8 i0 = b0 ^ b1;
	u8 i1 = (numPoints == 3) ? (b1 ^ b2) : i0;
	u8 i2 = (numPoints == 3) ? (b0 ^ b2) : i0;

	// Primitive out of bounds - return
	if (!(i0 | i1 | i2))
		return;

	// First point inside viewport - update internal bbox
	if (b0 == 3)
	{
		left = p0.x;
		top = p0.y;
		right = p0.x;
		bottom = p0.y;
	}

	// Second point inside
	if (b1 == 3)
	{
		left = (p1.x < left) ? p1.x : left;
		top = (p1.y < top) ? p1.y : top;
		right = (p1.x > right) ? p1.x : right;
		bottom = (p1.y > bottom) ? p1.y : bottom;
	}

	// Third point inside
	if ((b2 == 3) && (numPoints == 3))
	{
		left = (p2.x < left) ? p2.x : left;
		top = (p2.y < top) ? p2.y : top;
		right = (p2.x > right) ? p2.x : right;
		bottom = (p2.y > bottom) ? p2.y : bottom;
	}

	// Triangle equation vars
	float m, c;

	// Some definitions to help with rounding later on
	const float highNum = 89374289734.0f;
	const float roundUp = 0.001f;

	// Intersection result
	s32 s;

	// First line intersects
	if (i0)
	{
		m = (p1.x - p0.x) ? ((p1.y - p0.y) / (p1.x - p0.x)) : highNum;
		c = p0.y - (m * p0.x);
		if (i0 & 1) { s = (s32)(c + roundUp); if (s >= 0 && s <= 479) left = 0;   top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
		if (i0 & 2) { s = (s32)((-c / m) + roundUp); if (s >= 0 && s <= 607) top = 0;   left = (s < left) ? s : left; right = (s > right) ? s : right; }
		if (i0 & 4) { s = (s32)((m * 607) + c + roundUp); if (s >= 0 && s <= 479) right = 607; top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
		if (i0 & 8) { s = (s32)(((479 - c) / m) + roundUp); if (s >= 0 && s <= 607) bottom = 479; left = (s < left) ? s : left; right = (s > right) ? s : right; }
	}

	// Only check other lines if we are dealing with a triangle
	if (numPoints == 3)
	{
		// Second line intersects
		if (i1)
		{
			m = (p2.x - p1.x) ? ((p2.y - p1.y) / (p2.x - p1.x)) : highNum;
			c = p1.y - (m * p1.x);
			if (i1 & 1) { s = (s32)(c + roundUp); if (s >= 0 && s <= 479) left = 0;   top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
			if (i1 & 2) { s = (s32)((-c / m) + roundUp); if (s >= 0 && s <= 607) top = 0;   left = (s < left) ? s : left; right = (s > right) ? s : right; }
			if (i1 & 4) { s = (s32)((m * 607) + c + roundUp); if (s >= 0 && s <= 479) right = 607; top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
			if (i1 & 8) { s = (s32)(((479 - c) / m) + roundUp); if (s >= 0 && s <= 607) bottom = 479; left = (s < left) ? s : left; right = (s > right) ? s : right; }
		}

		// Third line intersects
		if (i2)
		{
			m = (p2.x - p0.x) ? ((p2.y - p0.y) / (p2.x - p0.x)) : highNum;
			c = p0.y - (m * p0.x);
			if (i2 & 1) { s = (s32)(c + roundUp); if (s >= 0 && s <= 479) left = 0;   top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
			if (i2 & 2) { s = (s32)((-c / m) + roundUp); if (s >= 0 && s <= 607) top = 0;   left = (s < left) ? s : left; right = (s > right) ? s : right; }
			if (i2 & 4) { s = (s32)((m * 607) + c + roundUp); if (s >= 0 && s <= 479) right = 607; top = (s < top) ? s : top;  bottom = (s > bottom) ? s : bottom; }
			if (i2 & 8) { s = (s32)(((479 - c) / m) + roundUp); if (s >= 0 && s <= 607) bottom = 479; left = (s < left) ? s : left; right = (s > right) ? s : right; }
		}
	}

	// Wrong bounding box values, discard this polygon (it is outside)
	if (left > 607 || top > 479 || right < 0 || bottom < 0)
		return;

	// Trim bounding box to viewport
	left = (left   < 0) ? 0 : left;
	top = (top    < 0) ? 0 : top;
	right = (right  > 607) ? 607 : right;
	bottom = (bottom > 479) ? 479 : bottom;

	// Update bounding box
	PixelEngine::bbox[0] = (left   < PixelEngine::bbox[0]) ? left : PixelEngine::bbox[0];
	PixelEngine::bbox[1] = (right  > PixelEngine::bbox[1]) ? right : PixelEngine::bbox[1];
	PixelEngine::bbox[2] = (top    < PixelEngine::bbox[2]) ? top : PixelEngine::bbox[2];
	PixelEngine::bbox[3] = (bottom > PixelEngine::bbox[3]) ? bottom : PixelEngine::bbox[3];
}

void VertexLoader_BBox::SetPrimitive(s32 primitive)
{
	s_bbox_primitive = primitive;
	s_bbox_currPoint = 0;
	s_bbox_loadedPoints = 0;
}