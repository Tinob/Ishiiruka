// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once
#include "Common/ConstantBuffer.h"
#include "VideoCommon/VertexShaderGen.h"

class PointerWrap;

void UpdateProjectionHack(int iParams[], std::string sParams[]);

// The non-API dependent parts.
class VertexShaderManager
{
public:
	static const size_t ConstantBufferSize = C_VENVCONST_END * 4;
	static void Init();
	static void Dirty();
	static void Shutdown();
	static void DoState(PointerWrap &p);
	static float* GetBufferToUpdate(u32 const_number, u32 size);
	static const float* GetBuffer();
	static bool IsDirty();
	static void Clear();
	static void EnableDirtyRegions();
	static void DisableDirtyRegions();
	static const regionvector &GetDirtyRegions();
	// constant management
	static void SetConstants();

	static void InvalidateXFRange(int start, int end);
	static void SetTexMatrixChangedA(u32 value);
	static void SetTexMatrixChangedB(u32 value);
	static void SetViewportChanged();
	static void SetProjectionChanged();
	static void SetMaterialColorChanged(int index);

	static void TranslateView(float x, float y, float z = 0.0f);
	static void RotateView(float x, float y);
	static void ResetView();
private:
	static float vsconstants[ConstantBufferSize];
	static ConstatBuffer m_buffer;
};