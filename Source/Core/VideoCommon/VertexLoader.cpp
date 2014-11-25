// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka By Tino

#include "Core/Host.h"

#include "VideoCommon/BPMemory.h"
#include "VideoCommon/BoundingBox.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoader_Color.h"
#include "VideoCommon/VertexLoader_Normal.h"
#include "VideoCommon/VertexLoader_Position.h"
#include "VideoCommon/VertexLoader_TextCoord.h"
#include "VideoCommon/VertexLoader_Mtx.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VideoConfig.h"

TPipelineState g_PipelineState;

const float fractionTable[32] = {
	1.0f / (1U << 0), 1.0f / (1U << 1), 1.0f / (1U << 2), 1.0f / (1U << 3),
	1.0f / (1U << 4), 1.0f / (1U << 5), 1.0f / (1U << 6), 1.0f / (1U << 7),
	1.0f / (1U << 8), 1.0f / (1U << 9), 1.0f / (1U << 10), 1.0f / (1U << 11),
	1.0f / (1U << 12), 1.0f / (1U << 13), 1.0f / (1U << 14), 1.0f / (1U << 15),
	1.0f / (1U << 16), 1.0f / (1U << 17), 1.0f / (1U << 18), 1.0f / (1U << 19),
	1.0f / (1U << 20), 1.0f / (1U << 21), 1.0f / (1U << 22), 1.0f / (1U << 23),
	1.0f / (1U << 24), 1.0f / (1U << 25), 1.0f / (1U << 26), 1.0f / (1U << 27),
	1.0f / (1U << 28), 1.0f / (1U << 29), 1.0f / (1U << 30), 1.0f / (1U << 31),
};

VertexLoaderUID::VertexLoaderUID(const TVtxDesc& VtxDesc, const VAT& vat)
{
	u32 fullmask = 0xFFFFFFFFu;
	vid[0] = (u32)((VtxDesc.Hex >> 1) & fullmask);
	// Disable unused components		
	u32 mask = ~VAT_0_FRACBITS;
	mask &= VtxDesc.Color0 ? fullmask : ~VAT_0_COL0BITS;
	mask &= VtxDesc.Color1 ? fullmask : ~VAT_0_COL1BITS;
	mask &= VtxDesc.Normal ? fullmask : ~VAT_0_NRMBITS;
	mask &= VtxDesc.Tex0Coord || VtxDesc.Tex0MatIdx ? fullmask : ~VAT_0_TEX0BITS;
	vid[1] = vat.g0.Hex & mask;
	mask = ~VAT_1_FRACBITS;
	mask &= VtxDesc.Tex1Coord || VtxDesc.Tex1MatIdx ? fullmask : ~VAT_1_TEX1BITS;
	mask &= VtxDesc.Tex2Coord || VtxDesc.Tex2MatIdx ? fullmask : ~VAT_1_TEX2BITS;
	mask &= VtxDesc.Tex3Coord || VtxDesc.Tex3MatIdx ? fullmask : ~VAT_1_TEX3BITS;
	mask &= VtxDesc.Tex4Coord || VtxDesc.Tex4MatIdx ? fullmask : ~VAT_1_TEX4BITS;
	vid[2] = vat.g1.Hex & mask;
	// encode posmtxidx in the free bit inside VAT2
	vid[2] = VtxDesc.PosMatIdx ? (vid[2] | 0x80000000u) : (vid[2] & 0x7FFFFFFFu);
	mask = ~VAT_2_FRACBITS;
	mask &= VtxDesc.Tex4Coord || VtxDesc.Tex4MatIdx ? fullmask : ~VAT_2_TEX4BITS;
	mask &= VtxDesc.Tex5Coord || VtxDesc.Tex5MatIdx ? fullmask : ~VAT_2_TEX5BITS;
	mask &= VtxDesc.Tex6Coord || VtxDesc.Tex6MatIdx ? fullmask : ~VAT_2_TEX6BITS;
	mask &= VtxDesc.Tex7Coord || VtxDesc.Tex7MatIdx ? fullmask : ~VAT_2_TEX7BITS;
	vid[3] = vat.g2.Hex & mask;
	hash = CalculateHash();
	if (sizeof(size_t) >= sizeof(u64))
	{
		platformhash = (size_t)hash;
	}
	else
	{
		size_t platformmask = 0;
		platformmask = ~platformmask;
		platformhash = (size_t)(hash & platformmask);
		u32 sl = sizeof(size_t) * 8;
		platformhash = platformhash ^ (size_t)((hash >> sl) && platformmask);
	}
}

bool VertexLoaderUID::operator < (const VertexLoaderUID &other) const
{
	// This is complex because of speed.
	if (vid[0] < other.vid[0])
		return true;
	else if (vid[0] > other.vid[0])
		return false;

	for (int i = 1; i < 4; ++i)
	{
		if (vid[i] < other.vid[i])
			return true;
		else if (vid[i] > other.vid[i])
			return false;
	}

	return false;
}

bool VertexLoaderUID::operator == (const VertexLoaderUID& rh) const
{
	return hash == rh.hash && std::equal(vid, vid + sizeof(vid) / sizeof(vid[0]), rh.vid);
}

u64 VertexLoaderUID::GetHash() const
{
	return hash;
}

size_t VertexLoaderUID::GetplatformHash() const
{
	return platformhash;
}

u32 VertexLoaderUID::GetElement(u32 idx) const
{
	return vid[idx];
}

u64 VertexLoaderUID::CalculateHash()
{
	u64 h = -1;
	for (auto word : vid)
	{
		h = h * 137 + word;
	}
	return h;
}

const VertexLoader* VertexLoader::s_CurrentVertexLoader = NULL;

VertexLoader::VertexLoader(const TVtxDesc &vtx_desc, const VAT &vtx_attr, TCompiledLoaderFunction precompiledFunction)
{
	m_numLoadedVertices = 0;
	m_VertexSize = 0;

	VertexLoader_Normal::Init();
	VertexLoader_Position::Init();
	VertexLoader_TextCoord::Init();

	m_VtxDesc = vtx_desc;
	SetVAT(vtx_attr);
	m_numPipelineStages = 0;
	CompileVertexTranslator();
	m_Isprecompiled = precompiledFunction != nullptr;
	m_CompiledFunction = precompiledFunction != nullptr ? precompiledFunction : VertexLoader::ConvertVertices;
}

VertexLoader::~VertexLoader()
{

}

bool VertexLoader::IsPrecompiled()
{
	return m_Isprecompiled;
}

void VertexLoader::CompileVertexTranslator()
{
	m_VertexSize = 0;
	// Reset pipeline
	m_numPipelineStages = 0;

	// Get the pointer to this vertex's buffer data for the bounding box
	if (!g_ActiveConfig.backend_info.bSupportsBBox)
		WriteCall(BoundingBox::SetVertexBufferPosition);

	// Colors
	const u32 col[2] = { m_VtxDesc.Color0, m_VtxDesc.Color1 };
	// TextureCoord
	// Since m_VtxDesc.Text7Coord is broken across a 32 bit word boundary, retrieve its value manually.
	// If we didn't do this, the vertex format would be read as one bit offset from where it should be, making
	// 01 become 00, and 10/11 become 01
	const u32 tc[8] = {
		m_VtxDesc.Tex0Coord, m_VtxDesc.Tex1Coord, m_VtxDesc.Tex2Coord, m_VtxDesc.Tex3Coord,
		m_VtxDesc.Tex4Coord, m_VtxDesc.Tex5Coord, m_VtxDesc.Tex6Coord, (const u32)((m_VtxDesc.Hex >> 31) & 3)
	};

	u32 components = 0;

	// Position in pc vertex format.
	int nat_offset = 0;
	memset(&m_vtx_decl, 0, sizeof(m_vtx_decl));

	// Position Matrix Index
	if (m_VtxDesc.PosMatIdx)
	{
		WriteCall(Vertexloader_Mtx::PosMtx_ReadDirect_UByte);
		components |= VB_HAS_POSMTXIDX;
		m_VertexSize += 1;
	}

	if (m_VtxDesc.Tex0MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX0; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex1MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX1; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex2MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX2; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex3MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX3; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex4MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX4; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex5MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX5; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex6MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX6; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }
	if (m_VtxDesc.Tex7MatIdx) { m_VertexSize += 1; components |= VB_HAS_TEXMTXIDX7; WriteCall(Vertexloader_Mtx::TexMtx_ReadDirect_UByte); }

	WriteCall(VertexLoader_Position::GetFunction(m_VtxDesc.Position, m_VtxAttr.PosFormat, m_VtxAttr.PosElements));
	
	m_VertexSize += VertexLoader_Position::GetSize(m_VtxDesc.Position, m_VtxAttr.PosFormat, m_VtxAttr.PosElements);
	nat_offset += 12;
	m_vtx_decl.position.components = 3;
	m_vtx_decl.position.enable = true;
	m_vtx_decl.position.offset = 0;
	m_vtx_decl.position.type = FORMAT_FLOAT;

	// Normals
	if (m_VtxDesc.Normal != NOT_PRESENT)
	{
		m_VertexSize += VertexLoader_Normal::GetSize(m_VtxDesc.Normal,
			m_VtxAttr.NormalFormat, m_VtxAttr.NormalElements, m_VtxAttr.NormalIndex3);

		TPipelineFunction pFunc = VertexLoader_Normal::GetFunction(m_VtxDesc.Normal,
			m_VtxAttr.NormalFormat, m_VtxAttr.NormalElements, m_VtxAttr.NormalIndex3);

		if (pFunc == 0)
		{
			PanicAlert("VertexLoader_Normal::GetFunction(%i %i %i %i) returned zero!",
				m_VtxDesc.Normal, m_VtxAttr.NormalFormat,
				m_VtxAttr.NormalElements, m_VtxAttr.NormalIndex3);
		}
		WriteCall(pFunc);

		for (int i = 0; i < (m_VtxAttr.NormalElements ? 3 : 1); i++)
		{
			m_vtx_decl.normals[i].components = 3;
			m_vtx_decl.normals[i].enable = true;
			m_vtx_decl.normals[i].offset = nat_offset;
			m_vtx_decl.normals[i].type = FORMAT_FLOAT;
			nat_offset += 12;
		}

		components |= VB_HAS_NRM0;
		if (m_VtxAttr.NormalElements == 1)
			components |= VB_HAS_NRM1 | VB_HAS_NRM2;
	}

	for (int i = 0; i < 2; i++)
	{
		switch (col[i])
		{
		case NOT_PRESENT:
			break;
		case DIRECT:
			switch (m_VtxAttr.color[i].Comp)
			{
			case FORMAT_16B_565:	m_VertexSize += 2; WriteCall(Color_ReadDirect_16b_565); break;
			case FORMAT_24B_888:	m_VertexSize += 3; WriteCall(Color_ReadDirect_24b_888); break;
			case FORMAT_32B_888x:	m_VertexSize += 4; WriteCall(Color_ReadDirect_32b_888x); break;
			case FORMAT_16B_4444:	m_VertexSize += 2; WriteCall(Color_ReadDirect_16b_4444); break;
			case FORMAT_24B_6666:	m_VertexSize += 3; WriteCall(Color_ReadDirect_24b_6666); break;
			case FORMAT_32B_8888:	m_VertexSize += 4; WriteCall(Color_ReadDirect_32b_8888); break;
			default: _assert_(0); break;
			}
			break;
		case INDEX8:
			m_VertexSize += 1;
			switch (m_VtxAttr.color[i].Comp)
			{
			case FORMAT_16B_565:	WriteCall(Color_ReadIndex8_16b_565); break;
			case FORMAT_24B_888:	WriteCall(Color_ReadIndex8_24b_888); break;
			case FORMAT_32B_888x:	WriteCall(Color_ReadIndex8_32b_888x); break;
			case FORMAT_16B_4444:	WriteCall(Color_ReadIndex8_16b_4444); break;
			case FORMAT_24B_6666:	WriteCall(Color_ReadIndex8_24b_6666); break;
			case FORMAT_32B_8888:	WriteCall(Color_ReadIndex8_32b_8888); break;
			default: _assert_(0); break;
			}
			break;
		case INDEX16:
			m_VertexSize += 2;
			switch (m_VtxAttr.color[i].Comp)
			{
			case FORMAT_16B_565:	WriteCall(Color_ReadIndex16_16b_565); break;
			case FORMAT_24B_888:	WriteCall(Color_ReadIndex16_24b_888); break;
			case FORMAT_32B_888x:	WriteCall(Color_ReadIndex16_32b_888x); break;
			case FORMAT_16B_4444:	WriteCall(Color_ReadIndex16_16b_4444); break;
			case FORMAT_24B_6666:	WriteCall(Color_ReadIndex16_24b_6666); break;
			case FORMAT_32B_8888:	WriteCall(Color_ReadIndex16_32b_8888); break;
			default: _assert_(0); break;
			}
			break;
		}
		// Common for the three bottom cases
		if (col[i] != NOT_PRESENT)
		{
			components |= VB_HAS_COL0 << i;
			m_vtx_decl.colors[i].components = 4;
			m_vtx_decl.colors[i].type = FORMAT_UBYTE;
			m_vtx_decl.colors[i].offset = nat_offset;
			m_vtx_decl.colors[i].enable = true;
			nat_offset += 4;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		const int format = m_VtxAttr.texCoord[i].Format;
		const int elements = m_VtxAttr.texCoord[i].Elements;

		if (tc[i] != NOT_PRESENT)
		{
			_assert_msg_(VIDEO, DIRECT <= tc[i] && tc[i] <= INDEX16, "Invalid texture coordinates!\n(tc[i] = %d)", tc[i]);
			_assert_msg_(VIDEO, FORMAT_UBYTE <= format && format <= FORMAT_FLOAT, "Invalid texture coordinates format!\n(format = %d)", format);
			_assert_msg_(VIDEO, 0 <= elements && elements <= 1, "Invalid number of texture coordinates elements!\n(elements = %d)", elements);

			components |= VB_HAS_UV0 << i;
			WriteCall(VertexLoader_TextCoord::GetFunction(tc[i], format, elements));
			m_VertexSize += VertexLoader_TextCoord::GetSize(tc[i], format, elements);
		}

		if (components & (VB_HAS_TEXMTXIDX0 << i))
		{
			m_vtx_decl.texcoords[i].enable = true;
			m_vtx_decl.texcoords[i].offset = nat_offset;
			m_vtx_decl.texcoords[i].type = FORMAT_FLOAT;
			if (tc[i] != NOT_PRESENT)
			{
				// if texmtx is included, texcoord will always be 3 floats, z will be the texmtx index
				m_vtx_decl.texcoords[i].components = 3;
				nat_offset += 12;
				WriteCall(m_VtxAttr.texCoord[i].Elements ? Vertexloader_Mtx::TexMtx_Write_Float : Vertexloader_Mtx::TexMtx_Write_Float2);
			}
			else
			{
				components |= VB_HAS_UV0 << i; // have to include since using now
				m_vtx_decl.texcoords[i].components = 4;
				nat_offset += 16; // still include the texture coordinate, but this time as 6 + 2 bytes
				WriteCall(Vertexloader_Mtx::TexMtx_Write_Float4);
			}
		}
		else
		{
			if (tc[i] != NOT_PRESENT)
			{
				m_vtx_decl.texcoords[i].enable = true;
				m_vtx_decl.texcoords[i].offset = nat_offset;
				m_vtx_decl.texcoords[i].type = FORMAT_FLOAT;
				m_vtx_decl.texcoords[i].components = m_VtxAttr.texCoord[i].Elements ? 2 : 1;
				nat_offset += 4 * (m_VtxAttr.texCoord[i].Elements ? 2 : 1);
			}
		}
		if (tc[i] == NOT_PRESENT)
		{
			// if there's more tex coords later, have to write a dummy call
			int j = i + 1;
			for (; j < 8; ++j)
			{
				if (tc[j] != NOT_PRESENT)
				{
					WriteCall(VertexLoader_TextCoord::GetDummyFunction()); // important to get indices right!
					break;
				}
			}
			// tricky!
			if (j == 8 && !((components & VB_HAS_TEXMTXIDXALL) & (VB_HAS_TEXMTXIDXALL << (i + 1))))
			{
				// no more tex coords and tex matrices, so exit loop
				break;
			}
		}
	}
	// Update the bounding box
	if (!g_ActiveConfig.backend_info.bSupportsBBox)
		WriteCall(BoundingBox::Update);

	if (m_VtxDesc.PosMatIdx)
	{
		WriteCall(Vertexloader_Mtx::PosMtx_Write);
		m_vtx_decl.posmtx.enable = true;
	}
	else
	{
		WriteCall(Vertexloader_Mtx::PosMtxDisabled_Write);
	}
	m_vtx_decl.posmtx.components = 4;
	m_vtx_decl.posmtx.offset = nat_offset;
	m_vtx_decl.posmtx.type = FORMAT_UBYTE;
	nat_offset += 4;

	m_native_stride = nat_offset;
	m_vtx_decl.stride = m_native_stride;
	m_NativeFmt = VertexLoaderManager::GetNativeVertexFormat(m_vtx_decl, components);
}

void VertexLoader::WriteCall(TPipelineFunction func)
{
	m_PipelineStages[m_numPipelineStages++] = func;
}

void VertexLoader::RunVertices(const VertexLoaderParameters &parameters)
{
	m_numLoadedVertices += parameters.count;
	const VAT &vat = *parameters.VtxAttr;
	// Load position and texcoord scale factors.
	m_VtxAttr.PosFrac = vat.g0.PosFrac;
	m_VtxAttr.texCoord[0].Frac = vat.g0.Tex0Frac;
	m_VtxAttr.texCoord[1].Frac = vat.g1.Tex1Frac;
	m_VtxAttr.texCoord[2].Frac = vat.g1.Tex2Frac;
	m_VtxAttr.texCoord[3].Frac = vat.g1.Tex3Frac;
	m_VtxAttr.texCoord[4].Frac = vat.g2.Tex4Frac;
	m_VtxAttr.texCoord[5].Frac = vat.g2.Tex5Frac;
	m_VtxAttr.texCoord[6].Frac = vat.g2.Tex6Frac;
	m_VtxAttr.texCoord[7].Frac = vat.g2.Tex7Frac;
	g_PipelineState.bUseBBox = !g_ActiveConfig.backend_info.bSupportsBBox ? 1 : 0;
	g_PipelineState.posScale = fractionTable[m_VtxAttr.PosFrac];
	if (m_NativeFmt->m_components & VB_HAS_UVALL)
		for (int i = 0; i < 8; i++)
			g_PipelineState.tcScale[i] = fractionTable[m_VtxAttr.texCoord[i].Frac];
	for (int i = 0; i < 2; i++)
		g_PipelineState.colElements[i] = m_VtxAttr.color[i].Elements;
	// Prepare bounding box
	if (!g_ActiveConfig.backend_info.bSupportsBBox)
		BoundingBox::Prepare(vat, parameters.primitive, m_VtxDesc, m_vtx_decl);
	g_PipelineState.count = parameters.count;
	g_PipelineState.Initialize(parameters.source, parameters.destination);
	s_CurrentVertexLoader = this;
	m_CompiledFunction(g_PipelineState);
}

void LOADERDECL VertexLoader::ConvertVertices(TPipelineState& pipelinestate)
{
	s32 stagescount = s_CurrentVertexLoader->m_numPipelineStages;
	const TPipelineFunction* stages = s_CurrentVertexLoader->m_PipelineStages;
	s32 count = pipelinestate.count;
	while (count)
	{
		pipelinestate.tcIndex = 0;
		pipelinestate.colIndex = 0;
		pipelinestate.texmtxwrite = pipelinestate.texmtxread = 0;
		for (int i = 0; i < stagescount; i++)
			stages[i]();
		count--;
	}
}

void VertexLoader::SetVAT(const VAT &vat)
{
	m_VtxAttr.PosElements = vat.g0.PosElements;
	m_VtxAttr.PosFormat = vat.g0.PosFormat;
	m_VtxAttr.PosFrac = vat.g0.PosFrac;
	m_VtxAttr.NormalElements = vat.g0.NormalElements;
	m_VtxAttr.NormalFormat = vat.g0.NormalFormat;
	m_VtxAttr.color[0].Elements = vat.g0.Color0Elements;
	m_VtxAttr.color[0].Comp = vat.g0.Color0Comp;
	m_VtxAttr.color[1].Elements = vat.g0.Color1Elements;
	m_VtxAttr.color[1].Comp = vat.g0.Color1Comp;
	m_VtxAttr.texCoord[0].Elements = vat.g0.Tex0CoordElements;
	m_VtxAttr.texCoord[0].Format = vat.g0.Tex0CoordFormat;
	m_VtxAttr.texCoord[0].Frac = vat.g0.Tex0Frac;
	m_VtxAttr.ByteDequant = vat.g0.ByteDequant;
	m_VtxAttr.NormalIndex3 = vat.g0.NormalIndex3;

	m_VtxAttr.texCoord[1].Elements = vat.g1.Tex1CoordElements;
	m_VtxAttr.texCoord[1].Format = vat.g1.Tex1CoordFormat;
	m_VtxAttr.texCoord[1].Frac = vat.g1.Tex1Frac;
	m_VtxAttr.texCoord[2].Elements = vat.g1.Tex2CoordElements;
	m_VtxAttr.texCoord[2].Format = vat.g1.Tex2CoordFormat;
	m_VtxAttr.texCoord[2].Frac = vat.g1.Tex2Frac;
	m_VtxAttr.texCoord[3].Elements = vat.g1.Tex3CoordElements;
	m_VtxAttr.texCoord[3].Format = vat.g1.Tex3CoordFormat;
	m_VtxAttr.texCoord[3].Frac = vat.g1.Tex3Frac;
	m_VtxAttr.texCoord[4].Elements = vat.g1.Tex4CoordElements;
	m_VtxAttr.texCoord[4].Format = vat.g1.Tex4CoordFormat;

	m_VtxAttr.texCoord[4].Frac = vat.g2.Tex4Frac;
	m_VtxAttr.texCoord[5].Elements = vat.g2.Tex5CoordElements;
	m_VtxAttr.texCoord[5].Format = vat.g2.Tex5CoordFormat;
	m_VtxAttr.texCoord[5].Frac = vat.g2.Tex5Frac;
	m_VtxAttr.texCoord[6].Elements = vat.g2.Tex6CoordElements;
	m_VtxAttr.texCoord[6].Format = vat.g2.Tex6CoordFormat;
	m_VtxAttr.texCoord[6].Frac = vat.g2.Tex6Frac;
	m_VtxAttr.texCoord[7].Elements = vat.g2.Tex7CoordElements;
	m_VtxAttr.texCoord[7].Format = vat.g2.Tex7CoordFormat;
	m_VtxAttr.texCoord[7].Frac = vat.g2.Tex7Frac;

	if (!m_VtxAttr.ByteDequant) {
		ERROR_LOG(VIDEO, "ByteDequant is set to zero");
	}
};

void VertexLoader::GetName(std::string *dest) const
{
	static const char *posMode[4] = {
		"Inv",
		"Dir",
		"I8",
		"I16",
	};
	static const char *posFormats[5] = {
		"u8", "s8", "u16", "s16", "flt",
	};
	static const char *colorFormat[8] = {
		"565",
		"888",
		"888x",
		"4444",
		"6666",
		"8888",
		"Inv",
		"Inv",
	};

	dest->append(StringFromFormat("P_mtx%i_%i_%s_%s_", m_VtxDesc.PosMatIdx,
		m_VtxAttr.PosElements ? 3 : 2, posMode[m_VtxDesc.Position], posFormats[m_VtxAttr.PosFormat]));

	if (m_VtxDesc.Normal)
	{
		dest->append(StringFromFormat("Nrm_%i_%i_%s_%s_",
			m_VtxAttr.NormalElements, m_VtxAttr.NormalIndex3, posMode[m_VtxDesc.Normal], posFormats[m_VtxAttr.NormalFormat]));
	}

	u32 color_mode[2] = { m_VtxDesc.Color0, m_VtxDesc.Color1 };
	for (int i = 0; i < 2; i++)
	{
		if (color_mode[i])
		{
			dest->append(StringFromFormat("C%i_%i_%s_%s_", i, m_VtxAttr.color[i].Elements, posMode[color_mode[i]], colorFormat[m_VtxAttr.color[i].Comp]));
		}
	}
	const u32 tex_mode[8] = {
		m_VtxDesc.Tex0Coord, m_VtxDesc.Tex1Coord, m_VtxDesc.Tex2Coord, m_VtxDesc.Tex3Coord,
		m_VtxDesc.Tex4Coord, m_VtxDesc.Tex5Coord, m_VtxDesc.Tex6Coord, (const u32)((m_VtxDesc.Hex >> 31) & 3)
	};
	const u32 tex_mtxidx[8] = {
		m_VtxDesc.Tex0MatIdx, m_VtxDesc.Tex1MatIdx, m_VtxDesc.Tex2MatIdx, m_VtxDesc.Tex3MatIdx,
		m_VtxDesc.Tex4MatIdx, m_VtxDesc.Tex5MatIdx, m_VtxDesc.Tex6MatIdx, m_VtxDesc.Tex7MatIdx
	};
	for (int i = 0; i < 8; i++)
	{
		if (tex_mode[i] || tex_mtxidx[i])
		{
			dest->append(StringFromFormat("T%i_mtx%i_%i_%s_%s_",
				i, tex_mtxidx[i], m_VtxAttr.texCoord[i].Elements, posMode[tex_mode[i]], posFormats[m_VtxAttr.texCoord[i].Format]));
		}
	}
}

void VertexLoader::AppendToString(std::string *dest) const
{
	dest->append(StringFromFormat("%ib ", m_VertexSize));
	GetName(dest);
	dest->append(StringFromFormat(" - %i v\n", m_numLoadedVertices));
}
