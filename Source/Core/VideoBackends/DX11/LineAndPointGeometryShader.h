// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once
#include <unordered_map>
#include <memory>
#include "VideoCommon/VideoCommon.h"
#include "VideoBackends/DX11/D3DPtr.h"

struct ID3D11Buffer;
struct ID3D11GeometryShader;

namespace DX11
{
// This class manages a collection of line and point geometry shaders, one for each
// vertex format.
//TODO: Init/Shutdown in ctor/dtor, no more ready member
class LineAndPointGeometryShader
{
public:
	void Init();
	void Shutdown();
	// Returns true on success, false on failure
	bool SetLineShader(u32 components, float lineWidth, float texOffset,
		float vpWidth, float vpHeight, const bool* texOffsetEnable);

	// Returns true on success, false on failure
	bool SetPointShader(u32 components, float pointSize, float texOffset,
		float vpWidth, float vpHeight, const bool* texOffsetEnable);

private:

	ID3D11GeometryShader* GetShader(u32 components, bool line);
	void UpdateConstantBuffer(float lineWidth, float pointSize, float texOffset,
		float vpWidth, float vpHeight, const bool* texOffsetEnable);

	bool ready_;

	GC_ALIGNED16(struct GSParams
	{
		FLOAT LineWidth; // In units of 1/6 of an EFB pixel
		FLOAT PointSize;
		FLOAT TexOffset;
		UINT  SetPaddingToZero;
		FLOAT VpWidth; // Width and height of the viewport in EFB pixels
		FLOAT VpHeight;
		UINT  SetPaddingToZero1;
		UINT  SetPaddingToZero2;
		FLOAT TexOffsetEnable[8]; // For each tex coordinate, whether to apply offset to it (1 on, 0 off)
		void ClearPadding() {
			SetPaddingToZero = 0;
			SetPaddingToZero1 = 0;
			SetPaddingToZero2 = 0;
		}
	});

	GSParams shadowParamsBuffer_;
	D3D::UniquePtr<ID3D11Buffer> paramsBuffer_;

	using GeometryShaderPtr = D3D::UniquePtr<ID3D11GeometryShader>;
	using ComboMap = std::unordered_map<u32, GeometryShaderPtr>;

	ComboMap lineShaders_;
	ComboMap pointShaders_;
};

}
