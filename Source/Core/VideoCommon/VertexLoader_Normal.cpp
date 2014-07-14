// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino

#include "VideoCommon/VertexLoader_Normal.h"
#include "VideoCommon/VertexLoader_NormalFuncs.h"

enum ENormalType
{
	NRM_NOT_PRESENT = 0,
	NRM_DIRECT = 1,
	NRM_INDEX8 = 2,
	NRM_INDEX16 = 3,
	NUM_NRM_TYPE
};

#define NUM_NRM_FORMAT 5

enum ENormalElements
{
	NRM_NBT = 0,
	NRM_NBT3 = 1,
	NUM_NRM_ELEMENTS
};

enum ENormalIndices
{
	NRM_INDICES1 = 0,
	NRM_INDICES3 = 1,
	NUM_NRM_INDICES
};

struct Set
{
	template <typename T>
	void operator=(const T&)
	{
		gc_size = T::size;
		function = T::function;
	}

	int gc_size;
	TPipelineFunction function;
};

static Set m_Table[NUM_NRM_TYPE][NUM_NRM_INDICES][NUM_NRM_ELEMENTS][NUM_NRM_FORMAT];
static const char* m_TableSTR[NUM_NRM_TYPE][NUM_NRM_INDICES][NUM_NRM_ELEMENTS][NUM_NRM_FORMAT] =
{
	{
		{
			{ NULL, NULL, NULL, NULL, NULL },
			{ NULL, NULL, NULL, NULL, NULL }
		},
		{
			{ NULL, NULL, NULL, NULL, NULL },
			{ NULL, NULL, NULL, NULL, NULL }
		}
	},
	{
		{
			{
				"\t_Normal_Direct<u8, 1>(pipelinestate);\n",
				"\t_Normal_Direct<s8, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_U16_SSSE4<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<u16, 1>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_S16_SSSE4<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<s16, 1>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Direct_FLOAT_SSSE3<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<float, 1>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Direct<u8, 3>(pipelinestate);\n",
				"\t_Normal_Direct<s8, 3>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_U16_SSSE4<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<u16, 3>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_S16_SSSE4<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<s16, 3>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Direct_FLOAT_SSSE3<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<float, 3>(pipelinestate);\n\t}\n"
			}
		},
		{
			{
				"\t_Normal_Direct<u8, 1>(pipelinestate);\n",
				"\t_Normal_Direct<s8, 1>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_U16_SSSE4<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<u16, 1>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_S16_SSSE4<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<s16, 1>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Direct_FLOAT_SSSE3<1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<float, 1>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Direct<u8, 3>(pipelinestate);\n",
				"\t_Normal_Direct<s8, 3>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_U16_SSSE4<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<u16, 3>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Direct_S16_SSSE4<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<s16, 3>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Direct_FLOAT_SSSE3<3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Direct<float, 3>(pipelinestate);\n\t}\n"
			}
		}
	},
	{
		{
			{
				"\t_Normal_Index_Offset<u8, u8, 1, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, u16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Index_Offset<u8, u8, 3, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u8, s8, 3, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u8, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, u16, 3, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u8, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, s16, 3, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u8, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, float, 3, 0>(pipelinestate);\n\t}\n"
			}
		},
		{
			{
				"\t_Normal_Index_Offset<u8, u8, 1, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u8, s8, 1, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, u16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, s16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u8, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u8, float, 1, 0>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Index_Offset3<u8, u8>(pipelinestate);\n",
				"\t_Normal_Index_Offset3<u8, s8>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index3_U16_SSE4<u8>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u8, u16>(pipelinestate);\n"
				"\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index3_S16_SSE4<u8>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u8, s16>(pipelinestate);\n"
				"\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index3_FLOAT_SSSE3<u8>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u8, float>(pipelinestate);\n"
				"\t}\n"
			}
		}
	},
	{
		{
			{
				"\t_Normal_Index_Offset<u16, u8, 1, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, u16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Index_Offset<u16, u8, 3, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u16, s8, 3, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u16, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, u16, 3, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u16, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, s16, 3, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u16, 3>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, float, 3, 0>(pipelinestate);\n\t}\n"
			}
		},
		{
			{
				"\t_Normal_Index_Offset<u16, u8, 1, 0>(pipelinestate);\n",
				"\t_Normal_Index_Offset<u16, s8, 1, 0>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_U16_SSE4<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, u16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index_S16_SSE4<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, s16, 1, 0>(pipelinestate);\n\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index_FLOAT_SSSE3<u16, 1>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n\t\t_Normal_Index_Offset<u16, float, 1, 0>(pipelinestate);\n\t}\n"
			},
			{
				"\t_Normal_Index_Offset3<u16, u8>(pipelinestate);\n",
				"\t_Normal_Index_Offset3<u16, s8>(pipelinestate);\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index3_U16_SSE4<u16>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u16, u16>(pipelinestate);\n"
				"\t}\n",
				"#if _M_SSE >= 0x401\n"
				"\tif(iSSE >= 0x401)\n"
				"\t{\n\t\t_Normal_Index3_S16_SSE4<u16>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u16, s16>(pipelinestate);\n"
				"\t}\n",
				"#if _M_SSE >= 0x301\n"
				"\tif(iSSE >= 0x301)\n"
				"\t{\n\t\t_Normal_Index3_FLOAT_SSSE3<u16>(pipelinestate);\n\t}\n"
				"\telse\n"
				"#endif\n"
				"\t{\n"
				"\t_Normal_Index_Offset3<u16, float>(pipelinestate);\n"
				"\t}\n"
			}
		}
	}
};

bool VertexLoader_Normal::Initialized = false;
namespace
{

	template <typename T, int N>
	struct Normal_Direct
	{
		static void LOADERDECL function()
		{
			_Normal_Direct<T, N>(g_PipelineState);
		}
		static const int size = sizeof(T) * N * 3;
	};

	template <typename I, typename T, int N>
	struct Normal_Index
	{
		static void LOADERDECL function()
		{
			_Normal_Index_Offset<I, T, N, 0>(g_PipelineState);
		}

		static const int size = sizeof(I);
	};

	template <typename I, typename T>
	struct Normal_Index_Indices3
	{
		static void LOADERDECL function()
		{
			_Normal_Index_Offset3<I, T>(g_PipelineState);
		}

		static const int size = sizeof(I) * 3;
	};

#if _M_SSE >= 0x301
	template <int N>
	struct Normal_Direct_FLOAT_SSSE3
	{
		static void LOADERDECL function()
		{
			_Normal_Direct_FLOAT_SSSE3<N>(g_PipelineState);
		}
		static const int size = sizeof(float) * N * 3;
	};

	template <typename I, int N>
	struct Normal_Index_FLOAT_SSSE3
	{
		static void LOADERDECL function()
		{
			_Normal_Index_FLOAT_SSSE3<I, N>(g_PipelineState);
		}

		static const int size = sizeof(I);
	};

	template <typename I>
	struct Normal_Index3_FLOAT_SSSE3
	{
		static void LOADERDECL function()
		{
			_Normal_Index3_FLOAT_SSSE3<I>(g_PipelineState);
		}
		static const int size = sizeof(I) * 3;
	};
#endif

#if _M_SSE >= 0x401

	template <int N>
	struct Normal_Direct_S16_SSSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Direct_S16_SSSE4<N>(g_PipelineState);
		}

		static const int size = sizeof(s16) * N * 3;
	};

	template <int N>
	struct Normal_Direct_U16_SSSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Direct_U16_SSSE4<N>(g_PipelineState);
		}

		static const int size = sizeof(u16) * N * 3;
	};

	template <typename I, int N>
	struct Normal_Index_S16_SSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Index_S16_SSE4<I, N>(g_PipelineState);
		}

		static const int size = sizeof(I);
	};

	template <typename I, int N>
	struct Normal_Index_U16_SSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Index_U16_SSE4<I, N>(g_PipelineState);
		}

		static const int size = sizeof(I);
	};

	template <typename I>
	struct Normal_Index3_S16_SSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Index3_S16_SSE4<I>(g_PipelineState);
		}

		static const int size = sizeof(I) * 3;
	};

	template <typename I>
	struct Normal_Index3_U16_SSE4
	{
		static void LOADERDECL function()
		{
			_Normal_Index3_U16_SSE4<I>(g_PipelineState);
		}

		static const int size = sizeof(I) * 3;
	};
#endif


}

void VertexLoader_Normal::Init(void)
{
	if (Initialized)
	{
		return;
	}
	Initialized = true;
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_UBYTE] = Normal_Direct<u8, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_BYTE] = Normal_Direct<s8, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Direct<u16, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Direct<s16, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Direct<float, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] = Normal_Direct<u8, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_BYTE] = Normal_Direct<s8, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Direct<u16, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Direct<s16, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct<float, 3>();

	// Same as above
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_UBYTE] = Normal_Direct<u8, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_BYTE] = Normal_Direct<s8, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Direct<u16, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Direct<s16, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Direct<float, 1>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] = Normal_Direct<u8, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_BYTE] = Normal_Direct<s8, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Direct<u16, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Direct<s16, 3>();
	m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct<float, 3>();

	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_UBYTE] = Normal_Index<u8, u8, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_BYTE] = Normal_Index<u8, s8, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index<u8, u16, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index<u8, s16, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index<u8, float, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] = Normal_Index<u8, u8, 3>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_BYTE] = Normal_Index<u8, s8, 3>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index<u8, u16, 3>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index<u8, s16, 3>();
	m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index<u8, float, 3>();

	// Same as above for NRM_NBT
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_UBYTE] = Normal_Index<u8, u8, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_BYTE] = Normal_Index<u8, s8, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index<u8, u16, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index<u8, s16, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index<u8, float, 1>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] = Normal_Index_Indices3<u8, u8>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_BYTE] = Normal_Index_Indices3<u8, s8>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index_Indices3<u8, u16>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index_Indices3<u8, s16>();
	m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_Indices3<u8, float>();

	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_UBYTE] = Normal_Index<u16, u8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_BYTE] = Normal_Index<u16, s8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index<u16, u16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index<u16, s16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index<u16, float, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_UBYTE] = Normal_Index<u16, u8, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_BYTE] = Normal_Index<u16, s8, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index<u16, u16, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index<u16, s16, 3>();
	m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index<u16, float, 3>();

	// Same as above for NRM_NBT
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_UBYTE] = Normal_Index<u16, u8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_BYTE] = Normal_Index<u16, s8, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index<u16, u16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index<u16, s16, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index<u16, float, 1>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_UBYTE] = Normal_Index_Indices3<u16, u8>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_BYTE] = Normal_Index_Indices3<u16, s8>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index_Indices3<u16, u16>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index_Indices3<u16, s16>();
	m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_Indices3<u16, float>();


#if _M_SSE >= 0x301
	if (cpu_info.bSSSE3)
	{
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<1>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<3>();

		// Same as above
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Direct_FLOAT_SSSE3<3>();

		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index3_FLOAT_SSSE3<u8>();

		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_FLOAT] = Normal_Index_FLOAT_SSSE3<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_FLOAT] = Normal_Index3_FLOAT_SSSE3<u16>();
	}
#endif
#if _M_SSE >= 0x401
	if (cpu_info.bSSE4_1)
	{
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<3>();
		m_Table[NRM_DIRECT][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<3>();

		// Same as above
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<1>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Direct_U16_SSSE4<3>();
		m_Table[NRM_DIRECT][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Direct_S16_SSSE4<3>();

		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 3>();
		m_Table[NRM_INDEX8][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u8, 1>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index3_U16_SSE4<u8>();
		m_Table[NRM_INDEX8][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index3_S16_SSE4<u8>();

		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 3>();
		m_Table[NRM_INDEX16][NRM_INDICES1][NRM_NBT3][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 3>();

		// Same as above for NRM_NBT
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_USHORT] = Normal_Index_U16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT][FORMAT_SHORT] = Normal_Index_S16_SSE4<u16, 1>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_USHORT] = Normal_Index3_U16_SSE4<u16>();
		m_Table[NRM_INDEX16][NRM_INDICES3][NRM_NBT3][FORMAT_SHORT] = Normal_Index3_S16_SSE4<u16>();
	}
#endif

}

u32 VertexLoader_Normal::GetSize(u32 _type, u32 _format, u32 _elements, u32 _index3)
{
	return m_Table[_type][_index3][_elements][_format].gc_size;
}

TPipelineFunction VertexLoader_Normal::GetFunction(u32 _type, u32 _format, u32 _elements, u32 _index3)
{
	TPipelineFunction pFunc = m_Table[_type][_index3][_elements][_format].function;
	return pFunc;
}

void VertexLoader_Normal::GetFunctionSTR(std::string *dest, u32 _type, u32 _format, u32 _elements, u32 _index3)
{
	dest->append(m_TableSTR[_type][_index3][_elements][_format]);
}
