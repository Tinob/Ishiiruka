// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka By Tino
#pragma once

// Top vertex loaders
// Metroid Prime: P I16-flt N I16-s16 T0 I16-u16 T1 i16-flt

#include <algorithm>
#include <string>

#include "Common/Common.h"
#include "Common/x64Emitter.h"

#include "VideoCommon/CPMemory.h"
#include "VideoCommon/DataReader.h"
#include "VideoCommon/NativeVertexFormat.h"

extern const float fractionTable[];
extern TPipelineState g_PipelineState;
class VertexLoaderUID
{
	u32 vid[5];
	size_t hash;
public:
	VertexLoaderUID()
	{
	}

	void InitFromCurrentState(s32 vtx_attr_group)
	{
		u32 fullmask = 0xFFFFFFFFu;
		vid[0] = g_VtxDesc.Hex & fullmask;
		vid[1] = (g_VtxDesc.Hex >> 32) & 1;// only the first bit is used
		// Disable unused components		
		u32 mask = ~VAT_0_FRACBITS;
		mask &= g_VtxDesc.Color0 ? fullmask : ~VAT_0_COL0BITS;
		mask &= g_VtxDesc.Color1 ? fullmask : ~VAT_0_COL1BITS;
		mask &= g_VtxDesc.Normal ? fullmask : ~VAT_0_NRMBITS;
		mask &= g_VtxDesc.Tex0Coord || g_VtxDesc.Tex0MatIdx ? fullmask : ~VAT_0_TEX0BITS;
		vid[2] = g_VtxAttr[vtx_attr_group].g0.Hex & mask;
		mask = ~VAT_1_FRACBITS;
		mask &= g_VtxDesc.Tex1Coord || g_VtxDesc.Tex1MatIdx ? fullmask : ~VAT_1_TEX1BITS;
		mask &= g_VtxDesc.Tex2Coord || g_VtxDesc.Tex2MatIdx ? fullmask : ~VAT_1_TEX2BITS;
		mask &= g_VtxDesc.Tex3Coord || g_VtxDesc.Tex3MatIdx ? fullmask : ~VAT_1_TEX3BITS;
		mask &= g_VtxDesc.Tex4Coord || g_VtxDesc.Tex4MatIdx ? fullmask : ~VAT_1_TEX4BITS;
		vid[3] = g_VtxAttr[vtx_attr_group].g1.Hex & mask;
		mask = ~VAT_2_FRACBITS;
		mask &= g_VtxDesc.Tex4Coord || g_VtxDesc.Tex4MatIdx ? fullmask : ~VAT_2_TEX4BITS;
		mask &= g_VtxDesc.Tex5Coord || g_VtxDesc.Tex5MatIdx ? fullmask : ~VAT_2_TEX5BITS;
		mask &= g_VtxDesc.Tex6Coord || g_VtxDesc.Tex6MatIdx ? fullmask : ~VAT_2_TEX6BITS;
		mask &= g_VtxDesc.Tex7Coord || g_VtxDesc.Tex7MatIdx ? fullmask : ~VAT_2_TEX7BITS;
		vid[4] = g_VtxAttr[vtx_attr_group].g2.Hex & mask;
		hash = CalculateHash();
	}

	bool operator < (const VertexLoaderUID &other) const
	{
		// This is complex because of speed.
		if (vid[0] < other.vid[0])
			return true;
		else if (vid[0] > other.vid[0])
			return false;

		for (int i = 1; i < 5; ++i)
		{
			if (vid[i] < other.vid[i])
				return true;
			else if (vid[i] > other.vid[i])
				return false;
		}

		return false;
	}

	bool operator == (const VertexLoaderUID& rh) const
	{
		return hash == rh.hash && std::equal(vid, vid + sizeof(vid) / sizeof(vid[0]), rh.vid);
	}

	size_t GetHash() const
	{
		return hash;
	}

	u32 GetElement(u32 idx) const
	{
		return vid[idx];
	}

private:

	size_t CalculateHash()
	{
		size_t h = -1;

		for (auto word : vid)
		{
			h = h * 137 + word;
		}

		return h;
	}
};

class VertexLoader;

typedef void (LOADERDECL *TCompiledLoaderFunction)(VertexLoader *loader);

class VertexLoader
{
public:
	VertexLoader(const TVtxDesc &vtx_desc, const VAT &vtx_attr, TCompiledLoaderFunction precompiledFunction);
	~VertexLoader();

	s32 GetVertexSize() const { return m_VertexSize; }

	int SetupRunVertices(s32 vtx_attr_group, s32 primitive, s32 const count);
	void RunVertices(s32 vtx_attr_group, s32 primitive, s32 count);
	void RunCompiledVertices(s32 vtx_attr_group, s32 primitive, s32 count, u8* Data);

	// For debugging / profiling
	void AppendToString(std::string *dest) const;
	void GetName(std::string *dest) const;
	void DumpCode(std::string *dest) const;

	u64 GetNumLoadedVerts() const { return m_numLoadedVertices; }
	bool IsPrecompiled();
private:
	bool m_Isprecompiled;
	TCompiledLoaderFunction m_CompiledFunction;
	enum
	{
		NRM_ZERO = 0,
		NRM_ONE = 1,
		NRM_THREE = 3,
	};

	s32 m_VertexSize;      // number of bytes of a raw GC vertex. Computed by CompileVertexTranslator.

	// GC vertex format
	TVtxAttr m_VtxAttr;  // VAT decoded into easy format
	TVtxDesc m_VtxDesc;  // Not really used currently - or well it is, but could be easily avoided.

	// PC vertex format
	NativeVertexFormat *m_NativeFmt;
	s32 native_stride;

	TPipelineFunction m_PipelineStages[32];
	s32 m_numPipelineStages;

	u64 m_numLoadedVertices;

	void SetVAT(u32 _group0, u32 _group1, u32 _group2);

	void CompileVertexTranslator();
	static void LOADERDECL ConvertVertices(VertexLoader *loader);

	void WriteCall(TPipelineFunction);
};