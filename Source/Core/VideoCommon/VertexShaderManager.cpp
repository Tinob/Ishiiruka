// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Common.h"
#include "VideoCommon/VideoConfig.h"
#include "Common/MathUtil.h"

#include <cmath>
#include <sstream>

#include "VideoCommon/Statistics.h"

#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/BPMemory.h"
#include "VideoCommon/CPMemory.h"
#include "VideoCommon/XFMemory.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VertexManagerBase.h"

#include "VideoCommon/RenderBase.h"

GC_ALIGNED16(float VertexShaderManager::vsconstants[VertexShaderManager::ConstantBufferSize]);
ConstatBuffer VertexShaderManager::m_buffer(VertexShaderManager::vsconstants, VertexShaderManager::ConstantBufferSize);

float GC_ALIGNED16(g_fProjectionMatrix[16]);

// track changes
static bool bTexMatricesChanged[2], bPosNormalMatrixChanged, bProjectionChanged, bViewportChanged;
static int nMaterialsChanged;
static int nTransformMatricesChanged[2]; // min,max
static int nNormalMatricesChanged[2]; // min,max
static int nPostTransformMatricesChanged[2]; // min,max
static int nLightsChanged[2]; // min,max

static Matrix44 s_viewportCorrection;
static Matrix33 s_viewRotationMatrix;
static Matrix33 s_viewInvRotationMatrix;
static float s_fViewTranslationVector[3];
static float s_fViewRotation[2];
const float U8_NORM_COEF = 1 / 255.0f;
const float U24_NORM_COEF = 1 / 16777216.0f;

void UpdateViewport(Matrix44& vpCorrection);

void UpdateViewportWithCorrection()
{
	UpdateViewport(s_viewportCorrection);
}

struct ProjectionHack
{
	float sign;
	float value;
	ProjectionHack() { }
	ProjectionHack(float new_sign, float new_value)
		: sign(new_sign), value(new_value) {}
};

namespace
{
// Control Variables
static ProjectionHack g_ProjHack1;
static ProjectionHack g_ProjHack2;
static bool g_ProjHack3;
} // Namespace

float PHackValue(std::string sValue)
{
	float f = 0;
	bool fp = false;
	const char *cStr = sValue.c_str();
	char *c = new char[strlen(cStr)+1];
	std::istringstream sTof("");

	for (unsigned int i=0; i<=strlen(cStr); ++i)
	{
		if (i == 20)
		{
			c[i] = '\0';
			break;
		}

		c[i] = (cStr[i] == ',') ? '.' : *(cStr+i);
		if (c[i] == '.')
			fp = true;
	}

	cStr = c;
	sTof.str(cStr);
	sTof >> f;

	if (!fp)
		f /= 0xF4240;

	delete [] c;
	return f;
}

void UpdateProjectionHack(int iPhackvalue[], std::string sPhackvalue[])
{
	float fhackvalue1 = 0, fhackvalue2 = 0;
	float fhacksign1 = 1.0, fhacksign2 = 1.0;
	bool bProjHack3 = false;
	const char *sTemp[2];

	if (iPhackvalue[0] == 1)
	{
		NOTICE_LOG(VIDEO, "\t\t--- Orthographic Projection Hack ON ---");

		fhacksign1 *= (iPhackvalue[1] == 1) ? -1.0f : fhacksign1;
		sTemp[0] = (iPhackvalue[1] == 1) ? " * (-1)" : "";
		fhacksign2 *= (iPhackvalue[2] == 1) ? -1.0f : fhacksign2;
		sTemp[1] = (iPhackvalue[2] == 1) ? " * (-1)" : "";

		fhackvalue1 = PHackValue(sPhackvalue[0]);
		NOTICE_LOG(VIDEO, "- zNear Correction = (%f + zNear)%s", fhackvalue1, sTemp[0]);

		fhackvalue2 = PHackValue(sPhackvalue[1]);
		NOTICE_LOG(VIDEO, "- zFar Correction =  (%f + zFar)%s", fhackvalue2, sTemp[1]);

		sTemp[0] = "DISABLED";
		bProjHack3 = (iPhackvalue[3] == 1) ? true : bProjHack3;
		if (bProjHack3)
			sTemp[0] = "ENABLED";
		NOTICE_LOG(VIDEO, "- Extra Parameter: %s", sTemp[0]);
	}

	// Set the projections hacks
	g_ProjHack1 = ProjectionHack(fhacksign1, fhackvalue1);
	g_ProjHack2 = ProjectionHack(fhacksign2, fhackvalue2);
	g_ProjHack3 = bProjHack3;
}

void VertexShaderManager::Init()
{
	Dirty();
	m_buffer.Clear();
	memset(&xfmem, 0, sizeof(xfmem));
	ResetView();

	// TODO: should these go inside ResetView()?
	Matrix44::LoadIdentity(s_viewportCorrection);
	memset(g_fProjectionMatrix, 0, sizeof(g_fProjectionMatrix));
	for (int i = 0; i < 4; ++i)
		g_fProjectionMatrix[i*5] = 1.0f;
}

void VertexShaderManager::Shutdown()
{
	
}
const float* VertexShaderManager::GetBuffer()
{
	return vsconstants;
}

float* VertexShaderManager::GetBufferToUpdate(u32 const_number, u32 size)
{
	return m_buffer.GetBufferToUpdate<float>(const_number, size);
}

bool VertexShaderManager::IsDirty()
{
	return m_buffer.IsDirty();
}

const regionvector &VertexShaderManager::GetDirtyRegions()
{
	return m_buffer.GetRegions();
}

void VertexShaderManager::Clear()
{
	m_buffer.Clear();
}

void VertexShaderManager::EnableDirtyRegions()
{
	m_buffer.EnableDirtyRegions();
}
void VertexShaderManager::DisableDirtyRegions()
{
	m_buffer.DisableDirtyRegions();
}

void VertexShaderManager::Dirty()
{
	nTransformMatricesChanged[0] = 0; 
	nTransformMatricesChanged[1] = 256;

	nNormalMatricesChanged[0] = 0;
	nNormalMatricesChanged[1] = 96;

	nPostTransformMatricesChanged[0] = 0; 
	nPostTransformMatricesChanged[1] = 256;

	nLightsChanged[0] = 0; 
	nLightsChanged[1] = 0x80;

	bPosNormalMatrixChanged = true;
	bTexMatricesChanged[0] = true;
	bTexMatricesChanged[1] = true;

	bProjectionChanged = true;

	nMaterialsChanged = 15;
}

// Syncs the shader constant buffers with xfmem
// TODO: A cleaner way to control the matrices without making a mess in the parameters field
void VertexShaderManager::SetConstants()
{
	if (g_ActiveConfig.backend_info.APIType == API_OPENGL && !g_ActiveConfig.backend_info.bSupportsGLSLUBO)
		Dirty();
	
	if (nTransformMatricesChanged[0] >= 0)
	{
		int startn = nTransformMatricesChanged[0] / 4;
		int endn = (nTransformMatricesChanged[1] + 3) / 4;
		const float* pstart = (const float*)&xfmem.posMatrices[startn * 4];
		m_buffer.SetMultiConstant4v(C_TRANSFORMMATRICES + startn, endn - startn, pstart);
		nTransformMatricesChanged[0] = nTransformMatricesChanged[1] = -1;
	}

	if (nNormalMatricesChanged[0] >= 0)
	{
		int startn = nNormalMatricesChanged[0] / 3;
		int endn = (nNormalMatricesChanged[1] + 2) / 3;
		const float *pnstart = (const float*)&xfmem.normalMatrices[3 * startn];
		m_buffer.SetMultiConstant3v(C_NORMALMATRICES + startn, endn - startn, pnstart);
		nNormalMatricesChanged[0] = nNormalMatricesChanged[1] = -1;
	}

	if (nPostTransformMatricesChanged[0] >= 0)
	{
		int startn = nPostTransformMatricesChanged[0] / 4;
		int endn = (nPostTransformMatricesChanged[1] + 3 ) / 4;
		const float* pstart = (const float*)&xfmem.postMatrices[startn * 4];
		m_buffer.SetMultiConstant4v(C_POSTTRANSFORMMATRICES + startn, endn - startn, pstart);
		nPostTransformMatricesChanged[0] = nPostTransformMatricesChanged[1] = -1;
	}

	if (nLightsChanged[0] >= 0)
	{
		// lights don't have a 1 to 1 mapping, the color component needs to be converted to 4 floats
		int istart = nLightsChanged[0] / 0x10;
		int iend = (nLightsChanged[1] + 15) / 0x10;
		
		for (int i = istart; i < iend; ++i)
		{
			const Light& light = xfmem.lights[i];
			// xfmem.light.color is packed as abgr in u8[4], so we have to swap the order
			m_buffer.SetConstant4(C_LIGHTS + 5 * i,
				light.color[3] * U8_NORM_COEF,
				light.color[2] * U8_NORM_COEF,
				light.color[1] * U8_NORM_COEF,
				light.color[0] * U8_NORM_COEF);
			m_buffer.SetConstant3v(C_LIGHTS + 5 * i + 1, light.cosatt);
			if (fabs(light.distatt[0]) < 0.00001f &&
				fabs(light.distatt[1]) < 0.00001f &&
				fabs(light.distatt[2]) < 0.00001f)
			{
				// dist attenuation, make sure not equal to 0!!!
				m_buffer.SetConstant4(C_LIGHTS + 5 * i + 2, 0.00001f, light.distatt[1], light.distatt[2], 0.0f);
			}
			else
			{
				m_buffer.SetConstant3v(C_LIGHTS + 5 * i + 2, light.distatt);
			}
			m_buffer.SetConstant3v(C_LIGHTS + 5 * i + 3, light.dpos);
			double norm = double(light.ddir[0]) * double(light.ddir[0]) +
				double(light.ddir[1]) * double(light.ddir[1]) +
				double(light.ddir[2]) * double(light.ddir[2]);
			norm = 1.0 / sqrt(norm);
			float norm_float = static_cast<float>(norm);
			m_buffer.SetConstant4(C_LIGHTS + 5 * i + 4, light.ddir[0] * norm_float, light.ddir[1] * norm_float, light.ddir[2] * norm_float, 0.0f);
		}

		nLightsChanged[0] = nLightsChanged[1] = -1;
	}

	if (nMaterialsChanged)
	{
		float GC_ALIGNED16(material[4]);
		for (int i = 0; i < 2; ++i)
		{
			if (nMaterialsChanged & (1 << i))
			{
				u32 data = *(xfmem.ambColor + i);
				material[0] = ((data >> 24) & 0xFF) * U8_NORM_COEF;
				material[1] = ((data >> 16) & 0xFF) * U8_NORM_COEF;
				material[2] = ((data >>  8) & 0xFF) * U8_NORM_COEF;
				material[3] = ( data        & 0xFF) * U8_NORM_COEF;

				m_buffer.SetConstant4v(C_MATERIALS + i, material);
			}
		}
		
		for (int i = 0; i < 2; ++i)
		{
			if (nMaterialsChanged & (1 << (i + 2)))
			{
				u32 data = *(xfmem.matColor + i);
				material[0] = ((data >> 24) & 0xFF) * U8_NORM_COEF;
				material[1] = ((data >> 16) & 0xFF) * U8_NORM_COEF;
				material[2] = ((data >>  8) & 0xFF) * U8_NORM_COEF;
				material[3] = ( data        & 0xFF) * U8_NORM_COEF;

				m_buffer.SetConstant4v(C_MATERIALS + i + 2, material);
			}
		}

		nMaterialsChanged = 0;
	}

	if (bPosNormalMatrixChanged)
	{
		bPosNormalMatrixChanged = false;

		const float *pos = (const float *)xfmem.posMatrices + MatrixIndexA.PosNormalMtxIdx * 4;
		const float *norm = (const float *)xfmem.normalMatrices + 3 * (MatrixIndexA.PosNormalMtxIdx & 31);

		m_buffer.SetMultiConstant4v(C_POSNORMALMATRIX, 3, pos);
		m_buffer.SetMultiConstant3v(C_POSNORMALMATRIX + 3, 3, norm);
	}

	if (bTexMatricesChanged[0])
	{
		bTexMatricesChanged[0] = false;
		const float *fptrs[] = 
		{
			(const float *)xfmem.posMatrices + MatrixIndexA.Tex0MtxIdx * 4, 
			(const float *)xfmem.posMatrices + MatrixIndexA.Tex1MtxIdx * 4,
			(const float *)xfmem.posMatrices + MatrixIndexA.Tex2MtxIdx * 4, 
			(const float *)xfmem.posMatrices + MatrixIndexA.Tex3MtxIdx * 4
		};

		for (int i = 0; i < 4; ++i)
		{
			m_buffer.SetMultiConstant4v(C_TEXMATRICES + 3 * i, 3, fptrs[i]);
		}
	}

	if (bTexMatricesChanged[1])
	{
		bTexMatricesChanged[1] = false;
		const float *fptrs[] = {
			(const float *)xfmem.posMatrices + MatrixIndexB.Tex4MtxIdx * 4, 
			(const float *)xfmem.posMatrices + MatrixIndexB.Tex5MtxIdx * 4,
			(const float *)xfmem.posMatrices + MatrixIndexB.Tex6MtxIdx * 4, 
			(const float *)xfmem.posMatrices + MatrixIndexB.Tex7MtxIdx * 4
		};

		for (int i = 0; i < 4; ++i)
		{
			m_buffer.SetMultiConstant4v(C_TEXMATRICES + 3 * i + 12, 3, fptrs[i]);
		}
	}

	if (bViewportChanged)
	{
		bViewportChanged = false;
		// The console GPU places the pixel center at 7/12 unless antialiasing
		// is enabled, while D3D11 and OpenGL place it at 0.5, D3D9 at 0.0. See the comment
		// in VertexShaderGen.cpp for details.
		// NOTE: If we ever emulate antialiasing, the sample locations set by
		// BP registers 0x01-0x04 need to be considered here.
		const float pixel_center_correction = ((g_ActiveConfig.backend_info.APIType & API_D3D9) ? 0.0f : 0.5f) - 7.0f / 12.0f;
		const float pixel_size_x = 2.f / Renderer::EFBToScaledXf(2.f * xfmem.viewport.wd);
		const float pixel_size_y = 2.f / Renderer::EFBToScaledXf(2.f * xfmem.viewport.ht);
		m_buffer.SetConstant4(C_DEPTHPARAMS,
						xfmem.viewport.farZ * U24_NORM_COEF,
						xfmem.viewport.zRange * U24_NORM_COEF,
						pixel_center_correction * pixel_size_x,
						pixel_center_correction * pixel_size_y);
		// This is so implementation-dependent that we can't have it here.
		UpdateViewport(s_viewportCorrection);
		bProjectionChanged = true;
	}

	if (bProjectionChanged)
	{
		bProjectionChanged = false;
		
		float *rawProjection = xfmem.projection.rawProjection;

		switch(xfmem.projection.type)
		{
		case GX_PERSPECTIVE:

			g_fProjectionMatrix[0] = rawProjection[0] * g_ActiveConfig.fAspectRatioHackW;
			g_fProjectionMatrix[1] = 0.0f;
			g_fProjectionMatrix[2] = rawProjection[1];
			g_fProjectionMatrix[3] = 0.0f;

			g_fProjectionMatrix[4] = 0.0f;
			g_fProjectionMatrix[5] = rawProjection[2] * g_ActiveConfig.fAspectRatioHackH;
			g_fProjectionMatrix[6] = rawProjection[3];
			g_fProjectionMatrix[7] = 0.0f;

			g_fProjectionMatrix[8] = 0.0f;
			g_fProjectionMatrix[9] = 0.0f;
			g_fProjectionMatrix[10] = rawProjection[4];

			g_fProjectionMatrix[11] = rawProjection[5];

			g_fProjectionMatrix[12] = 0.0f;
			g_fProjectionMatrix[13] = 0.0f;
			// donkopunchstania: GC GPU rounds differently?
			// -(1 + epsilon) so objects are clipped as they are on the real HW
			g_fProjectionMatrix[14] = -1.00000011921f;
			g_fProjectionMatrix[15] = 0.0f;

			SETSTAT_FT(stats.gproj_0, g_fProjectionMatrix[0]);
			SETSTAT_FT(stats.gproj_1, g_fProjectionMatrix[1]);
			SETSTAT_FT(stats.gproj_2, g_fProjectionMatrix[2]);
			SETSTAT_FT(stats.gproj_3, g_fProjectionMatrix[3]);
			SETSTAT_FT(stats.gproj_4, g_fProjectionMatrix[4]);
			SETSTAT_FT(stats.gproj_5, g_fProjectionMatrix[5]);
			SETSTAT_FT(stats.gproj_6, g_fProjectionMatrix[6]);
			SETSTAT_FT(stats.gproj_7, g_fProjectionMatrix[7]);
			SETSTAT_FT(stats.gproj_8, g_fProjectionMatrix[8]);
			SETSTAT_FT(stats.gproj_9, g_fProjectionMatrix[9]);
			SETSTAT_FT(stats.gproj_10, g_fProjectionMatrix[10]);
			SETSTAT_FT(stats.gproj_11, g_fProjectionMatrix[11]);
			SETSTAT_FT(stats.gproj_12, g_fProjectionMatrix[12]);
			SETSTAT_FT(stats.gproj_13, g_fProjectionMatrix[13]);
			SETSTAT_FT(stats.gproj_14, g_fProjectionMatrix[14]);
			SETSTAT_FT(stats.gproj_15, g_fProjectionMatrix[15]);
			break;

		case GX_ORTHOGRAPHIC:

			g_fProjectionMatrix[0] = rawProjection[0];
			g_fProjectionMatrix[1] = 0.0f;
			g_fProjectionMatrix[2] = 0.0f;
			g_fProjectionMatrix[3] = rawProjection[1];

			g_fProjectionMatrix[4] = 0.0f;
			g_fProjectionMatrix[5] = rawProjection[2];
			g_fProjectionMatrix[6] = 0.0f;
			g_fProjectionMatrix[7] = rawProjection[3];

			g_fProjectionMatrix[8] = 0.0f;
			g_fProjectionMatrix[9] = 0.0f;
			g_fProjectionMatrix[10] = (g_ProjHack1.value + rawProjection[4]) * ((g_ProjHack1.sign == 0) ? 1.0f : g_ProjHack1.sign);
			g_fProjectionMatrix[11] = (g_ProjHack2.value + rawProjection[5]) * ((g_ProjHack2.sign == 0) ? 1.0f : g_ProjHack2.sign);

			g_fProjectionMatrix[12] = 0.0f;
			g_fProjectionMatrix[13] = 0.0f;

			/*
			projection hack for metroid other m...attempt to remove black projection layer from cut scenes.
			g_fProjectionMatrix[15] = 1.0f was the default setting before
			this hack was added...setting g_fProjectionMatrix[14] to -1 might make the hack more stable, needs more testing.
			Only works for OGL and DX9...this is not helping DX11
			*/

			g_fProjectionMatrix[14] = 0.0f;
			g_fProjectionMatrix[15] = (g_ProjHack3 && rawProjection[0] == 2.0f ? 0.0f : 1.0f);  //causes either the efb copy or bloom layer not to show if proj hack enabled

			SETSTAT_FT(stats.g2proj_0, g_fProjectionMatrix[0]);
			SETSTAT_FT(stats.g2proj_1, g_fProjectionMatrix[1]);
			SETSTAT_FT(stats.g2proj_2, g_fProjectionMatrix[2]);
			SETSTAT_FT(stats.g2proj_3, g_fProjectionMatrix[3]);
			SETSTAT_FT(stats.g2proj_4, g_fProjectionMatrix[4]);
			SETSTAT_FT(stats.g2proj_5, g_fProjectionMatrix[5]);
			SETSTAT_FT(stats.g2proj_6, g_fProjectionMatrix[6]);
			SETSTAT_FT(stats.g2proj_7, g_fProjectionMatrix[7]);
			SETSTAT_FT(stats.g2proj_8, g_fProjectionMatrix[8]);
			SETSTAT_FT(stats.g2proj_9, g_fProjectionMatrix[9]);
			SETSTAT_FT(stats.g2proj_10, g_fProjectionMatrix[10]);
			SETSTAT_FT(stats.g2proj_11, g_fProjectionMatrix[11]);
			SETSTAT_FT(stats.g2proj_12, g_fProjectionMatrix[12]);
			SETSTAT_FT(stats.g2proj_13, g_fProjectionMatrix[13]);
			SETSTAT_FT(stats.g2proj_14, g_fProjectionMatrix[14]);
			SETSTAT_FT(stats.g2proj_15, g_fProjectionMatrix[15]);
			SETSTAT_FT(stats.proj_0, rawProjection[0]);
			SETSTAT_FT(stats.proj_1, rawProjection[1]);
			SETSTAT_FT(stats.proj_2, rawProjection[2]);
			SETSTAT_FT(stats.proj_3, rawProjection[3]);
			SETSTAT_FT(stats.proj_4, rawProjection[4]);
			SETSTAT_FT(stats.proj_5, rawProjection[5]);
			break;

		default:
			ERROR_LOG(VIDEO, "Unknown projection type: %d", xfmem.projection.type);
		}

		PRIM_LOG("Projection: %f %f %f %f %f %f\n", rawProjection[0], rawProjection[1], rawProjection[2], rawProjection[3], rawProjection[4], rawProjection[5]);

		if ((g_ActiveConfig.bFreeLook || g_ActiveConfig.i3DStereo ) && xfmem.projection.type == GX_PERSPECTIVE)
		{
			Matrix44 mtxA;
			Matrix44 mtxB;
			Matrix44 viewMtx;

			Matrix44::Translate(mtxA, s_fViewTranslationVector);
			Matrix44::LoadMatrix33(mtxB, s_viewRotationMatrix);
			Matrix44::Multiply(mtxB, mtxA, viewMtx); // view = rotation x translation
			Matrix44::Set(mtxB, g_fProjectionMatrix);
			Matrix44::Multiply(mtxB, viewMtx, mtxA); // mtxA = projection x view
			Matrix44::Multiply(s_viewportCorrection, mtxA, mtxB); // mtxB = viewportCorrection x mtxA

			m_buffer.SetMultiConstant4v(C_PROJECTION, 4, mtxB.data);
		}
		else
		{
			Matrix44 projMtx;
			Matrix44::Set(projMtx, g_fProjectionMatrix);

			Matrix44 correctedMtx;
			Matrix44::Multiply(s_viewportCorrection, projMtx, correctedMtx);
			m_buffer.SetMultiConstant4v(C_PROJECTION, 4, correctedMtx.data);
		}
	}
}

void VertexShaderManager::InvalidateXFRange(int start, int end)
{
	if (((u32)start >= (u32)MatrixIndexA.PosNormalMtxIdx * 4 &&
		 (u32)start <  (u32)MatrixIndexA.PosNormalMtxIdx * 4 + 12) ||
		((u32)start >= XFMEM_NORMALMATRICES + ((u32)MatrixIndexA.PosNormalMtxIdx & 31) * 3 &&
		 (u32)start <  XFMEM_NORMALMATRICES + ((u32)MatrixIndexA.PosNormalMtxIdx & 31) * 3 + 9))
	{
		bPosNormalMatrixChanged = true;
	}

	if (((u32)start >= (u32)MatrixIndexA.Tex0MtxIdx*4 && (u32)start < (u32)MatrixIndexA.Tex0MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexA.Tex1MtxIdx*4 && (u32)start < (u32)MatrixIndexA.Tex1MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexA.Tex2MtxIdx*4 && (u32)start < (u32)MatrixIndexA.Tex2MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexA.Tex3MtxIdx*4 && (u32)start < (u32)MatrixIndexA.Tex3MtxIdx*4+12))
	{
		bTexMatricesChanged[0] = true;
	}

	if (((u32)start >= (u32)MatrixIndexB.Tex4MtxIdx*4 && (u32)start < (u32)MatrixIndexB.Tex4MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexB.Tex5MtxIdx*4 && (u32)start < (u32)MatrixIndexB.Tex5MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexB.Tex6MtxIdx*4 && (u32)start < (u32)MatrixIndexB.Tex6MtxIdx*4+12) ||
		((u32)start >= (u32)MatrixIndexB.Tex7MtxIdx*4 && (u32)start < (u32)MatrixIndexB.Tex7MtxIdx*4+12))
	{
		bTexMatricesChanged[1] = true;
	}

	if (start < XFMEM_POSMATRICES_END)
	{
		if (nTransformMatricesChanged[0] == -1)
		{
			nTransformMatricesChanged[0] = start;
			nTransformMatricesChanged[1] = end>XFMEM_POSMATRICES_END?XFMEM_POSMATRICES_END:end;
		}
		else
		{
			if (nTransformMatricesChanged[0] > start) nTransformMatricesChanged[0] = start;
			if (nTransformMatricesChanged[1] < end) nTransformMatricesChanged[1] = end>XFMEM_POSMATRICES_END?XFMEM_POSMATRICES_END:end;
		}
	}

	if (start < XFMEM_NORMALMATRICES_END && end > XFMEM_NORMALMATRICES)
	{
		int _start = start < XFMEM_NORMALMATRICES ? 0 : start-XFMEM_NORMALMATRICES;
		int _end = end < XFMEM_NORMALMATRICES_END ? end-XFMEM_NORMALMATRICES : XFMEM_NORMALMATRICES_END-XFMEM_NORMALMATRICES;

		if (nNormalMatricesChanged[0] == -1)
		{
			nNormalMatricesChanged[0] = _start;
			nNormalMatricesChanged[1] = _end;
		}
		else
		{
			if (nNormalMatricesChanged[0] > _start) nNormalMatricesChanged[0] = _start;
			if (nNormalMatricesChanged[1] < _end) nNormalMatricesChanged[1] = _end;
		}
	}

	if (start < XFMEM_POSTMATRICES_END && end > XFMEM_POSTMATRICES)
	{
		int _start = start < XFMEM_POSTMATRICES ? XFMEM_POSTMATRICES : start-XFMEM_POSTMATRICES;
		int _end = end < XFMEM_POSTMATRICES_END ? end-XFMEM_POSTMATRICES : XFMEM_POSTMATRICES_END-XFMEM_POSTMATRICES;

		if (nPostTransformMatricesChanged[0] == -1)
		{
			nPostTransformMatricesChanged[0] = _start;
			nPostTransformMatricesChanged[1] = _end;
		}
		else
		{
			if (nPostTransformMatricesChanged[0] > _start) nPostTransformMatricesChanged[0] = _start;
			if (nPostTransformMatricesChanged[1] < _end) nPostTransformMatricesChanged[1] = _end;
		}
	}

	if (start < XFMEM_LIGHTS_END && end > XFMEM_LIGHTS)
	{
		int _start = start < XFMEM_LIGHTS ? XFMEM_LIGHTS : start-XFMEM_LIGHTS;
		int _end = end < XFMEM_LIGHTS_END ? end-XFMEM_LIGHTS : XFMEM_LIGHTS_END-XFMEM_LIGHTS;

		if (nLightsChanged[0] == -1 )
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

void VertexShaderManager::SetTexMatrixChangedA(u32 Value)
{
	if (MatrixIndexA.Hex != Value)
	{
		VertexManager::Flush();
		if (MatrixIndexA.PosNormalMtxIdx != (Value&0x3f))
			bPosNormalMatrixChanged = true;
		bTexMatricesChanged[0] = true;
		MatrixIndexA.Hex = Value;
	}
}

void VertexShaderManager::SetTexMatrixChangedB(u32 Value)
{
	if (MatrixIndexB.Hex != Value)
	{
		VertexManager::Flush();
		bTexMatricesChanged[1] = true;
		MatrixIndexB.Hex = Value;
	}
}

void VertexShaderManager::SetViewportChanged()
{
	bViewportChanged = true;
}

void VertexShaderManager::SetProjectionChanged()
{
	bProjectionChanged = true;
}

void VertexShaderManager::SetMaterialColorChanged(int index)
{
	nMaterialsChanged  |= (1 << index);
}

void VertexShaderManager::TranslateView(float x, float y, float z)
{
	float result[3];
	float vector[3] = { x,z,y };

	Matrix33::Multiply(s_viewInvRotationMatrix, vector, result);

	for (int i = 0; i < 3; i++)
		s_fViewTranslationVector[i] += result[i];

	bProjectionChanged = true;
}

void VertexShaderManager::RotateView(float x, float y)
{
	s_fViewRotation[0] += x;
	s_fViewRotation[1] += y;

	Matrix33 mx;
	Matrix33 my;
	Matrix33::RotateX(mx, s_fViewRotation[1]);
	Matrix33::RotateY(my, s_fViewRotation[0]);
	Matrix33::Multiply(mx, my, s_viewRotationMatrix);

	// reverse rotation
	Matrix33::RotateX(mx, -s_fViewRotation[1]);
	Matrix33::RotateY(my, -s_fViewRotation[0]);
	Matrix33::Multiply(my, mx, s_viewInvRotationMatrix);

	bProjectionChanged = true;
}

void VertexShaderManager::ResetView()
{
	memset(s_fViewTranslationVector, 0, sizeof(s_fViewTranslationVector));
	Matrix33::LoadIdentity(s_viewRotationMatrix);
	Matrix33::LoadIdentity(s_viewInvRotationMatrix);
	s_fViewRotation[0] = s_fViewRotation[1] = 0.0f;

	bProjectionChanged = true;
}

void VertexShaderManager::DoState(PointerWrap &p)
{
	p.Do(g_fProjectionMatrix);
	p.Do(s_viewportCorrection);
	p.Do(s_viewRotationMatrix);
	p.Do(s_viewInvRotationMatrix);
	p.Do(s_fViewTranslationVector);
	p.Do(s_fViewRotation);

	if (p.GetMode() == PointerWrap::MODE_READ)
	{
		Dirty();
	}
}
