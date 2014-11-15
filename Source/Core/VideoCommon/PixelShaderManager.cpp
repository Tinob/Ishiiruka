// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cmath>

#include "Common/Common.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"

#include "VideoCommon/RenderBase.h"

GC_ALIGNED16(float PixelShaderManager::psconstants[PixelShaderManager::ConstantBufferSize]);
ConstatBuffer PixelShaderManager::m_buffer(PixelShaderManager::psconstants, PixelShaderManager::ConstantBufferSize);
static int s_nColorsChanged[2]; // 0 - regular colors, 1 - k colors
static int s_nIndTexMtxChanged;
static bool s_bAlphaChanged;
static bool s_bZBiasChanged;
static bool s_bZTextureTypeChanged;
static bool s_bDepthRangeChanged;
static bool s_bFogColorChanged;
static bool s_bFogParamChanged;
static bool s_bFogRangeAdjustChanged;
static int nLightsChanged[2]; // min,max
static int lastRGBAfull[2][4][4];
static u8 s_nTexDimsChanged;
static u8 s_nIndTexScaleChanged;
static u32 lastAlpha;
static u32 lastTexDims[8]; // width | height << 16 | wrap_s << 28 | wrap_t << 30
static u32 lastZBias;
static int nMaterialsChanged;
const float U8_NORM_COEF = 1 / 255.0f;
const float U24_NORM_COEF = 1 / 16777216.0f;


void PixelShaderManager::Init()
{
	m_buffer.Clear();
	lastAlpha = 0;
	memset(lastTexDims, 0, sizeof(lastTexDims));
	lastZBias = 0;
	memset(lastRGBAfull, 0, sizeof(lastRGBAfull));
	Dirty();
}

void PixelShaderManager::Dirty()
{
	s_nColorsChanged[0] = s_nColorsChanged[1] = 15;
	s_nTexDimsChanged = 0xFF;
	s_nIndTexScaleChanged = 0xFF;
	s_nIndTexMtxChanged = 15;
	s_bAlphaChanged = s_bZBiasChanged = s_bZTextureTypeChanged = s_bDepthRangeChanged = true;
	s_bFogRangeAdjustChanged = s_bFogColorChanged = s_bFogParamChanged = true;
	nLightsChanged[0] = 0; nLightsChanged[1] = 0x80;
	nMaterialsChanged = 15;
}

void PixelShaderManager::Shutdown()
{
	
}

const float* PixelShaderManager::GetBuffer()
{
	return psconstants;
}

float* PixelShaderManager::GetBufferToUpdate(u32 const_number, u32 size)
{
	return m_buffer.GetBufferToUpdate<float>(const_number, size);
}

bool PixelShaderManager::IsDirty()
{
	return m_buffer.IsDirty();
}

const regionvector &PixelShaderManager::GetDirtyRegions()
{
	return m_buffer.GetRegions();
}

void PixelShaderManager::Clear()
{
	m_buffer.Clear();
}

void PixelShaderManager::EnableDirtyRegions()
{
	m_buffer.EnableDirtyRegions();
}
void PixelShaderManager::DisableDirtyRegions()
{
	m_buffer.DisableDirtyRegions();
}

void PixelShaderManager::SetConstants()
{
	if (g_ActiveConfig.backend_info.APIType == API_OPENGL && !g_ActiveConfig.backend_info.bSupportsGLSLUBO)
		Dirty();

	for (int i = 0; i < 2; ++i)
	{
		if (s_nColorsChanged[i])
		{
			int baseind = i ? C_KCOLORS : C_COLORS;
			for (int j = 0; j < 4; ++j)
			{
				if ((s_nColorsChanged[i] & (1 << j)))
				{
					int* src = &lastRGBAfull[i][j][0];
					m_buffer.SetConstant4<float>(baseind + j, (float)src[0], (float)src[1], (float)src[2], (float)src[3]);
					s_nColorsChanged[i] &= ~(1 << j);
				}
			}
		}
	}

	if (s_nTexDimsChanged)
	{
		for (int i = 0; i < 8; ++i)
		{
			if ((s_nTexDimsChanged & (1 << i)))
			{
				SetPSTextureDims(i);
				s_nTexDimsChanged &= ~(1 << i);
			}
		}
	}

	if (s_bAlphaChanged)
	{
		m_buffer.SetConstant4<float>(C_ALPHA, ((float)(lastAlpha & 0xff)), ((float)((lastAlpha >> 8) & 0xff)), 0.0f, ((float)((lastAlpha >> 16) & 0xff)));
		s_bAlphaChanged = false;
	}

	if (s_bZTextureTypeChanged)
	{
		float *ftemp = m_buffer.GetBufferToUpdate<float>(C_ZBIAS, 1);
		switch (bpmem.ztex2.type)
		{
		case 0:
			// 8 bits
			ftemp[0] = 0; ftemp[1] = 0; ftemp[2] = 0; ftemp[3] = 255.0f / 16777215.0f;
			break;
		case 1:
			// 16 bits
			ftemp[0] = 255.0f / 16777215.0f; ftemp[1] = 0; ftemp[2] = 0; ftemp[3] = 65280.0f / 16777215.0f;
			break;
		case 2:
			// 24 bits
			ftemp[0] = 16711680.0f / 16777215.0f; ftemp[1] = 65280.0f / 16777215.0f; ftemp[2] = 255.0f / 16777215.0f; ftemp[3] = 0;
			break;
		}
		s_bZTextureTypeChanged = false;
	}

	if (s_bZBiasChanged || s_bDepthRangeChanged)
	{
		// reversed gxsetviewport(xorig, yorig, width, height, nearz, farz)
		// [0] = width/2
		// [1] = height/2
		// [2] = 16777215 * (farz - nearz)
		// [3] = xorig + width/2 + 342
		// [4] = yorig + height/2 + 342
		// [5] = 16777215 * farz

		//ERROR_LOG("pixel=%x,%x, bias=%x\n", bpmem.zcontrol.pixel_format, bpmem.ztex2.type, lastZBias);
		m_buffer.SetConstant4<float>(C_ZBIAS + 1, xfmem.viewport.farZ / 16777216.0f, xfmem.viewport.zRange / 16777216.0f, 0.0f, (float)(lastZBias) / 16777215.0f);
		s_bZBiasChanged = s_bDepthRangeChanged = false;
	}

	// indirect incoming texture scales
	if (s_nIndTexScaleChanged)
	{
		// set as two sets of vec4s, each containing S and T of two ind stages.
		if (s_nIndTexScaleChanged & 0x03)
		{
			float *f = m_buffer.GetBufferToUpdate<float>(C_INDTEXSCALE, 1);
			f[0] = (float)bpmem.texscale[0].ss0;
			f[1] = (float)bpmem.texscale[0].ts0;
			f[2] = (float)bpmem.texscale[0].ss1;
			f[3] = (float)bpmem.texscale[0].ts1;
		}

		if (s_nIndTexScaleChanged & 0x0c)
		{
			float *f = m_buffer.GetBufferToUpdate<float>(C_INDTEXSCALE + 1, 1);
			f[0] = (float)bpmem.texscale[1].ss0;
			f[1] = (float)bpmem.texscale[1].ts0;
			f[2] = (float)bpmem.texscale[1].ss1;
			f[3] = (float)bpmem.texscale[1].ts1;
		}
		s_nIndTexScaleChanged = 0;
	}

	if (s_nIndTexMtxChanged)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (s_nIndTexMtxChanged & (1 << i))
			{
				int scale = ((u32)bpmem.indmtx[i].col0.s0 << 0) |
					((u32)bpmem.indmtx[i].col1.s1 << 2) |
					((u32)bpmem.indmtx[i].col2.s2 << 4);
				scale = 17 - scale;
				m_buffer.SetConstant4<float>(C_INDTEXMTX + 2 * i,
					(float)bpmem.indmtx[i].col0.ma,
					(float)bpmem.indmtx[i].col1.mc,
					(float)bpmem.indmtx[i].col2.me,
					(float)scale);
				m_buffer.SetConstant4<float>(C_INDTEXMTX + 2 * i + 1,
					(float)bpmem.indmtx[i].col0.mb,
					(float)bpmem.indmtx[i].col1.md,
					(float)bpmem.indmtx[i].col2.mf,
					(float)scale);

				PRIM_LOG("indmtx%d: scale=%f, mat=(%f %f %f; %f %f %f)\n",
					i, scale,
					bpmem.indmtx[i].col0.ma, bpmem.indmtx[i].col1.mc, bpmem.indmtx[i].col2.me,
					bpmem.indmtx[i].col0.mb, bpmem.indmtx[i].col1.md, bpmem.indmtx[i].col2.mf);

				s_nIndTexMtxChanged &= ~(1 << i);
			}
		}
	}

	if (s_bFogColorChanged)
	{
		m_buffer.SetConstant4<float>(C_FOG, (float)bpmem.fog.color.r, (float)bpmem.fog.color.g, (float)bpmem.fog.color.b, 0.0f);
		s_bFogColorChanged = false;
	}

	if (s_bFogParamChanged)
	{
		if (!g_ActiveConfig.bDisableFog)
		{
			//downscale magnitude to 0.24 bits
			float b = (float)bpmem.fog.b_magnitude / 0xFFFFFF;

			float b_shf = (1.0f / (float)(1 << bpmem.fog.b_shift));
			m_buffer.SetConstant4<float>(C_FOG + 1, bpmem.fog.a.GetA(), b, bpmem.fog.c_proj_fsel.GetC(), b_shf);
		}
		else
			m_buffer.SetConstant4<float>(C_FOG + 1, 0.0f, 1.0f, 0.0f, 1.0f);

		s_bFogParamChanged = false;
	}

	if (s_bFogRangeAdjustChanged)
	{
		if (!g_ActiveConfig.bDisableFog && bpmem.fogRange.Base.Enabled == 1)
		{
			//bpmem.fogRange.Base.Center : center of the viewport in x axis. observation: bpmem.fogRange.Base.Center = realcenter + 342;
			int center = ((u32)bpmem.fogRange.Base.Center) - 342;
			// normalize center to make calculations easy
			float ScreenSpaceCenter = center / (2.0f * xfmem.viewport.wd);
			ScreenSpaceCenter = (ScreenSpaceCenter * 2.0f) - 1.0f;
			//bpmem.fogRange.K seems to be  a table of precalculated coefficients for the adjust factor
			//observations: bpmem.fogRange.K[0].LO appears to be the lowest value and bpmem.fogRange.K[4].HI the largest
			// they always seems to be larger than 256 so my theory is :
			// they are the coefficients from the center to the border of the screen
			// so to simplify I use the hi coefficient as K in the shader taking 256 as the scale
			m_buffer.SetConstant4<float>(C_FOG + 2, ScreenSpaceCenter, (float)Renderer::EFBToScaledX((int)(2.0f * xfmem.viewport.wd)), bpmem.fogRange.K[4].HI / 256.0f, 0.0f);
		}
		else
		{
			m_buffer.SetConstant4<float>(C_FOG + 2, 0.0f, 1.0f, 1.0f, 0.0f); // Need to update these values for older hardware that fails to divide by zero in shaders.
		}

		s_bFogRangeAdjustChanged = false;
	}

	if (g_ActiveConfig.bEnablePixelLighting 
		&& g_ActiveConfig.backend_info.bSupportsPixelLighting
		&& xfmem.numChan.numColorChans > 0)
	{
		if (nLightsChanged[0] >= 0)
		{
			// lights don't have a 1 to 1 mapping, the color component needs to be converted to 4 floats
			int istart = nLightsChanged[0] / 0x10;
			int iend = (nLightsChanged[1] + 15) / 0x10;

			for (int i = istart; i < iend; ++i)
			{
				const Light& light = xfmem.lights[i];
				
				m_buffer.SetConstant4<float>(C_PLIGHTS + 5 * i,
					light.color[3] * U8_NORM_COEF,
					light.color[2] * U8_NORM_COEF,
					light.color[1] * U8_NORM_COEF,
					light.color[0] * U8_NORM_COEF);
				m_buffer.SetConstant3v(C_PLIGHTS + 5 * i + 1, light.cosatt);
				if (fabs(light.distatt[0]) < 0.00001f &&
					fabs(light.distatt[1]) < 0.00001f &&
					fabs(light.distatt[2]) < 0.00001f)
				{
					// dist attenuation, make sure not equal to 0!!!
					m_buffer.SetConstant4(C_PLIGHTS + 5 * i + 2, 0.00001f, light.distatt[1], light.distatt[2], 0.0f);
				}
				else
				{
					m_buffer.SetConstant3v(C_PLIGHTS + 5 * i + 2, light.distatt);
				}
				m_buffer.SetConstant3v(C_PLIGHTS + 5 * i + 3, light.dpos);
				double norm = double(light.ddir[0]) * double(light.ddir[0]) +
					double(light.ddir[1]) * double(light.ddir[1]) +
					double(light.ddir[2]) * double(light.ddir[2]);
				norm = 1.0 / sqrt(norm);
				float norm_float = static_cast<float>(norm);
				m_buffer.SetConstant4(C_PLIGHTS + 5 * i + 4, light.ddir[0] * norm_float, light.ddir[1] * norm_float, light.ddir[2] * norm_float, 0.0f);
			}

			nLightsChanged[0] = nLightsChanged[1] = -1;
		}

		if (nMaterialsChanged)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (nMaterialsChanged & (1 << i))
				{
					u32 data = *(xfmem.ambColor + i);
					float* material = m_buffer.GetBufferToUpdate<float>(C_PMATERIALS + i, 1);
					material[0] = ((data >> 24) & 0xFF) * U8_NORM_COEF;
					material[1] = ((data >> 16) & 0xFF) * U8_NORM_COEF;
					material[2] = ((data >> 8) & 0xFF) * U8_NORM_COEF;
					material[3] = (data & 0xFF) * U8_NORM_COEF;
				}
			}

			for (int i = 0; i < 2; ++i)
			{
				if (nMaterialsChanged & (1 << (i + 2)))
				{
					u32 data = *(xfmem.matColor + i);
					float* material = m_buffer.GetBufferToUpdate<float>(C_PMATERIALS + i + 2, 1);
					material[0] = ((data >> 24) & 0xFF) * U8_NORM_COEF;
					material[1] = ((data >> 16) & 0xFF) * U8_NORM_COEF;
					material[2] = ((data >> 8) & 0xFF) * U8_NORM_COEF;
					material[3] = (data & 0xFF) * U8_NORM_COEF;
				}
			}
			nMaterialsChanged = 0;
		}
	}
}

void PixelShaderManager::SetPSTextureDims(int texid)
{
	// texdims.xy are reciprocals of the real texture dimensions
	// texdims.zw are the scaled dimensions
	float* fdims = m_buffer.GetBufferToUpdate<float>(C_TEXDIMS + texid, 1);
	TCoordInfo& tc = bpmem.texcoords[texid];
	fdims[0] = 1.0f / (float)(lastTexDims[texid] & 0xffff);
	fdims[1] = 1.0f / (float)((lastTexDims[texid] >> 16) & 0xfff);
	fdims[2] = (float)(tc.s.scale_minus_1 + 1);
	fdims[3] = (float)(tc.t.scale_minus_1 + 1);	
	PRIM_LOG("texdims%d: %f %f %f %f\n", texid, fdims[0], fdims[1], fdims[2], fdims[3]);
}

void PixelShaderManager::SetTevColor(int index, int component, s32 value)
{
	s32 *c = &lastRGBAfull[0][index][0];
	if (c[component] != value)
	{
		c[component] = value;
		s_nColorsChanged[0] |= 1 << index;
		PRIM_LOG("tev color%d: %d %d %d %d\n", index, c[0], c[1], c[2], c[3]);
	}
}

void PixelShaderManager::SetTevKonstColor(int index, int component, s32 value)
{
	s32 *c = &lastRGBAfull[1][index][0];
	if (c[component] != value)
	{
		c[component] = value;
		s_nColorsChanged[1] |= 1 << index;
		PRIM_LOG("tev konst color%d: %d %d %d %d\n", index, c[0], c[1], c[2], c[3]);
	}
}
/*
// This one is high in profiles (0.5%).
// TODO: Move conversion out, only store the raw color value
// and update it when the shader constant is set, only.
// TODO: Conversion should be checked in the context of tev_fixes..
void PixelShaderManager::SetColorChanged(int type, int num, bool high)
{
	int *pf = &lastRGBAfull[type][num][0];
	if (!high)
	{
		if (pf[0] != bpmem.tevregs[num].low.a
		|| pf[3] != bpmem.tevregs[num].low.b)
		{
			pf[0] = bpmem.tevregs[num].low.a;
			pf[3] = bpmem.tevregs[num].low.b;
			s_nColorsChanged[type] |= 1 << num;
		}
		
	}
	else
	{
		if (pf[1] != bpmem.tevregs[num].high.b
		|| pf[2] != bpmem.tevregs[num].high.a)
		{
			pf[1] = bpmem.tevregs[num].high.b;
			pf[2] = bpmem.tevregs[num].high.a;
			s_nColorsChanged[type] |= 1 << num;
		}
	}
	PRIM_LOG("pixel %scolor%d: %f %f %f %f\n", type ? "k" : "", num, pf[0], pf[1], pf[2], pf[3]);
}
*/
void PixelShaderManager::SetAlpha()
{
	if ((bpmem.alpha_test.hex & 0xffff) != lastAlpha)
	{
		lastAlpha = (lastAlpha & ~0xffff) | (bpmem.alpha_test.hex & 0xffff);
		s_bAlphaChanged = true;
	}
}

void PixelShaderManager::SetDestAlpha()
{
	if (bpmem.dstalpha.alpha != (lastAlpha >> 16))
	{
		lastAlpha = (lastAlpha & ~0xff0000) | ((bpmem.dstalpha.alpha & 0xff) << 16);
		s_bAlphaChanged = true;
	}
}

void PixelShaderManager::SetTexDims(int texmapid, u32 width, u32 height, u32 wraps, u32 wrapt)
{
	u32 wh = width | (height << 16) | (wraps << 28) | (wrapt << 30);
	if (lastTexDims[texmapid] != wh)
	{
		lastTexDims[texmapid] = wh;
		s_nTexDimsChanged |= 1 << texmapid;
	}
}

void PixelShaderManager::SetZTextureBias()
{
	if (lastZBias != bpmem.ztex1.bias)
	{
		s_bZBiasChanged = true;
		lastZBias = bpmem.ztex1.bias;
	}
}

void PixelShaderManager::SetViewportChanged()
{
	s_bDepthRangeChanged = true;
	s_bFogRangeAdjustChanged = true; // TODO: Shouldn't be necessary with an accurate fog range adjust implementation
}

void PixelShaderManager::SetIndTexScaleChanged(bool high)
{
	s_nIndTexScaleChanged |= high ? 0x0c : 0x03;
}

void PixelShaderManager::SetIndMatrixChanged(int matrixidx)
{
	s_nIndTexMtxChanged |= 1 << matrixidx;
}

void PixelShaderManager::SetZTextureTypeChanged()
{
	s_bZTextureTypeChanged = true;
}

void PixelShaderManager::SetTexCoordChanged(u8 texmapid)
{
	s_nTexDimsChanged |= 1 << texmapid;
}

void PixelShaderManager::SetFogColorChanged()
{
	s_bFogColorChanged = true;
}

void PixelShaderManager::SetFogParamChanged()
{
	s_bFogParamChanged = true;
}

void PixelShaderManager::SetFogRangeAdjustChanged()
{
	s_bFogRangeAdjustChanged = true;
}

void PixelShaderManager::SetColorMatrix(const float* pmatrix)
{
	m_buffer.SetMultiConstant4v<float>(C_COLORMATRIX, 7, pmatrix);
	s_nColorsChanged[0] = s_nColorsChanged[1] = 15;
}

void PixelShaderManager::InvalidateXFRange(int start, int end)
{
	if (start < XFMEM_LIGHTS_END && end > XFMEM_LIGHTS)
	{
		int _start = start < XFMEM_LIGHTS ? XFMEM_LIGHTS : start - XFMEM_LIGHTS;
		int _end = end < XFMEM_LIGHTS_END ? end - XFMEM_LIGHTS : XFMEM_LIGHTS_END - XFMEM_LIGHTS;

		if (nLightsChanged[0] == -1)
		{
			nLightsChanged[0] = _start;
			nLightsChanged[1] = _end;
		}
		else
		{
			if (nLightsChanged[0] > _start) nLightsChanged[0] = _start;
			if (nLightsChanged[1] < _end)   nLightsChanged[1] = _end;
		}
	}
}

void PixelShaderManager::SetMaterialColorChanged(int index)
{
	nMaterialsChanged |= (1 << index);
}

void PixelShaderManager::DoState(PointerWrap &p)
{
	p.Do(lastRGBAfull);
	p.Do(lastAlpha);
	p.Do(lastTexDims);
	p.Do(lastZBias);

	if (p.GetMode() == PointerWrap::MODE_READ)
	{
		Dirty();
	}
}
