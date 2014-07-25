// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino

#include "VideoCommon/VertexLoader_TextCoord.h"
#include "VideoCommon/VertexLoader_TextCoordFuncs.h"

bool VertexLoader_TextCoord::Initialized = false;

void LOADERDECL TexCoord_Read_Dummy()
{
	g_PipelineState.tcIndex++;
}

template <typename T, s32 N>
void LOADERDECL TexCoord_ReadDirect()
{
	_TexCoord_ReadDirect<T, N>(g_PipelineState);
}

template <typename I, typename T, s32 N>
void LOADERDECL TexCoord_ReadIndex()
{
	_TexCoord_ReadIndex<I, T, N>(g_PipelineState);
}

#if _M_SSE >= 0x301
template <typename I>
void LOADERDECL TexCoord_ReadIndex_Float2_SSSE3()
{
	_TexCoord_ReadIndex_Float2_SSSE3<I>(g_PipelineState);
}

void LOADERDECL TexCoord_ReadDirect_Float2_SSSE3()
{
	_TexCoord_ReadDirect_Float2_SSSE3(g_PipelineState);
}

#endif

#if _M_SSE >= 0x401
template <typename I, bool Signed>
void LOADERDECL TexCoord_ReadIndex_16x2_SSE4()
{
	_TexCoord_ReadIndex_16x2_SSE4<I, Signed>(g_PipelineState);
}

template <bool Signed>
void LOADERDECL TexCoord_ReadDirect_16x2_SSE4()
{
	_TexCoord_ReadDirect_16x2_SSE4<Signed>(g_PipelineState);
}
#endif

static TPipelineFunction tableReadTexCoord[4][5][2] = {
		{
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL }
		},
		{
			{ TexCoord_ReadDirect<u8, 1>, TexCoord_ReadDirect<u8, 2> },
			{ TexCoord_ReadDirect<s8, 1>, TexCoord_ReadDirect<s8, 2> },
			{ TexCoord_ReadDirect<u16, 1>, TexCoord_ReadDirect<u16, 2> },
			{ TexCoord_ReadDirect<s16, 1>, TexCoord_ReadDirect<s16, 2> },
			{ TexCoord_ReadDirect<float, 1>, TexCoord_ReadDirect<float, 2> }
		},
		{
			{ TexCoord_ReadIndex<u8, u8, 1>, TexCoord_ReadIndex<u8, u8, 2> },
			{ TexCoord_ReadIndex<u8, s8, 1>, TexCoord_ReadIndex<u8, s8, 2> },
			{ TexCoord_ReadIndex<u8, u16, 1>, TexCoord_ReadIndex<u8, u16, 2> },
			{ TexCoord_ReadIndex<u8, s16, 1>, TexCoord_ReadIndex<u8, s16, 2> },
			{ TexCoord_ReadIndex<u8, float, 1>, TexCoord_ReadIndex<u8, float, 2> }
		},
		{
			{ TexCoord_ReadIndex<u16, u8, 1>, TexCoord_ReadIndex<u16, u8, 2> },
			{ TexCoord_ReadIndex<u16, s8, 1>, TexCoord_ReadIndex<u16, s8, 2> },
			{ TexCoord_ReadIndex<u16, u16, 1>, TexCoord_ReadIndex<u16, u16, 2> },
			{ TexCoord_ReadIndex<u16, s16, 1>, TexCoord_ReadIndex<u16, s16, 2> },
			{ TexCoord_ReadIndex<u16, float, 1>, TexCoord_ReadIndex<u16, float, 2> }
		}
};

static const char* tableReadTexCoordSTR[4][5][2] = {
		{
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL },
			{ NULL, NULL }
		},
		{
			{
				"\t_TexCoord_ReadDirect<u8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadDirect<u8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadDirect<s8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadDirect<s8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadDirect<u16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadDirect_16x2_SSE4<false>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadDirect<u16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadDirect<s16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadDirect_16x2_SSE4<true>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadDirect<s16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadDirect<float, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x301\n"
				"\tif (iSSE >= 0x301)\n"
				"\t{\n\t\t_TexCoord_ReadDirect_Float2_SSSE3(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadDirect<float, 2>(pipelinestate);\n\t}\n"
			}
		},
		{
			{
				"\t_TexCoord_ReadIndex<u8, u8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadIndex<u8, u8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadIndex<u8, s8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadIndex<u8, s8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadIndex<u8, u16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_16x2_SSE4<u8, false>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u8, u16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadIndex<u8, s16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_16x2_SSE4<u8, true>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u8, s16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadIndex<u8, float, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x301\n"
				"\tif (iSSE >= 0x301)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_Float2_SSSE3<u8>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u8, float, 2>(pipelinestate);\n\t}\n"
			}
		},
		{
			{
				"\t_TexCoord_ReadIndex<u16, u8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadIndex<u16, u8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadIndex<u16, s8, 1>(pipelinestate);\n",
				"\t_TexCoord_ReadIndex<u16, s8, 2>(pipelinestate);\n"
			},
			{
				"\t_TexCoord_ReadIndex<u16, u16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_16x2_SSE4<u16, false>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u16, u16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadIndex<u16, s16, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif (iSSE >= 0x401)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_16x2_SSE4<u16, true>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u16, s16, 2>(pipelinestate);\n\t}\n"
			},
			{
				"\t_TexCoord_ReadIndex<u16, float, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x301\n"
				"\tif (iSSE >= 0x301)\n"
				"\t{\n\t\t_TexCoord_ReadIndex_Float2_SSSE3<u16>(pipelinestate);\n\t}\n\telse\n"
				"#endif\n"
				"\t{\n\t\t_TexCoord_ReadIndex<u16, float, 2>(pipelinestate);\n\t}\n"
			}
		}
};

static s32 tableReadTexCoordVertexSize[4][5][2] = {
		{
			{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }
		},
		{
			{ 1, 2 }, { 1, 2 }, { 2, 4 }, { 2, 4 }, { 4, 8 }
		},
		{
			{ 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 }
		},
		{
			{ 2, 2 }, { 2, 2 }, { 2, 2 }, { 2, 2 }, { 2, 2 }
		}
};

void VertexLoader_TextCoord::Init(void)
{
	if (Initialized)
	{
		return;
	}
	Initialized = true;
#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
		tableReadTexCoord[DIRECT][FORMAT_FLOAT][TC_ELEMENTS_2] = TexCoord_ReadDirect_Float2_SSSE3;
		tableReadTexCoord[INDEX8][FORMAT_FLOAT][TC_ELEMENTS_2] = TexCoord_ReadIndex_Float2_SSSE3<u8>;
		tableReadTexCoord[INDEX16][FORMAT_FLOAT][TC_ELEMENTS_2] = TexCoord_ReadIndex_Float2_SSSE3<u16>;
	}
#endif

#if _M_SSE >= 0x401

	if (cpu_info.bSSE4_1)
	{
		tableReadTexCoord[DIRECT][FORMAT_USHORT][TC_ELEMENTS_2] = TexCoord_ReadDirect_16x2_SSE4<false>;
		tableReadTexCoord[DIRECT][FORMAT_SHORT][TC_ELEMENTS_2] = TexCoord_ReadDirect_16x2_SSE4<true>;
		tableReadTexCoord[INDEX8][FORMAT_USHORT][TC_ELEMENTS_2] = TexCoord_ReadIndex_16x2_SSE4<u8, false>;
		tableReadTexCoord[INDEX8][FORMAT_SHORT][TC_ELEMENTS_2] = TexCoord_ReadIndex_16x2_SSE4<u8, true>;
		tableReadTexCoord[INDEX16][FORMAT_USHORT][TC_ELEMENTS_2] = TexCoord_ReadIndex_16x2_SSE4<u16, false>;
		tableReadTexCoord[INDEX16][FORMAT_SHORT][TC_ELEMENTS_2] = TexCoord_ReadIndex_16x2_SSE4<u16, true>;
	}
#endif
}

u32 VertexLoader_TextCoord::GetSize(u32 _type, u32 _format, u32 _elements)
{
	return tableReadTexCoordVertexSize[_type][_format][_elements];
}

TPipelineFunction VertexLoader_TextCoord::GetFunction(u32 _type, u32 _format, u32 _elements)
{
	return tableReadTexCoord[_type][_format][_elements];
}

void VertexLoader_TextCoord::GetFunctionSTR(std::string *dest, u32 _type, u32 _format, u32 _elements)
{
	dest->append(tableReadTexCoordSTR[_type][_format][_elements]);
}

TPipelineFunction VertexLoader_TextCoord::GetDummyFunction()
{
	return TexCoord_Read_Dummy;
}

void VertexLoader_TextCoord::GetDummyFunctionSTR(std::string *dest)
{
	dest->append("\tpipelinestate.tcIndex++;\n");
}