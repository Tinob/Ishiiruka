// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma	once
#include <string>
#include <d3d11_2.h>
#include "Common/MathUtil.h"
#include "VideoBackends/DX11/D3DPtr.h"

namespace DX11
{

namespace D3D
{
// Font creation flags
#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002

// Font rendering flags
#define D3DFONT_CENTERED    0x0001

class CD3DFont
{
	SrvPtr m_pTexture;
	BufferPtr m_pVB;
	InputLayoutPtr m_InputLayout;
	PixelShaderPtr m_pshader;
	VertexShaderPtr m_vshader;
	BlendStatePtr m_blendstate;
	RasterizerStatePtr m_raststate;
	const int m_dwTexWidth;
	const int m_dwTexHeight;
	unsigned int m_LineHeight;
	float m_fTexCoords[128 - 32][4];

public:
	CD3DFont();
	// 2D text drawing function
	// Initializing and destroying device-dependent objects
	int Init();
	int Shutdown();
	int DrawTextScaled(float x, float y,
		float size,
		float spacing, u32 dwColor,
		const std::string& strText, float scalex, float scaley);
};

// Ring buffer class, shared between the draw* functions
class UtilVertexBuffer
{
public:
	UtilVertexBuffer(int size);
	~UtilVertexBuffer();

	// returns vertex offset to the new data
	int AppendData(void* data, int size, int vertex_size);

	void AddWrapObserver(bool* observer);

	inline ID3D11Buffer* &GetBuffer() { return buf; }

private:
	ID3D11Buffer* buf;
	int offset;
	int max_size;

	std::list<bool*> observers;
};

// Ring Constant buffer class, only works as a ring if
// platform supports it
class ConstantStreamBuffer
{
public:
	ConstantStreamBuffer(int size);
	~ConstantStreamBuffer();

	// returns vertex offset to the new data
	int AppendData(void* data, int size);

	inline ID3D11Buffer* &GetBuffer() { return  buf; }

private:
	ID3D11Buffer* buf;
	int offset;
	int max_size;
	bool m_use_partial_buffer_update;
	bool m_need_init;
};

extern CD3DFont font;

void InitUtils();
void ShutdownUtils();

void SetPointCopySampler();
void SetLinearCopySampler();

ID3D11SamplerState* GetPointCopySampler();

ID3D11SamplerState* GetLinearCopySampler();

void drawShadedTexQuad(ID3D11ShaderResourceView* texture,
	const D3D11_RECT* rSource,
	int SourceWidth,
	int SourceHeight,	
	ID3D11PixelShader* PShader,
	ID3D11VertexShader* VShader,
	ID3D11InputLayout* layout,
	ID3D11GeometryShader* GShader = nullptr,
	float Gamma = 1.0f,
	u32 slice = 0,
	int DestWidth = 1,
	int DestHeight = 1);
void drawClearQuad(u32 Color, float z);
void drawColorQuad(u32 Color, float x1, float y1, float x2, float y2);
}

}