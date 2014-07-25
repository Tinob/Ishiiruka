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
	u32 vid[4];
	u64 hash;
	size_t platformhash;
public:
	VertexLoaderUID(){}
	void InitFromCurrentState(s32 vtx_attr_group);
	bool operator < (const VertexLoaderUID &other) const;
	bool operator == (const VertexLoaderUID& rh) const;
	u64 GetHash() const;
	size_t GetplatformHash() const;
	u32 GetElement(u32 idx) const;
private:
	u64 CalculateHash();
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
	void RunCompiledVertices(s32 vtx_attr_group, s32 primitive, s32 count, const u8* Data);

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