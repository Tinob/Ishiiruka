// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "D3DBase.h"
#include "D3DTexture.h"

namespace DX11
{

namespace D3D
{

inline void CopyTextureData(u8 *pDst, const u8 *pSrc, const s32 width, const s32 height, const s32 srcpitch, const s32 dstpitch, const s32 pixelsize)
{
	const s32 rowsize = width * pixelsize;
	if (srcpitch == dstpitch && srcpitch == rowsize)
	{
		memcpy(pDst, pSrc, rowsize * height);
	}
	else
	{
		for (int y = 0; y < height; y++)
		{
			memcpy(pDst, pSrc, rowsize);
			pSrc += srcpitch;
			pDst += dstpitch;
		}
	}
}

inline void CopyCompressedTextureData(u8 *pDst, const u8 *pSrc, const s32 width, const s32 height, DXGI_FORMAT fmt, const s32 dstpitch)
{
	s32 numBlocksWide = (width + 3) >> 2;
	s32 numBlocksHigh = (height + 3) >> 2;
	s32 numBytesPerBlock = (fmt == DXGI_FORMAT_BC1_UNORM ? 8 : 16);
	s32 rowBytes = numBlocksWide * numBytesPerBlock;
	s32 numRows = numBlocksHigh;
	if (rowBytes == dstpitch)
	{
		memcpy(pDst, pSrc, rowBytes * numRows);
	}
	else
	{
		u8* pDestBits = pDst;
		const u8* pSrcBits = pSrc;
		// Copy stride line by line   
		for (s32 h = 0; h < numRows; h++)
		{
			memcpy(pDestBits, pSrcBits, rowBytes);
			pDestBits += dstpitch;
			pSrcBits += rowBytes;
		}
	}
}


void ReplaceRGBATexture2D(ID3D11Texture2D* pTexture, const u8* buffer, unsigned int width, unsigned int height, unsigned int pitch, unsigned int level, D3D11_USAGE usage, DXGI_FORMAT fmt)
{
	if (usage == D3D11_USAGE_DYNAMIC || usage == D3D11_USAGE_STAGING)
	{
		D3D11_MAPPED_SUBRESOURCE map;
		D3D::context->Map(pTexture, level, D3D11_MAP_WRITE_DISCARD, 0, &map);
		if (fmt == DXGI_FORMAT_BC1_UNORM || fmt == DXGI_FORMAT_BC2_UNORM || fmt == DXGI_FORMAT_BC3_UNORM)
		{
			CopyCompressedTextureData((u8*)map.pData, buffer, width, height, fmt, map.RowPitch);
		}
		else
		{
			CopyTextureData((u8*)map.pData, buffer, width, height, 4 * pitch, map.RowPitch, 4);
		}
		D3D::context->Unmap(pTexture, level);
	}
	else
	{
		D3D11_BOX dest_region = CD3D11_BOX(0, 0, 0, width, height, 1);
		if (fmt == DXGI_FORMAT_BC1_UNORM || fmt == DXGI_FORMAT_BC2_UNORM || fmt == DXGI_FORMAT_BC3_UNORM)
		{
			s32 numBlocksWide = (width + 3) >> 2;
			s32 numBytesPerBlock = (fmt == DXGI_FORMAT_BC1_UNORM ? 8 : 16);
			s32 rowBytes = numBlocksWide * numBytesPerBlock;			
			D3D::context->UpdateSubresource(pTexture, level, &dest_region, buffer, rowBytes, 0);
		}
		else
		{
			D3D11_BOX dest_region = CD3D11_BOX(0, 0, 0, width, height, 1);
			D3D::context->UpdateSubresource(pTexture, level, &dest_region, buffer, 4 * pitch, 4 * pitch*height);
		}
	}
}

}  // namespace

D3DTexture2D* D3DTexture2D::Create(unsigned int width, unsigned int height, D3D11_BIND_FLAG bind, D3D11_USAGE usage, DXGI_FORMAT fmt, unsigned int levels)
{
	ID3D11Texture2D* pTexture = NULL;
	HRESULT hr;

	D3D11_CPU_ACCESS_FLAG cpuflags;
	if (usage == D3D11_USAGE_STAGING) cpuflags = (D3D11_CPU_ACCESS_FLAG)((int)D3D11_CPU_ACCESS_WRITE|(int)D3D11_CPU_ACCESS_READ);
	else if (usage == D3D11_USAGE_DYNAMIC) cpuflags = D3D11_CPU_ACCESS_WRITE;
	else cpuflags = (D3D11_CPU_ACCESS_FLAG)0;
	D3D11_TEXTURE2D_DESC texdesc = CD3D11_TEXTURE2D_DESC(fmt, width, height, 1, levels, bind, usage, cpuflags);
	hr = D3D::device->CreateTexture2D(&texdesc, NULL, &pTexture);
	if (FAILED(hr))
	{
		PanicAlert("Failed to create texture at %s, line %d: hr=%#x\n", __FILE__, __LINE__, hr);
		return NULL;
	}

	D3DTexture2D* ret = new D3DTexture2D(pTexture, bind);
	SAFE_RELEASE(pTexture);
	return ret;
}

void D3DTexture2D::AddRef()
{
	++ref;
}

UINT D3DTexture2D::Release()
{
	--ref;
	if (ref == 0)
	{
		delete this;
		return 0;
	}
	return ref;
}

ID3D11Texture2D* &D3DTexture2D::GetTex() { return tex; }
ID3D11ShaderResourceView* &D3DTexture2D::GetSRV() { return srv; }
ID3D11RenderTargetView* &D3DTexture2D::GetRTV() { return rtv; }
ID3D11DepthStencilView* &D3DTexture2D::GetDSV() { return dsv; }

D3DTexture2D::D3DTexture2D(ID3D11Texture2D* texptr, D3D11_BIND_FLAG bind,
							DXGI_FORMAT srv_format, DXGI_FORMAT dsv_format, DXGI_FORMAT rtv_format, bool multisampled)
							: ref(1), tex(texptr), srv(NULL), rtv(NULL), dsv(NULL)
{
	D3D11_SRV_DIMENSION srv_dim = multisampled ? D3D11_SRV_DIMENSION_TEXTURE2DMS : D3D11_SRV_DIMENSION_TEXTURE2D;
	D3D11_DSV_DIMENSION dsv_dim = multisampled ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	D3D11_RTV_DIMENSION rtv_dim = multisampled ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = CD3D11_SHADER_RESOURCE_VIEW_DESC(srv_dim, srv_format);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = CD3D11_DEPTH_STENCIL_VIEW_DESC(dsv_dim, dsv_format);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = CD3D11_RENDER_TARGET_VIEW_DESC(rtv_dim, rtv_format);
	if (bind & D3D11_BIND_SHADER_RESOURCE) D3D::device->CreateShaderResourceView(tex, &srv_desc, &srv);
	if (bind & D3D11_BIND_RENDER_TARGET) D3D::device->CreateRenderTargetView(tex, &rtv_desc, &rtv);
	if (bind & D3D11_BIND_DEPTH_STENCIL) D3D::device->CreateDepthStencilView(tex, &dsv_desc, &dsv);
	tex->AddRef();
}

D3DTexture2D::~D3DTexture2D()
{
	SAFE_RELEASE(srv);
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(dsv);
	SAFE_RELEASE(tex);
}

}  // namespace DX11