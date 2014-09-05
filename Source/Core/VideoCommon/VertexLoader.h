// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka By Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/OpcodeDecoding.h"

class VertexLoaderUID
{
	u32 vid[4];
	u64 hash;
	size_t platformhash;
public:
	VertexLoaderUID(const TVtxDesc& VtxDesc, const VAT& vat);
	bool operator < (const VertexLoaderUID &other) const;
	bool operator == (const VertexLoaderUID& rh) const;
	u64 GetHash() const;
	size_t GetplatformHash() const;
	u32 GetElement(u32 idx) const;
private:
	u64 CalculateHash();
};

class VertexLoader
{
public:
	VertexLoader(const TVtxDesc &vtx_desc, const VAT &vtx_attr, TCompiledLoaderFunction precompiledFunction);
	~VertexLoader();

	s32 GetVertexSize() const { return m_VertexSize; }

	void RunVertices(const VAT &vtx_attr, s32 primitive, s32 count);

	// For debugging / profiling
	void AppendToString(std::string *dest) const;
	void GetName(std::string *dest) const;
	void DumpCode(std::string *dest) const;

	u64 GetNumLoadedVerts() const { return m_numLoadedVertices; }
	bool IsPrecompiled();
	// PC vertex format
	NativeVertexFormat *m_NativeFmt;
	s32 native_stride;
private:
	bool m_Isprecompiled;
	TCompiledLoaderFunction m_CompiledFunction;

	s32 m_VertexSize;      // number of bytes of a raw GC vertex. Computed by CompileVertexTranslator.

	// GC vertex format
	TVtxAttr m_VtxAttr;  // VAT decoded into easy format
	TVtxDesc m_VtxDesc;  // Not really used currently - or well it is, but could be easily avoided.



	TPipelineFunction m_PipelineStages[32];
	s32 m_numPipelineStages;

	u64 m_numLoadedVertices;

	void SetVAT(const VAT &vtx_attr);
	s32 SetupRunVertices(const VAT &vtx_attr, s32 primitive, s32 const count);
	void CompileVertexTranslator();

	static const VertexLoader* s_CurrentVertexLoader;
	static void LOADERDECL ConvertVertices(TPipelineState& pipelinestate);

	void WriteCall(TPipelineFunction);
};