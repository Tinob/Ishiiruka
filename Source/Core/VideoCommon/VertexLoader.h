// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka By Tino
#pragma once
#include "VideoCommon/NativeVertexFormat.h"

struct VertexLoaderParameters
{
	const u8* source;
	u8* destination;
	const TVtxDesc *VtxDesc;
	const VAT *VtxAttr;
	size_t buf_size;
	int vtx_attr_group;
	int primitive;
	int count;
	bool skip_draw;
	bool needloaderrefresh;
};

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

	inline s32 GetVertexSize() const { return m_VertexSize; }
	inline NativeVertexFormat* GetNativeVertexFormat() const { return m_NativeFmt; }
	inline s32 GetNativeStride() const { return m_native_stride; }

	void RunVertices(const VertexLoaderParameters &parameters);

	// For debugging / profiling
	void AppendToString(std::string *dest) const;
	void GetName(std::string *dest) const;
	void DumpCode(std::string *dest) const;

	u64 GetNumLoadedVerts() const { return m_numLoadedVertices; }
	bool IsPrecompiled();	
private:
	bool m_Isprecompiled;
	TCompiledLoaderFunction m_CompiledFunction;

	s32 m_VertexSize;      // number of bytes of a raw GC vertex. Computed by CompileVertexTranslator.
	// PC vertex format
	NativeVertexFormat *m_NativeFmt;
	s32 m_native_stride;
	PortableVertexDeclaration m_vtx_decl;
	// GC vertex format
	TVtxAttr m_VtxAttr;  // VAT decoded into easy format
	TVtxDesc m_VtxDesc;  // Not really used currently - or well it is, but could be easily avoided.



	TPipelineFunction m_PipelineStages[32];
	s32 m_numPipelineStages;

	u64 m_numLoadedVertices;

	void SetVAT(const VAT &vtx_attr);
	void CompileVertexTranslator();

	static const VertexLoader* s_CurrentVertexLoader;
	static void LOADERDECL ConvertVertices(TPipelineState& pipelinestate);

	void WriteCall(TPipelineFunction);
};