// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <vector>

#include "Common/Common.h"
#include "D3DWrapDeviceContext.h"

namespace DX11
{

#define SAFE_RELEASE(x) { if (x) (x)->Release(); (x) = nullptr; }
#define SAFE_DELETE(x) { delete (x); (x) = nullptr; }
#define SAFE_DELETE_ARRAY(x) { delete[] (x); (x) = nullptr; }
#define CHECK(cond, Message, ...) if (!(cond)) { PanicAlert(__FUNCTION__ " failed in %s at line %d: " Message, __FILE__, __LINE__, __VA_ARGS__); }

class D3DTexture2D;

namespace D3D
{

HRESULT LoadDXGI();
HRESULT LoadD3D();
void UnloadDXGI();
void UnloadD3D();

D3D_FEATURE_LEVEL GetFeatureLevel(IDXGIAdapter* adapter);
std::vector<DXGI_SAMPLE_DESC> EnumAAModes(IDXGIAdapter* adapter);
DXGI_SAMPLE_DESC GetAAMode(int index);

HRESULT Create(HWND wnd);
void Close();

extern ID3D11Device* device;
extern ID3D11Device1* device1;
extern WrapDeviceContext context;
extern HWND hWnd;
extern bool bFrameInProgress;

void Reset();
bool BeginFrame();
void EndFrame();
void Present();
D3D_FEATURE_LEVEL GetFeatureLevel();
unsigned int GetBackBufferWidth();
unsigned int GetBackBufferHeight();
D3DTexture2D* &GetBackBuffer();
const char* PixelShaderVersionString();
const char* GeometryShaderVersionString();
const char* VertexShaderVersionString();
const char* ComputeShaderVersionString();
DXGI_FORMAT GetBaseBufferFormat();
bool BGRATexturesSupported();
bool BGRA565TexturesSupported();

unsigned int GetMaxTextureSize();

struct PackedD3DRenderTargetBlendDesc {
	u32 BlendEnable : 1;
	u32 SrcBlend : 5;
	u32 DestBlend : 5;
	u32 BlendOp : 3;
	u32 SrcBlendAlpha : 5;
	u32 DestBlendAlpha : 5;
	u32 BlendOpAlpha : 3;
	u32 RenderTargetWriteMask : 5;
	PackedD3DRenderTargetBlendDesc() = default;
	PackedD3DRenderTargetBlendDesc(D3D11_RENDER_TARGET_BLEND_DESC const & desc) :
		BlendEnable(desc.BlendEnable),
		SrcBlend(desc.SrcBlend),
		DestBlend(desc.DestBlend),
		BlendOp(desc.BlendOp),
		SrcBlendAlpha(desc.SrcBlendAlpha),
		DestBlendAlpha(desc.DestBlendAlpha),
		BlendOpAlpha(desc.BlendOpAlpha),
		RenderTargetWriteMask(desc.RenderTargetWriteMask)
	{
	}
	D3D11_RENDER_TARGET_BLEND_DESC Unpack() const {
		return{
			BlendEnable,
			D3D11_BLEND(SrcBlend),
			D3D11_BLEND(DestBlend),
			D3D11_BLEND_OP(BlendOp),
			D3D11_BLEND(SrcBlendAlpha),
			D3D11_BLEND(DestBlendAlpha),
			D3D11_BLEND_OP(BlendOpAlpha),
			RenderTargetWriteMask
		};
	}
	D3D11_RENDER_TARGET_BLEND_DESC1 Unpack1() const {
		return{
			BlendEnable,
			FALSE,
			D3D11_BLEND(SrcBlend),
			D3D11_BLEND(DestBlend),
			D3D11_BLEND_OP(BlendOp),
			D3D11_BLEND(SrcBlendAlpha),
			D3D11_BLEND(DestBlendAlpha),
			D3D11_BLEND_OP(BlendOpAlpha),
			D3D11_LOGIC_OP_NOOP,
			RenderTargetWriteMask
		};
	}

};

struct PackedD3DBlendDesc {
	u32 AlphaToCoverageEnable : 8;
	u32 IndependentBlendEnable : 8;
	u32 LogicOpEnable : 8;
	u32 LogicOp : 8;
	PackedD3DRenderTargetBlendDesc RenderTarget[8];

	PackedD3DBlendDesc() = default;
	PackedD3DBlendDesc(D3D11_BLEND_DESC const& desc) :
		AlphaToCoverageEnable(desc.AlphaToCoverageEnable),
		IndependentBlendEnable(desc.IndependentBlendEnable)
	{
		for (int i = 0; i != 8; ++i)
			RenderTarget[i] = desc.RenderTarget[i];
	}

	D3D11_BLEND_DESC Unpack() const {
		return{
			AlphaToCoverageEnable,
			IndependentBlendEnable,
			{
				RenderTarget[0].Unpack(), RenderTarget[1].Unpack(), RenderTarget[2].Unpack(),
				RenderTarget[3].Unpack(), RenderTarget[4].Unpack(), RenderTarget[5].Unpack(),
				RenderTarget[6].Unpack(), RenderTarget[7].Unpack(),
			}
		};
	}

	D3D11_BLEND_DESC1 Unpack1() const {
		D3D11_BLEND_DESC1 result{
			AlphaToCoverageEnable,
			IndependentBlendEnable,
			{
				RenderTarget[0].Unpack1(), RenderTarget[1].Unpack1(), RenderTarget[2].Unpack1(),
				RenderTarget[3].Unpack1(), RenderTarget[4].Unpack1(), RenderTarget[5].Unpack1(),
				RenderTarget[6].Unpack1(), RenderTarget[7].Unpack1(),
			}
		};
		for (u32 i = 0; i < 8; i++)
		{
			result.RenderTarget[i].LogicOpEnable = LogicOpEnable;
			result.RenderTarget[i].LogicOp = D3D11_LOGIC_OP(LogicOp);
			if (LogicOpEnable)
				result.RenderTarget[i].BlendEnable = FALSE;
		}
		if (LogicOpEnable)
			result.IndependentBlendEnable = FALSE;
		return result;
	}
};


struct PackedD3DRasterisationDesc {
	u32 FillMode : 6;
	u32 CullMode : 6;
	u32 FrontCounterClockwise : 4;
	u32 DepthClipEnable : 4;
	u32 ScissorEnable : 4;
	u32 MultisampleEnable : 4;
	u32 AntialiasedLineEnable : 4;
	INT DepthBias;
	FLOAT DepthBiasClamp;
	FLOAT SlopeScaledDepthBias;

	PackedD3DRasterisationDesc() = default;
	PackedD3DRasterisationDesc(D3D11_RASTERIZER_DESC const& desc) :
		FillMode(desc.FillMode), CullMode(desc.CullMode),
		FrontCounterClockwise(desc.FrontCounterClockwise), DepthClipEnable(desc.DepthClipEnable),
		ScissorEnable(desc.ScissorEnable), MultisampleEnable(desc.MultisampleEnable),
		AntialiasedLineEnable(desc.AntialiasedLineEnable), DepthBias(desc.DepthBias),
		DepthBiasClamp(desc.DepthBiasClamp), SlopeScaledDepthBias(desc.SlopeScaledDepthBias)
	{
	}


	D3D11_RASTERIZER_DESC Unpack() const {
		return{
			D3D11_FILL_MODE(FillMode), D3D11_CULL_MODE(CullMode),
			FrontCounterClockwise, DepthBias,
			DepthBiasClamp, SlopeScaledDepthBias,
			DepthClipEnable, ScissorEnable,
			MultisampleEnable, AntialiasedLineEnable,
		};
	}
};

HRESULT SetFullscreenState(bool enable_fullscreen);
HRESULT GetFullscreenState(bool* fullscreen_state);

ID3D11RasterizerState*   GetRasterizerState(PackedD3DRasterisationDesc const&, char const* debugNameOnCreation = nullptr);
ID3D11BlendState*        GetBlendState(PackedD3DBlendDesc const&, char const* debugNameOnCreation = nullptr);
ID3D11DepthStencilState* GetDepthStencilState(D3D11_DEPTH_STENCIL_DESC const&, char const* debugNameOnCreation = nullptr);
ID3D11SamplerState*      GetSamplerState(D3D11_SAMPLER_DESC const&, char const* debugNameOnCreation = nullptr);

// Ihis function will assign a name to the given resource.
// The DirectX debug layer will make it easier to identify resources that way,
// e.g. when listing up all resources who have unreleased references.
template <typename T>
void SetDebugObjectName(T resource, const char* name)
{
	static_assert(std::is_convertible<T, ID3D11DeviceChild*>::value,
		"resource must be convertible to ID3D11DeviceChild*");
#if defined(_DEBUG) || defined(DEBUGFAST)
	if (name && resource) resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(name), name);
#endif
	}

}  // namespace D3D

typedef HRESULT(WINAPI *CREATEDXGIFACTORY)(REFIID, void**);
extern CREATEDXGIFACTORY PCreateDXGIFactory;
typedef HRESULT(WINAPI *D3D11CREATEDEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, CONST D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

}  // namespace DX11
