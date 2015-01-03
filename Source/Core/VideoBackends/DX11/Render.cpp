// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cinttypes>
#include <cmath>
#include <string>
#include <strsafe.h>
#include <array>
#include "Common/Timer.h"

#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/Host.h"
#include "Core/Movie.h"
#include "VideoBackends/DX11/BoundingBox.h"
#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DPtr.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoBackends/DX11/D3DUtil.h"
#include "VideoBackends/DX11/FramebufferManager.h"

#include "VideoBackends/DX11/PixelShaderCache.h"
#include "VideoBackends/DX11/Render.h"
#include "VideoBackends/DX11/Television.h"
#include "VideoBackends/DX11/TextureCache.h"
#include "VideoBackends/DX11/VertexShaderCache.h"

#include "VideoCommon/AVIDump.h"
#include "VideoCommon/BPFunctions.h"
#include "VideoCommon/Fifo.h"
#include "VideoCommon/FPSCounter.h"
#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/OnScreenDisplay.h"
#include "VideoCommon/PixelEngine.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoConfig.h"

namespace DX11
{

static u32 s_last_multisample_mode = 0;

static Television s_television;

static bool s_last_fullscreen_mode = false;

D3D::BufferPtr access_efb_cbuf;
D3D::BlendStatePtr clearblendstates[4];
D3D::DepthStencilStatePtr cleardepthstates[3];
D3D::BlendStatePtr resetblendstate;
D3D::DepthStencilStatePtr resetdepthstate;
D3D::RasterizerStatePtr resetraststate;

static D3D::Texture2dPtr s_screenshot_texture;


// GX pipeline state
struct
{
	SamplerState sampler[8];
	BlendState blend;
	ZMode zmode;
	RasterizerState raster;

} gx_state;

StateCache gx_state_cache;

void SetupDeviceObjects()
{
	s_television.Init();

	g_framebuffer_manager = new FramebufferManager;

	HRESULT hr;
	float colmat[20]= {0.0f};
	colmat[0] = colmat[5] = colmat[10] = 1.0f;
	D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(20*sizeof(float), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DEFAULT);
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = colmat;
	hr = D3D::device->CreateBuffer(&cbdesc, &data, D3D::ToAddr(access_efb_cbuf));
	CHECK(hr==S_OK, "Create constant buffer for Renderer::AccessEFB");
	D3D::SetDebugObjectName(access_efb_cbuf.get(), "constant buffer for Renderer::AccessEFB");

	D3D11_DEPTH_STENCIL_DESC ddesc;
	ddesc.DepthEnable		= FALSE;
	ddesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	ddesc.DepthFunc			= D3D11_COMPARISON_ALWAYS;
	ddesc.StencilEnable		= FALSE;
	ddesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	ddesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	hr = D3D::device->CreateDepthStencilState(&ddesc, D3D::ToAddr(cleardepthstates[0]));
	CHECK(hr==S_OK, "Create depth state for Renderer::ClearScreen");
	ddesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	ddesc.DepthEnable		= TRUE;
	hr = D3D::device->CreateDepthStencilState(&ddesc, D3D::ToAddr(cleardepthstates[1]));
	CHECK(hr==S_OK, "Create depth state for Renderer::ClearScreen");
	ddesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = D3D::device->CreateDepthStencilState(&ddesc, D3D::ToAddr(cleardepthstates[2]));
	CHECK(hr==S_OK, "Create depth state for Renderer::ClearScreen");
	D3D::SetDebugObjectName(cleardepthstates[0].get(), "depth state for Renderer::ClearScreen (depth buffer disabled)");
	D3D::SetDebugObjectName(cleardepthstates[1].get(), "depth state for Renderer::ClearScreen (depth buffer enabled, writing enabled)");
	D3D::SetDebugObjectName(cleardepthstates[2].get(), "depth state for Renderer::ClearScreen (depth buffer enabled, writing disabled)");

	D3D11_BLEND_DESC blenddesc;
	blenddesc.AlphaToCoverageEnable = FALSE;
	blenddesc.IndependentBlendEnable = FALSE;
	blenddesc.RenderTarget[0].BlendEnable = FALSE;
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	hr = D3D::device->CreateBlendState(&blenddesc, D3D::ToAddr(resetblendstate));
	CHECK(hr==S_OK, "Create blend state for Renderer::ResetAPIState");
	D3D::SetDebugObjectName(resetblendstate.get(), "blend state for Renderer::ResetAPIState");

	clearblendstates[0] = resetblendstate.Share();	

	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED|D3D11_COLOR_WRITE_ENABLE_GREEN|D3D11_COLOR_WRITE_ENABLE_BLUE;
	hr = D3D::device->CreateBlendState(&blenddesc, D3D::ToAddr(clearblendstates[1]));
	CHECK(hr==S_OK, "Create blend state for Renderer::ClearScreen");

	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALPHA;
	hr = D3D::device->CreateBlendState(&blenddesc, D3D::ToAddr(clearblendstates[2]));
	CHECK(hr==S_OK, "Create blend state for Renderer::ClearScreen");

	blenddesc.RenderTarget[0].RenderTargetWriteMask = 0;
	hr = D3D::device->CreateBlendState(&blenddesc, D3D::ToAddr(clearblendstates[3]));
	CHECK(hr==S_OK, "Create blend state for Renderer::ClearScreen");

	ddesc.DepthEnable		= FALSE;
	ddesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	ddesc.DepthFunc			= D3D11_COMPARISON_LESS;
	ddesc.StencilEnable		= FALSE;
	ddesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	ddesc.StencilWriteMask	= D3D11_DEFAULT_STENCIL_WRITE_MASK;
	hr = D3D::device->CreateDepthStencilState(&ddesc, D3D::ToAddr(resetdepthstate));
	CHECK(hr==S_OK, "Create depth state for Renderer::ResetAPIState");
	D3D::SetDebugObjectName(resetdepthstate.get(), "depth stencil state for Renderer::ResetAPIState");

	D3D11_RASTERIZER_DESC rastdesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.f, 0.f, false, false, false, false);
	hr = D3D::device->CreateRasterizerState(&rastdesc, D3D::ToAddr(resetraststate));
	CHECK(hr==S_OK, "Create rasterizer state for Renderer::ResetAPIState");
	D3D::SetDebugObjectName(resetraststate.get(), "rasterizer state for Renderer::ResetAPIState");

	s_screenshot_texture.reset();
}

// Kill off all device objects
void TeardownDeviceObjects()
{
	delete g_framebuffer_manager;

	access_efb_cbuf.reset();
	clearblendstates[0].reset();
	clearblendstates[1].reset();
	clearblendstates[2].reset();
	clearblendstates[3].reset();
	cleardepthstates[0].reset();
	cleardepthstates[1].reset();
	cleardepthstates[2].reset();
	resetblendstate.reset();
	resetdepthstate.reset();
	resetraststate.reset();
	s_screenshot_texture.reset();

	s_television.Shutdown();
	
	gx_state_cache.Clear();
}

void CreateScreenshotTexture(const TargetRectangle& rc)
{
	D3D11_TEXTURE2D_DESC scrtex_desc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, rc.GetWidth(), rc.GetHeight(), 1, 1, 0, D3D11_USAGE_STAGING, D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE);
	HRESULT hr = D3D::device->CreateTexture2D(&scrtex_desc, nullptr, D3D::ToAddr(s_screenshot_texture));
	CHECK(hr == S_OK, "Create screenshot staging texture");
	D3D::SetDebugObjectName(s_screenshot_texture.get(), "staging screenshot texture");
}

Renderer::Renderer(void *&window_handle)
{
	D3D::Create((HWND)window_handle);
	
	s_backbuffer_width = D3D::GetBackBufferWidth();
	s_backbuffer_height = D3D::GetBackBufferHeight();

	FramebufferManagerBase::SetLastXfbWidth(MAX_XFB_WIDTH);
	FramebufferManagerBase::SetLastXfbHeight(MAX_XFB_HEIGHT);

	UpdateDrawRectangle(s_backbuffer_width, s_backbuffer_height);

	s_last_multisample_mode = g_ActiveConfig.iMultisampleMode;
	s_LastEFBScale = g_ActiveConfig.iEFBScale;
	s_last_fullscreen_mode = g_ActiveConfig.bFullscreen;
	CalculateTargetSize(s_backbuffer_width, s_backbuffer_height);

	SetupDeviceObjects();


	// Setup GX pipeline state
	gx_state.blend.blend_enable = false;
	gx_state.blend.write_mask = D3D11_COLOR_WRITE_ENABLE_ALL;
	gx_state.blend.src_blend = D3D11_BLEND_ONE;
	gx_state.blend.dst_blend = D3D11_BLEND_ZERO;
	gx_state.blend.blend_op = D3D11_BLEND_OP_ADD;
	gx_state.blend.use_dst_alpha = false;

	for (unsigned int k = 0; k < 8; k++)
	{
		gx_state.sampler[k].packed = 0;
	}

	gx_state.zmode.testenable = false;
	gx_state.zmode.updateenable = false;
	gx_state.zmode.func = ZMode::NEVER;

	gx_state.raster.cull_mode = D3D11_CULL_NONE;

	// Clear EFB textures
	float ClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	D3D::context->ClearRenderTargetView(FramebufferManager::GetEFBColorTexture()->GetRTV(), ClearColor);
	D3D::context->ClearDepthStencilView(FramebufferManager::GetEFBDepthTexture()->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);

	D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.f, 0.f, (float)s_target_width, (float)s_target_height);
	D3D::context->RSSetViewports(1, &vp);
	D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTexture()->GetRTV(), FramebufferManager::GetEFBDepthTexture()->GetDSV());
	D3D::BeginFrame();
}

Renderer::~Renderer()
{
	TeardownDeviceObjects();
	D3D::EndFrame();
	D3D::Present();
	D3D::Close();
}

void Renderer::RenderText(const std::string& text, int left, int top, u32 color)
{
	TargetRectangle trc = GetTargetRectangle();	
	const int nBackbufferWidth = trc.right - trc.left;
	const int nBackbufferHeight = trc.bottom - trc.top;

	float scalex = 1 / (float)nBackbufferWidth * 2.f;
	float scaley = 1 / (float)nBackbufferHeight * 2.f;
	

	D3D::font.DrawTextScaled((float)left, (float)top, 20.f, 0.0f, color, text.c_str(), scalex, scaley);
}

TargetRectangle Renderer::ConvertEFBRectangle(const EFBRectangle& rc)
{
	TargetRectangle result;
	result.left   = EFBToScaledX(rc.left);
	result.top    = EFBToScaledY(rc.top);
	result.right  = EFBToScaledX(rc.right);
	result.bottom = EFBToScaledY(rc.bottom);
	return result;
}

// With D3D, we have to resize the backbuffer if the window changed
// size.
bool Renderer::CheckForResize()
{
	RECT rcWindow;
	GetClientRect(D3D::hWnd, &rcWindow);
	int client_width = rcWindow.right - rcWindow.left;
	int client_height = rcWindow.bottom - rcWindow.top;

	// Sanity check
	if ((client_width != Renderer::GetBackbufferWidth() ||
		client_height != Renderer::GetBackbufferHeight()) && 
		client_width >= 4 && client_height >= 4)
	{
		return true;
	}

	return false;
}

void Renderer::SetScissorRect(const TargetRectangle& rc)
{
	D3D::context->RSSetScissorRects(1, rc.AsRECT());
}

void Renderer::SetColorMask()
{
	// Only enable alpha channel if it's supported by the current EFB format
	UINT8 color_mask = 0;
	if (bpmem.alpha_test.TestResult() != AlphaTest::FAIL)
	{
		if (bpmem.blendmode.alphaupdate && (bpmem.zcontrol.pixel_format == PEControl::RGBA6_Z24))
			color_mask = D3D11_COLOR_WRITE_ENABLE_ALPHA;
		if (bpmem.blendmode.colorupdate)
			color_mask |= D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
	}
	gx_state.blend.write_mask = color_mask;
}

// This function allows the CPU to directly access the EFB.
// There are EFB peeks (which will read the color or depth of a pixel)
// and EFB pokes (which will change the color or depth of a pixel).
//
// The behavior of EFB peeks can only be modified by:
//	- GX_PokeAlphaRead
// The behavior of EFB pokes can be modified by:
//	- GX_PokeAlphaMode (TODO)
//	- GX_PokeAlphaUpdate (TODO)
//	- GX_PokeBlendMode (TODO)
//	- GX_PokeColorUpdate (TODO)
//	- GX_PokeDither (TODO)
//	- GX_PokeDstAlpha (TODO)
//	- GX_PokeZMode (TODO)
u32 Renderer::AccessEFB(EFBAccessType type, u32 x, u32 y, u32 poke_data)
{
	// TODO: This function currently is broken if anti-aliasing is enabled
	D3D11_MAPPED_SUBRESOURCE map;
	ID3D11Texture2D* read_tex;

	if (type == POKE_Z)
	{
		static bool alert_only_once = true;
		if (!alert_only_once) return 0;
		PanicAlert("EFB: Poke Z not implemented (tried to poke z value %#x at (%d,%d))", poke_data, x, y);
		alert_only_once = false;
		return 0;
	}

	// Convert EFB dimensions to the ones of our render target
	EFBRectangle efbPixelRc;
	efbPixelRc.left = x;
	efbPixelRc.top = y;
	efbPixelRc.right = x + 1;
	efbPixelRc.bottom = y + 1;
	TargetRectangle targetPixelRc = Renderer::ConvertEFBRectangle(efbPixelRc);

	// Take the mean of the resulting dimensions; TODO: Don't use the center pixel, compute the average color instead
	D3D11_RECT RectToLock;
	if(type == PEEK_COLOR || type == PEEK_Z)
	{
		RectToLock.left = (targetPixelRc.left + targetPixelRc.right) / 2;
		RectToLock.top = (targetPixelRc.top + targetPixelRc.bottom) / 2;
		RectToLock.right = RectToLock.left + 1;
		RectToLock.bottom = RectToLock.top + 1;
	}
	else
	{
		RectToLock.left = targetPixelRc.left;
		RectToLock.right = targetPixelRc.right;
		RectToLock.top = targetPixelRc.top;
		RectToLock.bottom = targetPixelRc.bottom;
	}

	if (type == PEEK_Z)
	{
		ResetAPIState(); // Reset any game specific settings

		// depth buffers can only be completely CopySubresourceRegion'ed, so we're using drawShadedTexQuad instead
		D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBDepthReadTexture()->GetRTV(), nullptr);
		D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.f, 0.f, 1.f, 1.f);
		D3D::context->RSSetViewports(1, &vp);
		D3D::stateman->SetPixelConstants(0, access_efb_cbuf.get());
		D3D::SetPointCopySampler();
		D3D::drawShadedTexQuad(FramebufferManager::GetEFBDepthTexture()->GetSRV(),
								&RectToLock,
								Renderer::GetTargetWidth(),
								Renderer::GetTargetHeight(),
								PixelShaderCache::GetColorCopyProgram(true),
								VertexShaderCache::GetSimpleVertexShader(),
								VertexShaderCache::GetSimpleInputLayout());

		D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTexture()->GetRTV(), FramebufferManager::GetEFBDepthTexture()->GetDSV());

		// copy to system memory
		D3D11_BOX box = CD3D11_BOX(0, 0, 0, 1, 1, 1);
		read_tex = FramebufferManager::GetEFBDepthStagingBuffer();
		D3D::context->CopySubresourceRegion(read_tex, 0, 0, 0, 0, FramebufferManager::GetEFBDepthReadTexture()->GetTex(), 0, &box);

		RestoreAPIState(); // restore game state

		// read the data from system memory
		D3D::context->Map(read_tex, 0, D3D11_MAP_READ, 0, &map);

		float val = *(float*)map.pData;
		u32 ret = 0;
		if(bpmem.zcontrol.pixel_format == PEControl::RGB565_Z16)
		{
			// if Z is in 16 bit format you must return a 16 bit integer
			ret = ((u32)(val * 0xffff));
		}
		else
		{
			ret = ((u32)(val * 0xffffff));
		}
		D3D::context->Unmap(read_tex, 0);

		// TODO: in RE0 this value is often off by one in Video_DX9 (where this code is derived from), which causes lighting to disappear
		return ret;
	}
	else if (type == PEEK_COLOR)
	{
		// we can directly copy to system memory here
		read_tex = FramebufferManager::GetEFBColorStagingBuffer();
		D3D11_BOX box = CD3D11_BOX(RectToLock.left, RectToLock.top, 0, RectToLock.right, RectToLock.bottom, 1);
		D3D::context->CopySubresourceRegion(read_tex, 0, 0, 0, 0, FramebufferManager::GetEFBColorTexture()->GetTex(), 0, &box);

		// read the data from system memory
		D3D::context->Map(read_tex, 0, D3D11_MAP_READ, 0, &map);
		u32 ret = 0;
		if(map.pData)
			ret = *(u32*)map.pData;
		D3D::context->Unmap(read_tex, 0);

		// check what to do with the alpha channel (GX_PokeAlphaRead)
		PixelEngine::UPEAlphaReadReg alpha_read_mode = PixelEngine::GetAlphaReadMode();

		if (bpmem.zcontrol.pixel_format == PEControl::RGBA6_Z24)
		{
			ret = RGBA8ToRGBA6ToRGBA8(ret);
		}
		else if (bpmem.zcontrol.pixel_format == PEControl::RGB565_Z16)
		{
			ret = RGBA8ToRGB565ToRGBA8(ret);
		}			
		if(bpmem.zcontrol.pixel_format != PEControl::RGBA6_Z24)
		{
			ret |= 0xFF000000;
		}

		if(alpha_read_mode.ReadMode == 2) return ret; // GX_READ_NONE
		else if(alpha_read_mode.ReadMode == 1) return (ret | 0xFF000000); // GX_READ_FF
		else /*if(alpha_read_mode.ReadMode == 0)*/ return (ret & 0x00FFFFFF); // GX_READ_00
	}
	else //if(type == POKE_COLOR)
	{
		u32 rgbaColor = (poke_data & 0xFF00FF00) | ((poke_data >> 16) & 0xFF) | ((poke_data << 16) & 0xFF0000);

		// TODO: The first five PE registers may change behavior of EFB pokes, this isn't implemented, yet.
		ResetAPIState();
		D3D::drawColorQuad(rgbaColor, (float)RectToLock.left   * 2.f / (float)Renderer::GetTargetWidth()  - 1.f,
									- (float)RectToLock.top	* 2.f / (float)Renderer::GetTargetHeight() + 1.f,
									  (float)RectToLock.right  * 2.f / (float)Renderer::GetTargetWidth()  - 1.f,
									- (float)RectToLock.bottom * 2.f / (float)Renderer::GetTargetHeight() + 1.f);

		RestoreAPIState();
		return 0;
	}
}

// Viewport correction:
// Say you want a viewport at (ix, iy) with size (iw, ih),
// but your viewport must be clamped at (ax, ay) with size (aw, ah).
// Just multiply the projection matrix with the following to get the same
// effect:
// [   (iw/aw)         0     0    ((iw - 2*(ax-ix)) / aw - 1)   ]
// [         0   (ih/ah)     0   ((-ih + 2*(ay-iy)) / ah + 1)   ]
// [         0         0     1                              0   ]
// [         0         0     0                              1   ]
static void ViewportCorrectionMatrix(Matrix44& result,
	float ix, float iy, float iw, float ih, // Intended viewport (x, y, width, height)
	float ax, float ay, float aw, float ah) // Actual viewport (x, y, width, height)
{
	Matrix44::LoadIdentity(result);
	if (aw == 0.f || ah == 0.f)
		return;
	result.data[4*0+0] = iw / aw;
	result.data[4*0+3] = (iw - 2.f * (ax - ix)) / aw - 1.f;
	result.data[4*1+1] = ih / ah;
	result.data[4*1+3] = (-ih + 2.f * (ay - iy)) / ah + 1.f;
}

// Called from VertexShaderManager
void Renderer::UpdateViewport(Matrix44& vpCorrection)
{
	// reversed gxsetviewport(xorig, yorig, width, height, nearz, farz)
	// [0] = width/2
	// [1] = height/2
	// [2] = 16777215 * (farz - nearz)
	// [3] = xorig + width/2 + 342
	// [4] = yorig + height/2 + 342
	// [5] = 16777215 * farz

	int scissorXOff = bpmem.scissorOffset.x * 2;
	int scissorYOff = bpmem.scissorOffset.y * 2;

	// TODO: ceil, floor or just cast to int?
	// TODO: Directly use the floats instead of rounding them?
	int intendedX = Renderer::EFBToScaledX((int)ceil(xfmem.viewport.xOrig - xfmem.viewport.wd - scissorXOff));
	int intendedY = Renderer::EFBToScaledY((int)ceil(xfmem.viewport.yOrig + xfmem.viewport.ht - scissorYOff));
	int intendedWd = Renderer::EFBToScaledX((int)ceil(2.0f * xfmem.viewport.wd));
	int intendedHt = Renderer::EFBToScaledY((int)ceil(-2.0f * xfmem.viewport.ht));
	if (intendedWd < 0)
	{
		intendedX += intendedWd;
		intendedWd = -intendedWd;
	}
	if (intendedHt < 0)
	{
		intendedY += intendedHt;
		intendedHt = -intendedHt;
	}

	// In D3D, the viewport rectangle must fit within the render target.
	int X = intendedX;
	if (X < 0)
		X = 0;

	int Y = intendedY;
	if (Y < 0)
		Y = 0;

	int Wd = intendedWd;
	if (X + Wd > GetTargetWidth())
		Wd = GetTargetWidth() - X;
	int Ht = intendedHt;
	if (Y + Ht > GetTargetHeight())
		Ht = GetTargetHeight() - Y;
	
	// If GX viewport is off the render target, we must clamp our viewport
	// within the bounds. Use the correction matrix to compensate.
	ViewportCorrectionMatrix(vpCorrection,
		(float)intendedX, (float)intendedY,
		(float)intendedWd, (float)intendedHt,
		(float)X, (float)Y,
		(float)Wd, (float)Ht);

	// Some games set invalid values for z-min and z-max so fix them to the max and min allowed and let the shaders do this work
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT((float)X, (float)Y,
										(float)Wd, (float)Ht,
										0.f,	// (xfmem.viewport.farZ - xfmem.viewport.zRange) / 16777216.0f;
										1.f);   //  xfmem.viewport.farZ / 16777216.0f;
	D3D::context->RSSetViewports(1, &vp);
}

void Renderer::ClearScreen(const EFBRectangle& rc, bool colorEnable, bool alphaEnable, bool zEnable, u32 color, u32 z)
{
	ResetAPIState();

	if (colorEnable && alphaEnable) D3D::stateman->PushBlendState(clearblendstates[0].get());
	else if (colorEnable) D3D::stateman->PushBlendState(clearblendstates[1].get());
	else if (alphaEnable) D3D::stateman->PushBlendState(clearblendstates[2].get());
	else D3D::stateman->PushBlendState(clearblendstates[3].get());

	// TODO: Should we enable Z testing here?
	/*if (!bpmem.zmode.testenable) D3D::stateman->PushDepthState(cleardepthstates[0]);
	else */if (zEnable) D3D::stateman->PushDepthState(cleardepthstates[1].get());
	else /*if (!zEnable)*/ D3D::stateman->PushDepthState(cleardepthstates[2].get());

	// Update the view port for clearing the picture
	TargetRectangle targetRc = Renderer::ConvertEFBRectangle(rc);
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT((float)targetRc.left, (float)targetRc.top, (float)targetRc.GetWidth(), (float)targetRc.GetHeight(), 0.f, 1.f); 
	D3D::context->RSSetViewports(1, &vp);

	// Color is passed in bgra mode so we need to convert it to rgba
	u32 rgbaColor = (color & 0xFF00FF00) | ((color >> 16) & 0xFF) | ((color << 16) & 0xFF0000);
	D3D::drawClearQuad(rgbaColor, (z & 0xFFFFFF) / float(0xFFFFFF), PixelShaderCache::GetClearProgram(), VertexShaderCache::GetClearVertexShader(), VertexShaderCache::GetClearInputLayout());

	D3D::stateman->PopDepthState();
	D3D::stateman->PopBlendState();

	RestoreAPIState();
}

void Renderer::ReinterpretPixelData(unsigned int convtype)
{
	// TODO: MSAA support..
	D3D11_RECT source = CD3D11_RECT(0, 0, g_renderer->GetTargetWidth(), g_renderer->GetTargetHeight());

	ID3D11PixelShader* pixel_shader;
	if (convtype == 0) pixel_shader = PixelShaderCache::ReinterpRGB8ToRGBA6(true);
	else if (convtype == 2) pixel_shader = PixelShaderCache::ReinterpRGBA6ToRGB8(true);
	else
	{
		ERROR_LOG(VIDEO, "Trying to reinterpret pixel data with unsupported conversion type %d", convtype);
		return;
	}

	// convert data and set the target texture as our new EFB
	g_renderer->ResetAPIState();
	D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTempTexture()->GetRTV(), nullptr);
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.f, 0.f, (float)g_renderer->GetTargetWidth(), (float)g_renderer->GetTargetHeight());
	D3D::context->RSSetViewports(1, &vp);
	
	D3D::SetPointCopySampler();
	D3D::drawShadedTexQuad(FramebufferManager::GetEFBColorTexture()->GetSRV(), &source, g_renderer->GetTargetWidth(), g_renderer->GetTargetHeight(), pixel_shader, VertexShaderCache::GetSimpleVertexShader(), VertexShaderCache::GetSimpleInputLayout());

	g_renderer->RestoreAPIState();

	FramebufferManager::SwapReinterpretTexture();
	D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTexture()->GetRTV(), FramebufferManager::GetEFBDepthTexture()->GetDSV());
}

void Renderer::SetBlendMode(bool forceUpdate)
{
	// Our render target always uses an alpha channel, so we need to override the blend functions to assume a destination alpha of 1 if the render target isn't supposed to have an alpha channel
	// Example: D3DBLEND_DESTALPHA needs to be D3DBLEND_ONE since the result without an alpha channel is assumed to always be 1.
	bool target_has_alpha = bpmem.zcontrol.pixel_format == PEControl::RGBA6_Z24;
	const D3D11_BLEND d3dSrcFactors[8] =
	{
		D3D11_BLEND_ZERO,
		D3D11_BLEND_ONE,
		D3D11_BLEND_DEST_COLOR,
		D3D11_BLEND_INV_DEST_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA, // NOTE: Use SRC1_ALPHA if dst alpha is enabled!
		(target_has_alpha) ? D3D11_BLEND_DEST_ALPHA : D3D11_BLEND_ONE,
		(target_has_alpha) ? D3D11_BLEND_INV_DEST_ALPHA : D3D11_BLEND_ZERO
	};
	const D3D11_BLEND d3dDestFactors[8] =
	{
		D3D11_BLEND_ZERO,
		D3D11_BLEND_ONE,
		D3D11_BLEND_SRC_COLOR,
		D3D11_BLEND_INV_SRC_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA, // NOTE: Use SRC1_ALPHA if dst alpha is enabled!
		(target_has_alpha) ? D3D11_BLEND_DEST_ALPHA : D3D11_BLEND_ONE,
		(target_has_alpha) ? D3D11_BLEND_INV_DEST_ALPHA : D3D11_BLEND_ZERO
	};

	if (bpmem.blendmode.logicopenable && !bpmem.blendmode.blendenable && !forceUpdate)
		return;

	if (bpmem.blendmode.subtract)
	{
		gx_state.blend.blend_enable = true;
		gx_state.blend.blend_op = D3D11_BLEND_OP_REV_SUBTRACT;
		gx_state.blend.src_blend = D3D11_BLEND_ONE;
		gx_state.blend.dst_blend = D3D11_BLEND_ONE;
	}
	else
	{
		gx_state.blend.blend_enable = (u32)bpmem.blendmode.blendenable;
		if (bpmem.blendmode.blendenable)
		{
			gx_state.blend.blend_op = D3D11_BLEND_OP_ADD;
			gx_state.blend.src_blend = d3dSrcFactors[bpmem.blendmode.srcfactor];
			gx_state.blend.dst_blend = d3dDestFactors[bpmem.blendmode.dstfactor];
		}
	}
}

bool Renderer::SaveScreenshot(const std::string &filename, const TargetRectangle& rc)
{
	if (!s_screenshot_texture)
		CreateScreenshotTexture(rc);

	// copy back buffer to system memory
	D3D11_BOX box = CD3D11_BOX(rc.left, rc.top, 0, rc.right, rc.bottom, 1);
	D3D::context->CopySubresourceRegion(s_screenshot_texture.get(), 0, 0, 0, 0, (ID3D11Resource*)D3D::GetBackBuffer()->GetTex(), 0, &box);

	D3D11_MAPPED_SUBRESOURCE map;
	D3D::context->Map(s_screenshot_texture.get(), 0, D3D11_MAP_READ_WRITE, 0, &map);

	bool saved_png = TextureToPng((u8*)map.pData, map.RowPitch, filename, rc.GetWidth(), rc.GetHeight(), false);

	D3D::context->Unmap(s_screenshot_texture.get(), 0);


	if (saved_png)
	{
		OSD::AddMessage(StringFromFormat("Saved %i x %i %s", rc.GetWidth(),
			rc.GetHeight(), filename.c_str()));
	}
	else
	{
		OSD::AddMessage(StringFromFormat("Error saving %s", filename.c_str()));
	}

	return saved_png;
}

void formatBufferDump(const u8* in, u8* out, int w, int h, int p)
{
	for (int y = 0; y < h; ++y)
	{
		auto line = (in + (h - y - 1) * p);
		for (int x = 0; x < w; ++x)
		{
			out[0] = line[2];
			out[1] = line[1];
			out[2] = line[0];
			out += 3;
			line += 4;
		}
	}
}

// This function has the final picture. We adjust the aspect ratio here.
void Renderer::SwapImpl(u32 xfbAddr, u32 fbWidth, u32 fbStride, u32 fbHeight, const EFBRectangle& rc, float Gamma)
{
	if (g_bSkipCurrentFrame || (!XFBWrited && !g_ActiveConfig.RealXFBEnabled()) || !fbWidth || !fbHeight)
	{
		if (SConfig::GetInstance().m_DumpFrames && !frame_data.empty())
			AVIDump::AddFrame(&frame_data[0], fbWidth, fbHeight);

		Core::Callback_VideoCopiedToXFB(false);
		return;
	}

	u32 xfbCount = 0;
	const XFBSourceBase* const* xfbSourceList = FramebufferManager::GetXFBSource(xfbAddr, fbWidth, fbHeight, &xfbCount);
	if ((!xfbSourceList || xfbCount == 0) && g_ActiveConfig.bUseXFB && !g_ActiveConfig.bUseRealXFB)
	{
		if (SConfig::GetInstance().m_DumpFrames && !frame_data.empty())
			AVIDump::AddFrame(&frame_data[0], fbWidth, fbHeight);

		Core::Callback_VideoCopiedToXFB(false);
		return;
	}

	ResetAPIState();

	// Prepare to copy the XFBs to our backbuffer
	UpdateDrawRectangle(s_backbuffer_width, s_backbuffer_height);
	const TargetRectangle Tr = GetTargetRectangle();
	int X = Tr.left;
	int Y = Tr.top;
	int Width  = Tr.right - Tr.left;
	int Height = Tr.bottom - Tr.top;
	float scalex = 1 / (float)Width * 2.f;
	float scaley = 1 / (float)Height * 2.f;
	D3D::context->OMSetRenderTargets(1, &D3D::GetBackBuffer()->GetRTV(), nullptr);
	D3D11_VIEWPORT vp = CD3D11_VIEWPORT((float)X, (float)Y, (float)Width, (float)Height);
	D3D::context->RSSetViewports(1, &vp);
	

	// activate linear filtering for the buffer copies
	D3D::SetLinearCopySampler();

	if (g_ActiveConfig.bUseXFB && g_ActiveConfig.bUseRealXFB)
	{
		// TODO: Television should be used to render Virtual XFB mode as well.
		s_television.Submit(xfbAddr, fbStride, fbWidth, fbHeight);
		s_television.Render();
	}
	else if(g_ActiveConfig.bUseXFB)
	{
		const XFBSourceBase* xfbSource;

		// draw each xfb source
		for (u32 i = 0; i < xfbCount; ++i)
		{
			xfbSource = xfbSourceList[i];
			MathUtil::Rectangle<float> sourceRc;
			
			sourceRc.left = 0;
			sourceRc.top = 0;
			sourceRc.right = (float)xfbSource->texWidth;
			sourceRc.bottom = (float)xfbSource->texHeight;

			MathUtil::Rectangle<float> drawRc;

			if (g_ActiveConfig.bUseRealXFB)
			{
				drawRc.top = 1;
				drawRc.bottom = -1;
				drawRc.left = -1;
				drawRc.right = 1;
			}
			else
			{
				// use virtual xfb with offset
				int xfbHeight = xfbSource->srcHeight;
				int xfbWidth = xfbSource->srcWidth;
				int hOffset = ((s32)xfbSource->srcAddr - (s32)xfbAddr) / ((s32)fbStride * 2);

				drawRc.top = 1.0f - (2.0f * (hOffset) / (float)fbHeight);
				drawRc.bottom = 1.0f - (2.0f * (hOffset + xfbHeight) / (float)fbHeight);
				drawRc.left = -(xfbWidth / (float)fbStride);
				drawRc.right = (xfbWidth / (float)fbStride);

				// The following code disables auto stretch.  Kept for reference.
				// scale draw area for a 1 to 1 pixel mapping with the draw target
				//float vScale = (float)fbHeight / (float)s_backbuffer_height;
				//float hScale = (float)fbWidth / (float)s_backbuffer_width;
				//drawRc.top *= vScale;
				//drawRc.bottom *= vScale;
				//drawRc.left *= hScale;
				//drawRc.right *= hScale;
			}

			xfbSource->Draw(sourceRc, drawRc, 0, 0);
		}
	}
	else
	{
		TargetRectangle targetRc = Renderer::ConvertEFBRectangle(rc);

		// TODO: Improve sampling algorithm for the pixel shader so that we can use the multisampled EFB texture as source
		D3DTexture2D* read_texture = FramebufferManager::GetResolvedEFBColorTexture();
		D3D::drawShadedTexQuad(read_texture->GetSRV(), targetRc.AsRECT(), Renderer::GetTargetWidth(), Renderer::GetTargetHeight(), PixelShaderCache::GetColorCopyProgram(false),VertexShaderCache::GetSimpleVertexShader(), VertexShaderCache::GetSimpleInputLayout(), Gamma);
	}

	// done with drawing the game stuff, good moment to save a screenshot
	if (s_bScreenshot)
	{
		SaveScreenshot(s_sScreenshotName, GetTargetRectangle());
		s_bScreenshot = false;
	}

	// Dump frames
	static int w = 0, h = 0;
	if (g_ActiveConfig.bDumpFrames)
	{
		static int s_recordWidth;
		static int s_recordHeight;

		if (!s_screenshot_texture)
			CreateScreenshotTexture(GetTargetRectangle());

		D3D11_BOX box = CD3D11_BOX(GetTargetRectangle().left, GetTargetRectangle().top, 0, GetTargetRectangle().right, GetTargetRectangle().bottom, 1);
		D3D::context->CopySubresourceRegion(s_screenshot_texture.get(), 0, 0, 0, 0, (ID3D11Resource*)D3D::GetBackBuffer()->GetTex(), 0, &box);
		if (!bLastFrameDumped)
		{
			s_recordWidth = GetTargetRectangle().GetWidth();
			s_recordHeight = GetTargetRectangle().GetHeight();
			bAVIDumping = AVIDump::Start(D3D::hWnd, s_recordWidth, s_recordHeight);
			if (!bAVIDumping)
			{
				PanicAlert("Error dumping frames to AVI.");
			}
			else
			{
				char msg [255];
				sprintf_s(msg,255, "Dumping Frames to \"%sframedump0.avi\" (%dx%d RGB24)",
						File::GetUserPath(D_DUMPFRAMES_IDX).c_str(), s_recordWidth, s_recordHeight);
				OSD::AddMessage(msg, 2000);
			}
		}
		if (bAVIDumping)
		{
			D3D11_MAPPED_SUBRESOURCE map;
			D3D::context->Map(s_screenshot_texture.get(), 0, D3D11_MAP_READ, 0, &map);

			if (frame_data.empty() || w != s_recordWidth || h != s_recordHeight)
			{
				frame_data.resize(3 * s_recordWidth * s_recordHeight);
				w = s_recordWidth;
				h = s_recordHeight;
			}
			formatBufferDump((u8*)map.pData, &frame_data[0], s_recordWidth, s_recordHeight, map.RowPitch);
			AVIDump::AddFrame(&frame_data[0], GetTargetRectangle().GetWidth(), GetTargetRectangle().GetHeight());
			D3D::context->Unmap(s_screenshot_texture.get(), 0);
		}
		bLastFrameDumped = true;
	}
	else
	{
		if (bLastFrameDumped && bAVIDumping)
		{
			std::vector<u8>().swap(frame_data);
			w = h = 0;

			AVIDump::Stop();
			bAVIDumping = false;
			OSD::AddMessage("Stop dumping frames to AVI", 2000);
		}
		bLastFrameDumped = false;
	}

	// Finish up the current frame, print some stats
	if (g_ActiveConfig.bShowFPS || SConfig::GetInstance().m_ShowFrameCount)
	{
		std::string fps = "";
		if (g_ActiveConfig.bShowFPS)
			fps = StringFromFormat("FPS: %d", m_fps_counter.m_fps);

		if (g_ActiveConfig.bShowFPS && SConfig::GetInstance().m_ShowFrameCount)
			fps += " - ";
		if (SConfig::GetInstance().m_ShowFrameCount)
		{
			fps += StringFromFormat("Frame: %d", Movie::g_currentFrame);
			if (Movie::IsPlayingInput())
				fps += StringFromFormat(" / %d", Movie::g_totalFrames);
		}

		fps += "\n";
		D3D::font.DrawTextScaled(0, 0, 20, 0.0f, 0xFF00FFFF, fps, scalex, scaley);
	}

	if (SConfig::GetInstance().m_ShowLag)
	{
		std::string lag = StringFromFormat("Lag: %" PRIu64 "\n", Movie::g_currentLagCount);
		D3D::font.DrawTextScaled(0, 18, 20, 0.0f, 0xFF00FFFF, lag, scalex, scaley);
	}

	if (g_ActiveConfig.bShowInputDisplay)
	{
		D3D::font.DrawTextScaled(0, 36, 20, 0.0f, 0xFF00FFFF, Movie::GetInputDisplay(), scalex, scaley);
	}
	Renderer::DrawDebugText();

	if (g_ActiveConfig.bOverlayStats)
	{
		D3D::font.DrawTextScaled(0, 36, 20, 0.0f, 0xFF00FFFF, Statistics::ToString(), scalex, scaley);
	}
	else if (g_ActiveConfig.bOverlayProjStats)
	{
		D3D::font.DrawTextScaled(0, 36, 20, 0.0f, 0xFF00FFFF, Statistics::ToStringProj(), scalex, scaley);
	}

	OSD::DrawMessages();
	D3D::EndFrame();

	TextureCache::Cleanup();

	// Enable configuration changes
	UpdateActiveConfig();
	TextureCache::OnConfigChanged(g_ActiveConfig);

	SetWindowSize(fbWidth, fbHeight);

	const bool windowResized = CheckForResize();
	const bool fullscreen = g_ActiveConfig.bFullscreen &&
		!SConfig::GetInstance().m_LocalCoreStartupParameter.bRenderToMain;

	bool fullscreen_changed = s_last_fullscreen_mode != fullscreen;
	bool fullscreen_state;
	if (SUCCEEDED(D3D::GetFullscreenState(&fullscreen_state)))
	{
		if (fullscreen_state != fullscreen && Host_RendererHasFocus())
		{
			// The current fullscreen state does not match the configuration,
			// this may happen when the renderer frame loses focus. When the
			// render frame is in focus again we can re-apply the configuration.
			fullscreen_changed = true;
		}
	}

	bool xfbchanged = false;

	if (FramebufferManagerBase::LastXfbWidth() != fbWidth || FramebufferManagerBase::LastXfbHeight() != fbHeight)
	{
		xfbchanged = true;
		unsigned int w = (fbWidth < 1 || fbWidth > MAX_XFB_WIDTH) ? MAX_XFB_WIDTH : fbWidth;
		unsigned int h = (fbHeight < 1 || fbHeight > MAX_XFB_HEIGHT) ? MAX_XFB_HEIGHT : fbHeight;
		FramebufferManagerBase::SetLastXfbWidth(w);
		FramebufferManagerBase::SetLastXfbHeight(h);
	}

	// Flip/present backbuffer to frontbuffer here
	D3D::Present();

	// resize the back buffers NOW to avoid flickering
	if (xfbchanged ||
		windowResized ||
		fullscreen_changed ||
		s_LastEFBScale != g_ActiveConfig.iEFBScale ||
		s_last_multisample_mode != g_ActiveConfig.iMultisampleMode)
	{
		s_last_multisample_mode = g_ActiveConfig.iMultisampleMode;
		PixelShaderCache::InvalidateMSAAShaders();

		if (windowResized || fullscreen_changed)
		{
			// Apply fullscreen state
			if (fullscreen_changed)
			{
				s_last_fullscreen_mode = fullscreen;
				D3D::SetFullscreenState(fullscreen);

				// Notify the host that it is safe to exit fullscreen
				if (!fullscreen)
				{
					Host_RequestFullscreen(false);
				}
			}
			// TODO: Aren't we still holding a reference to the back buffer right now?
			D3D::Reset();
			s_screenshot_texture.reset();
			s_backbuffer_width = D3D::GetBackBufferWidth();
			s_backbuffer_height = D3D::GetBackBufferHeight();
		}

		UpdateDrawRectangle(s_backbuffer_width, s_backbuffer_height);

		s_LastEFBScale = g_ActiveConfig.iEFBScale;
		CalculateTargetSize(s_backbuffer_width, s_backbuffer_height);

		D3D::context->OMSetRenderTargets(1, &D3D::GetBackBuffer()->GetRTV(), nullptr);

		delete g_framebuffer_manager;
		g_framebuffer_manager = new FramebufferManager;
		float clear_col[4] = { 0.f, 0.f, 0.f, 1.f };
		D3D::context->ClearRenderTargetView(FramebufferManager::GetEFBColorTexture()->GetRTV(), clear_col);
		D3D::context->ClearDepthStencilView(FramebufferManager::GetEFBDepthTexture()->GetDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}

	// begin next frame
	Renderer::RestoreAPIState();
	D3D::BeginFrame();
	D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTexture()->GetRTV(), FramebufferManager::GetEFBDepthTexture()->GetDSV());
	VertexShaderManager::SetViewportChanged();
}

// ALWAYS call RestoreAPIState for each ResetAPIState call you're doing
void Renderer::ResetAPIState()
{
	D3D::stateman->PushBlendState(resetblendstate.get());
	D3D::stateman->PushDepthState(resetdepthstate.get());
	D3D::stateman->PushRasterizerState(resetraststate.get());
}

void Renderer::RestoreAPIState()
{
	// Gets us back into a more game-like state.
	D3D::stateman->PopBlendState();
	D3D::stateman->PopDepthState();
	D3D::stateman->PopRasterizerState();
	VertexShaderManager::SetViewportChanged();
	BPFunctions::SetScissor();
}

void Renderer::ApplyState(bool bUseDstAlpha)
{
	gx_state.blend.use_dst_alpha = bUseDstAlpha;
	D3D::stateman->PushBlendState(gx_state_cache.Get(gx_state.blend));
	D3D::stateman->PushDepthState(gx_state_cache.Get(gx_state.zmode));
	D3D::stateman->PushRasterizerState(gx_state_cache.Get(gx_state.raster));

	for (unsigned int stage = 0; stage < 8; stage++)
	{
		// TODO: cache SamplerState directly, not d3d object
		gx_state.sampler[stage].max_anisotropy = g_ActiveConfig.iMaxAnisotropy;
		D3D::stateman->SetSampler(stage, gx_state_cache.Get(gx_state.sampler[stage]));
	}

	if (bUseDstAlpha)
	{
		// restore actual state
		SetBlendMode(false);
		SetLogicOpMode();
	}

	ID3D11Buffer* vertexConstants = VertexShaderCache::GetConstantBuffer();

	D3D::stateman->SetPixelConstants(PixelShaderCache::GetConstantBuffer(), g_ActiveConfig.bEnablePixelLighting ? vertexConstants : nullptr);
	D3D::stateman->SetVertexConstants(vertexConstants);

	D3D::stateman->SetPixelShader(PixelShaderCache::GetActiveShader());
	D3D::stateman->SetVertexShader(VertexShaderCache::GetActiveShader());
}

void Renderer::RestoreState()
{
	D3D::stateman->PopBlendState();
	D3D::stateman->PopDepthState();
	D3D::stateman->PopRasterizerState();
}

void Renderer::ApplyCullDisable()
{
	RasterizerState rast = gx_state.raster;
	rast.cull_mode = D3D11_CULL_NONE;

	ID3D11RasterizerState* raststate = gx_state_cache.Get(rast);
	D3D::stateman->PushRasterizerState(raststate);
}

void Renderer::RestoreCull()
{
	D3D::stateman->PopRasterizerState();
}

void Renderer::SetGenerationMode()
{
	const D3D11_CULL_MODE d3dCullModes[4] =
	{
		D3D11_CULL_NONE,
		D3D11_CULL_BACK,
		D3D11_CULL_FRONT,
		D3D11_CULL_BACK
	};

	// rastdc.FrontCounterClockwise must be false for this to work
	// TODO: GX_CULL_ALL not supported, yet!
	gx_state.raster.cull_mode = d3dCullModes[bpmem.genMode.cullmode];
}

void Renderer::SetDepthMode()
{
	gx_state.zmode = bpmem.zmode;
}

void Renderer::SetLogicOpMode()
{
	// D3D11 doesn't support logic blending, so this is a huge hack
	// TODO: Make use of D3D11.1's logic blending support

	// 0   0x00
	// 1   Source & destination
	// 2   Source & ~destination
	// 3   Source
	// 4   ~Source & destination
	// 5   Destination
	// 6   Source ^ destination =  Source & ~destination | ~Source & destination
	// 7   Source | destination
	// 8   ~(Source | destination)
	// 9   ~(Source ^ destination) = ~Source & ~destination | Source & destination
	// 10  ~Destination
	// 11  Source | ~destination
	// 12  ~Source
	// 13  ~Source | destination
	// 14  ~(Source & destination)
	// 15  0xff
	const D3D11_BLEND_OP d3dLogicOps[16] =
	{
		D3D11_BLEND_OP_ADD,//0
		D3D11_BLEND_OP_ADD,//1
		D3D11_BLEND_OP_SUBTRACT,//2
		D3D11_BLEND_OP_ADD,//3
		D3D11_BLEND_OP_REV_SUBTRACT,//4
		D3D11_BLEND_OP_ADD,//5
		D3D11_BLEND_OP_MAX,//6
		D3D11_BLEND_OP_ADD,//7
		D3D11_BLEND_OP_MAX,//8
		D3D11_BLEND_OP_MAX,//9
		D3D11_BLEND_OP_ADD,//10
		D3D11_BLEND_OP_ADD,//11
		D3D11_BLEND_OP_ADD,//12
		D3D11_BLEND_OP_ADD,//13
		D3D11_BLEND_OP_ADD,//14
		D3D11_BLEND_OP_ADD//15
	};
	const D3D11_BLEND d3dLogicOpSrcFactors[16] =
	{
		D3D11_BLEND_ZERO,//0
		D3D11_BLEND_DEST_COLOR,//1
		D3D11_BLEND_ONE,//2
		D3D11_BLEND_ONE,//3
		D3D11_BLEND_DEST_COLOR,//4
		D3D11_BLEND_ZERO,//5
		D3D11_BLEND_INV_DEST_COLOR,//6
		D3D11_BLEND_INV_DEST_COLOR,//7
		D3D11_BLEND_INV_SRC_COLOR,//8
		D3D11_BLEND_INV_SRC_COLOR,//9
		D3D11_BLEND_INV_DEST_COLOR,//10
		D3D11_BLEND_ONE,//11
		D3D11_BLEND_INV_SRC_COLOR,//12
		D3D11_BLEND_INV_SRC_COLOR,//13
		D3D11_BLEND_INV_DEST_COLOR,//14
		D3D11_BLEND_ONE//15
	};
	const D3D11_BLEND d3dLogicOpDestFactors[16] =
	{
		D3D11_BLEND_ZERO,//0
		D3D11_BLEND_ZERO,//1
		D3D11_BLEND_INV_SRC_COLOR,//2
		D3D11_BLEND_ZERO,//3
		D3D11_BLEND_ONE,//4
		D3D11_BLEND_ONE,//5
		D3D11_BLEND_INV_SRC_COLOR,//6
		D3D11_BLEND_ONE,//7
		D3D11_BLEND_INV_DEST_COLOR,//8
		D3D11_BLEND_SRC_COLOR,//9
		D3D11_BLEND_INV_DEST_COLOR,//10
		D3D11_BLEND_INV_DEST_COLOR,//11
		D3D11_BLEND_INV_SRC_COLOR,//12
		D3D11_BLEND_ONE,//13
		D3D11_BLEND_INV_SRC_COLOR,//14
		D3D11_BLEND_ONE//15
	};

	if (bpmem.blendmode.logicopenable && !bpmem.blendmode.blendenable)
	{
		gx_state.blend.blend_enable = true;
		gx_state.blend.blend_op = d3dLogicOps[bpmem.blendmode.logicmode];
		gx_state.blend.src_blend = d3dLogicOpSrcFactors[bpmem.blendmode.logicmode];
		gx_state.blend.dst_blend = d3dLogicOpDestFactors[bpmem.blendmode.logicmode];
	}
	else
	{
		SetBlendMode(true);
	}
}

void Renderer::SetDitherMode()
{
	// TODO: Set dither mode to bpmem.blendmode.dither
}

void Renderer::SetSamplerState(int stage, int texindex)
{
	const FourTexUnits &tex = bpmem.tex[texindex];
	const TexMode0 &tm0 = tex.texMode0[stage];
	const TexMode1 &tm1 = tex.texMode1[stage];

	if (texindex)
		stage += 4;

	if (g_ActiveConfig.bForceFiltering)
	{
		gx_state.sampler[stage].min_filter = 6; // 4 (linear mip) | 2 (linear min)
		gx_state.sampler[stage].mag_filter = 1; // linear mag
	}
	else
	{
		gx_state.sampler[stage].min_filter = (u32)tm0.min_filter;
		gx_state.sampler[stage].mag_filter = (u32)tm0.mag_filter;
	}

	gx_state.sampler[stage].wrap_s = (u32)tm0.wrap_s;
	gx_state.sampler[stage].wrap_t = (u32)tm0.wrap_t;
	gx_state.sampler[stage].max_lod = (u32)tm1.max_lod;
	gx_state.sampler[stage].min_lod = (u32)tm1.min_lod;
	gx_state.sampler[stage].lod_bias = (s32)tm0.lod_bias;
}

void Renderer::SetInterlacingMode()
{
	// TODO
}

int Renderer::GetMaxTextureSize()
{
	return DX11::D3D::GetMaxTextureSize();
}

u16 Renderer::BBoxRead(int index)
{
	// Here we get the min/max value of the truncated position of the upscaled framebuffer.
	// So we have to correct them to the unscaled EFB sizes.
	int value = BBox::Get(index);
	
	if (index < 2)
	{
		// left/right
		value = value * EFB_WIDTH / s_target_width;
	}
	else
	{
		// up/down
		value = value * EFB_HEIGHT / s_target_height;		
	}
	if (index & 1)
	value++; // fix max values to describe the outer border
	
	return value;
}

void Renderer::BBoxWrite(int index, u16 _value)
{
	int value = _value; // u16 isn't enough to multiply by the efb width
	if (index & 1)
		value--;
	if (index < 2)
	{
		value = value * s_target_width / EFB_WIDTH;
	}
	else
	{
		value = value * s_target_height / EFB_HEIGHT;
	}
	BBox::Set(index, value);
}

}  // namespace DX11
