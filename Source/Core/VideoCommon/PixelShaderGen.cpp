// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <stdio.h>
#include <cmath>
#include <assert.h>
#include <locale.h>
#ifdef __APPLE__
#include <xlocale.h>
#endif

#include "LightingShaderGen.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/XFMemory.h"  // for texture projection mode
#include "VideoCommon/BPMemory.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/TextureCacheBase.h"

//   old tev->pixelshader notes
//
//   color for this stage (alpha, color) is given by bpmem.tevorders[0].colorchan0
//   konstant for this stage (alpha, color) is given by bpmem.tevksel
//   inputs are given by bpmem.combiners[0].colorC.a/b/c/d     << could be current channel color
//   according to GXTevColorArg table above
//   output is given by .outreg
//   tevtemp is set according to swapmodetables and

// Tev Source States
enum tevSources
{
	CPREV  = 0,
	APREV = 1,
	C0 = 2,
	A0 = 3,
	C1 = 4,
	A1 = 5,
	C2 = 6,
	A2 = 7,
	TEXC = 8,
	TEXA = 9,
	RASC = 10,
	RASA = 11,
	ONE = 12,
	HALF = 13,
	KONST = 14,
	ZERO = 15,
	SOURCECOUNT = 16
};

static bool TevOverflowState[tevSources::SOURCECOUNT];
static bool tevRascolor0_Expanded = false;
static bool tevRascolor1_Expanded = false;
inline void InitializeRegisterState()
{
	TevOverflowState[tevSources::CPREV] = true;
	TevOverflowState[tevSources::APREV] = true;
	TevOverflowState[tevSources::C0] = true;
	TevOverflowState[tevSources::A0] = true;
	TevOverflowState[tevSources::C1] = true;
	TevOverflowState[tevSources::A1] = true;
	TevOverflowState[tevSources::C2] = true;
	TevOverflowState[tevSources::A2] = true;
	TevOverflowState[tevSources::TEXC] = false;
	TevOverflowState[tevSources::TEXA] = false;
	TevOverflowState[tevSources::RASC] = true;
	TevOverflowState[tevSources::RASA] = true;
	TevOverflowState[tevSources::ONE] = false;
	TevOverflowState[tevSources::HALF] = false;
	TevOverflowState[tevSources::KONST] = true;
	TevOverflowState[tevSources::ZERO] = false;
	tevRascolor0_Expanded = false;
	tevRascolor1_Expanded = false;
}

static const char *tevKSelTableC[] = // KCSEL
{
	"255.0,255.0,255.0",	// 1   = 0x00
	"223.0,223.0,223.0",	// 7_8 = 0x01
	"191.0,191.0,191.0",	// 3_4 = 0x02
	"159.0,159.0,159.0",	// 5_8 = 0x03
	"128.0,128.0,128.0",	// 1_2 = 0x04
	"96.0,96.0,96.0",		// 3_8 = 0x05
	"64.0,64.0,64.0",		// 1_4 = 0x06
	"32.0,32.0,32.0",		// 1_8 = 0x07
	"0.0,0.0,0.0", // 0x08
	"0.0,0.0,0.0", // 0x09
	"0.0,0.0,0.0", // 0x0a
	"0.0,0.0,0.0", // 0x0b
	I_KCOLORS "[0].rgb", // K0 = 0x0C
	I_KCOLORS "[1].rgb", // K1 = 0x0D
	I_KCOLORS "[2].rgb", // K2 = 0x0E
	I_KCOLORS "[3].rgb", // K3 = 0x0F
	I_KCOLORS "[0].rrr", // K0_R = 0x10
	I_KCOLORS "[1].rrr", // K1_R = 0x11
	I_KCOLORS "[2].rrr", // K2_R = 0x12
	I_KCOLORS "[3].rrr", // K3_R = 0x13
	I_KCOLORS "[0].ggg", // K0_G = 0x14
	I_KCOLORS "[1].ggg", // K1_G = 0x15
	I_KCOLORS "[2].ggg", // K2_G = 0x16
	I_KCOLORS "[3].ggg", // K3_G = 0x17
	I_KCOLORS "[0].bbb", // K0_B = 0x18
	I_KCOLORS "[1].bbb", // K1_B = 0x19
	I_KCOLORS "[2].bbb", // K2_B = 0x1A
	I_KCOLORS "[3].bbb", // K3_B = 0x1B
	I_KCOLORS "[0].aaa", // K0_A = 0x1C
	I_KCOLORS "[1].aaa", // K1_A = 0x1D
	I_KCOLORS "[2].aaa", // K2_A = 0x1E
	I_KCOLORS "[3].aaa", // K3_A = 0x1F
};

static const char *tevKSelTableA[] = // KASEL
{
	"255.0",	// 1   = 0x00
	"223.0",	// 7_8 = 0x01
	"191.0",	// 3_4 = 0x02
	"159.0",	// 5_8 = 0x03
	"128.0",	// 1_2 = 0x04
	"96.0",		// 3_8 = 0x05
	"64.0",		// 1_4 = 0x06
	"32.0",		// 1_8 = 0x07
	"0.0", // 0x08
	"0.0", // 0x09
	"0.0", // 0x0a
	"0.0", // 0x0b
	"0.0", // 0x0c
	"0.0", // 0x0d
	"0.0", // 0x0e
	"0.0", // 0x0f
	I_KCOLORS "[0].r", // K0_R = 0x10
	I_KCOLORS "[1].r", // K1_R = 0x11
	I_KCOLORS "[2].r", // K2_R = 0x12
	I_KCOLORS "[3].r", // K3_R = 0x13
	I_KCOLORS "[0].g", // K0_G = 0x14
	I_KCOLORS "[1].g", // K1_G = 0x15
	I_KCOLORS "[2].g", // K2_G = 0x16
	I_KCOLORS "[3].g", // K3_G = 0x17
	I_KCOLORS "[0].b", // K0_B = 0x18
	I_KCOLORS "[1].b", // K1_B = 0x19
	I_KCOLORS "[2].b", // K2_B = 0x1A
	I_KCOLORS "[3].b", // K3_B = 0x1B
	I_KCOLORS "[0].a", // K0_A = 0x1C
	I_KCOLORS "[1].a", // K1_A = 0x1D
	I_KCOLORS "[2].a", // K2_A = 0x1E
	I_KCOLORS "[3].a", // K3_A = 0x1F
};

static const char *tevCInputTable[] = // CC
{
	"prev.rgb",         // CPREV,
	"prev.aaa",         // APREV,
	"c0.rgb",           // C0,
	"c0.aaa",           // A0,
	"c1.rgb",           // C1,
	"c1.aaa",		// A1,
	"c2.rgb",		// C2,
	"c2.aaa",		// A2,
	"tex_t.rgb",	// TEXC,
	"tex_t.aaa",	// TEXA,
	"ras_t.rgb",	// RASC,
	"ras_t.aaa",	// RASA,
	"255.0,255.0,255.0",	// ONE
	"128.0,128.0,128.0",	// HALF
	"konst_t.rgb",	// KONST
	"0.0,0.0,0.0"	// ZERO
};

static const char *tevAInputTable[] = // CA
{
	"prev.a",	// APREV,
	"c0.a",		// A0,
	"c1.a",		// A1,
	"c2.a",		// A2,
	"tex_t.a",	// TEXA,
	"ras_t.a",	// RASA,
	"konst_t.a",// KONST,  (hw1 had quarter)
	"0.0"		// ZERO
};

static const tevSources AInputSourceMap[] =
{
	tevSources::APREV,	// APREV,
	tevSources::A0,		// A0,
	tevSources::A1,		// A1,
	tevSources::A2,		// A2,
	tevSources::TEXA,	// TEXA,
	tevSources::RASA,	// RASA,
	tevSources::KONST,	// KONST,  (hw1 had quarter)
	tevSources::ZERO	// ZERO
};

static const char *tevRasTable[] =
{
	"colors_0",
	"colors_1",
	"float4(0.0,0.0,0.0,0.0)", //2
	"float4(0.0,0.0,0.0,0.0)", //3
	"float4(0.0,0.0,0.0,0.0)", //4
	"float4(a_bump,a_bump,a_bump,a_bump)", // use bump alpha
	"round(float4(a_bump,a_bump,a_bump,a_bump)*(255.0/248))", //normalized
	"float4(0.0,0.0,0.0,0.0)", // zero
};

//static const char *tevTexFunc[] = { "tex2D", "texRECT" };

static const char *tevCOutputTable[] = { "prev.rgb", "c0.rgb", "c1.rgb", "c2.rgb" };
static const tevSources tevCOutputSourceMap[] = { tevSources::CPREV , tevSources::C0, tevSources::C1, tevSources::C2 };
static const char *tevAOutputTable[] = { "prev.a", "c0.a", "c1.a", "c2.a" };
static const tevSources tevAOutputSourceMap[] = { tevSources::APREV, tevSources::A0, tevSources::A1, tevSources::A2 };

static const char* headerUtil = "float4 CHK_O_U8(float4 x)\n{\nreturn frac(((x) + 1024.0) * (1.0/256.0)) * 256.0;\n}\n"
"float2 BSH(float2 x, float2 n)\n"
"{\n"
"float2 z = exp2(n);\n"
"float2 y = (1.0f / z) - 1.0;\n"
"x.x = (x.x - ((z.x < 1.0 && x.x < 0.0) ? y.x : 0.0)) * z.x;\n"
"x.y = (x.y - ((z.y < 1.0 && x.y < 0.0) ? y.y : 0.0)) * z.y;\n"
"return trunc(x);\n"
"}\n"
"float2 BSHR(float2 x, float2 y, float2 z)\n"
"{\n"
"x.x = (x.x - ((x.x < 0.0) ? y.x : 0.0)) * z.x;\n"
"x.y = (x.y - ((x.y < 0.0) ? y.y : 0.0)) * z.y;\n"
"return trunc(x);\n"
"}\n"
// Fastmod implementation with the restriction that "y" must be always greater than 0
"float fastmod( float x, float y )\n"
"{\n"
"y = sign(x) * y;\n"
"return frac(x/y)*y;\n"
"}\n";

static char text[16384];

template<class T, bool Write_Code, API_TYPE ApiType> inline void WriteStage(T& out, pixel_shader_uid_data& uid_data, int n, const char swapModeTable[4][5]);
template<class T, bool Write_Code, API_TYPE ApiType> inline void SampleTexture(T& out, const char *texcoords, const char *texswap, int texmap);
template<class T, bool Write_Code, API_TYPE ApiType> inline void WriteAlphaTest(T& out, pixel_shader_uid_data& uid_data, DSTALPHA_MODE dstAlphaMode, bool per_pixel_depth);
template<class T, bool Write_Code> inline void WriteFog(T& out, pixel_shader_uid_data& uid_data);

template<class T, bool Write_Code, API_TYPE ApiType>
inline void GeneratePixelShader(T& out, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	// Non-uid template parameters will write to the dummy data (=> gets optimized out)
	pixel_shader_uid_data dummy_data;
	pixel_shader_uid_data& uid_data = (&out.template GetUidData<pixel_shader_uid_data>() != NULL)
		? out.template GetUidData<pixel_shader_uid_data>() : dummy_data;
#ifndef ANDROID	
	locale_t locale;
	locale_t old_locale;
#endif
	if (Write_Code)
	{
		out.SetBuffer(text);
#ifndef ANDROID			
		locale = newlocale(LC_NUMERIC_MASK, "C", NULL); // New locale for compilation
		old_locale = uselocale(locale); // Apply the locale for this thread			
#endif
		text[sizeof(text)-1] = 0x7C;  // canary
	}

	unsigned int numStages = bpmem.genMode.numtevstages + 1;
	unsigned int numTexgen = bpmem.genMode.numtexgens;

	const bool forced_early_z = g_ActiveConfig.backend_info.bSupportsEarlyZ && bpmem.UseEarlyDepthTest();
	const bool per_pixel_depth = (bpmem.ztex2.op != ZTEXTURE_DISABLE && bpmem.UseLateDepthTest()) || (!g_ActiveConfig.bFastDepthCalc && bpmem.zmode.testenable && !forced_early_z && dstAlphaMode != DSTALPHA_NULL);
	bool enable_pl = g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting;
	uid_data.pixel_lighting = enable_pl;
	uid_data.dstAlphaMode = dstAlphaMode;
	uid_data.genMode_numindstages = bpmem.genMode.numindstages;
	uid_data.genMode_numtevstages = bpmem.genMode.numtevstages;
	uid_data.genMode_numtexgens = bpmem.genMode.numtexgens;
	if (ApiType == API_D3D11)
	{
		uid_data.tex_pcformat.samp0 = TextureCache::getStagePCelementCount(0);
		uid_data.tex_pcformat.samp1 = TextureCache::getStagePCelementCount(1);
		uid_data.tex_pcformat.samp2 = TextureCache::getStagePCelementCount(2);
		uid_data.tex_pcformat.samp3 = TextureCache::getStagePCelementCount(3);
		uid_data.tex_pcformat.samp4 = TextureCache::getStagePCelementCount(4);
		uid_data.tex_pcformat.samp5 = TextureCache::getStagePCelementCount(5);
		uid_data.tex_pcformat.samp6 = TextureCache::getStagePCelementCount(6);
		uid_data.tex_pcformat.samp7 = TextureCache::getStagePCelementCount(7);
	}
	if (Write_Code)
	{
		InitializeRegisterState();
		out.Write("//Pixel Shader for TEV stages\n");
		out.Write("//%i TEV stages, %i texgens, %i IND stages\n",
			numStages, numTexgen, bpmem.genMode.numindstages);
		out.Write(headerUtil);

		if (ApiType == API_OPENGL)
		{
			// Declare samplers
			for (u32 i = 0; i < 8; ++i)
				out.Write("uniform sampler2D samp%d;\n", i);
		}
		else
		{
			// Declare samplers
			for (u32 i = 0; i < 8; ++i)
				out.Write("%s samp%d : register(s%d);\n", (ApiType == API_D3D11) ? "sampler" : "uniform sampler2D", i, i);

			if (ApiType == API_D3D11)
			{
				out.Write("\n");
				for (u32 i = 0; i < 8; ++i)
				{
					out.Write("Texture2D Tex%d : register(t%d);\n", i, i);
				}
			}
		}
		out.Write("\n");

		if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
			out.Write("layout(std140) uniform PSBlock {\n");

		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_COLORS, "float4", I_COLORS "[4]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_KCOLORS, "float4", I_KCOLORS "[4]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_ALPHA, "float4", I_ALPHA);
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_TEXDIMS, "float4", I_TEXDIMS "[8]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_ZBIAS, "float4", I_ZBIAS "[2]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_INDTEXSCALE, "float4", I_INDTEXSCALE "[4]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_INDTEXMTX, "float4", I_INDTEXMTX "[6]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_FOG, "float4", I_FOG "[3]");

		if (enable_pl)
		{
			DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_PLIGHTS, "float4", I_PLIGHTS "[40]");
			DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_PMATERIALS, "float4", I_PMATERIALS "[4]");
		}

		if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
			out.Write("};\n");

		if (ApiType == API_OPENGL)
		{
			out.Write("out vec4 ocol0;\n");
			if (dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND)
				out.Write("out vec4 ocol1;\n");

			if (per_pixel_depth)
				out.Write("#define depth gl_FragDepth\n");

			out.Write("VARYIN float4 colors_02;\n");
			out.Write("VARYIN float4 colors_12;\n");

			// compute window position if needed because binding semantic WPOS is not widely supported
			// Let's set up attributes
			if (numTexgen < 7)
			{
				for (u32 i = 0; i < numTexgen; ++i)
				{
					out.Write("VARYIN float3 uv%d_2;\n", i);
				}
				out.Write("VARYIN float4 clipPos_2;\n");
				if (enable_pl)
				{
					out.Write("VARYIN float4 Normal_2;\n");
				}
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (enable_pl)
				{
					for (u32 i = 0; i < 8; ++i)
					{
						out.Write("VARYIN float4 uv%d_2;\n", i);
					}
				}
				else
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("VARYIN float%d uv%d_2;\n", i < 4 ? 4 : 3, i);
					}
				}
				out.Write("float4 clipPos;\n");
			}

			if (forced_early_z)
			{
				// HACK: This doesn't force the driver to write to depth buffer if alpha test fails.
				// It just allows it, but it seems that all drivers do.
				out.Write("layout(early_fragment_tests) in;\n");
			}
			out.Write("void main()\n{\n");
		}
		else
		{
			if (forced_early_z)
			{
				out.Write("[earlydepthstencil]\n");
			}
			out.Write("void main(\n");
			if (ApiType != API_D3D11)
			{
				out.Write("  out float4 ocol0 : COLOR0,%s%s\n  in float4 rawpos : %s,\n",
					dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND ? "\n  out float4 ocol1 : COLOR1," : "",
					per_pixel_depth ? "\n  out float depth : DEPTH," : "",
					ApiType == API_D3D9_SM20 ? "POSITION" : "VPOS");
			}
			else
			{
				out.Write("  out float4 ocol0 : SV_Target0,%s%s\n  in float4 rawpos : SV_Position,\n",
					dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND ? "\n  out float4 ocol1 : SV_Target1," : "",
					per_pixel_depth ? "\n  out float depth : SV_Depth," : "");
			}

			// "centroid" attribute is only supported by D3D11
			const char* optCentroid = (ApiType == API_D3D11 ? "centroid" : "");

			out.Write("  in %s float4 colors_0 : COLOR0,\n", optCentroid);
			out.Write("  in %s float4 colors_1 : COLOR1", optCentroid);

			// compute window position if needed because binding semantic WPOS is not widely supported
			if (numTexgen < 7)
			{
				for (unsigned int i = 0; i < numTexgen; ++i)
					out.Write(",\n  in %s float3 uv%d : TEXCOORD%d", optCentroid, i, i);
				out.Write(",\n  in %s float4 clipPos : TEXCOORD%d", optCentroid, numTexgen);
				if (enable_pl)
					out.Write(",\n  in %s float4 Normal : TEXCOORD%d", optCentroid, numTexgen + 1);
				out.Write("        ) {\n");
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (enable_pl)
				{
					for (u32 i = 0; i < 8; ++i)
						out.Write(",\n  in float4 uv%d : TEXCOORD%d", i, i);
				}
				else
				{
					for (u32 i = 0; i < numTexgen; ++i)
						out.Write(",\n  in float%d uv%d : TEXCOORD%d", i < 4 ? 4 : 3, i, i);
				}
				out.Write("        ) {\n");
				out.Write("float4 clipPos = float4(0.0,0.0,0.0,0.0);");
			}
		}
		if (dstAlphaMode == DSTALPHA_NULL)
		{
			out.Write("ocol0 = float4(0.0,0.0,0.0,0.0);\n");
			out.Write("}\n");
			if (text[sizeof(text)-1] != 0x7C)
				PanicAlert("PixelShader generator - buffer too small, canary has been eaten!");

#ifndef ANDROID
			uselocale(old_locale); // restore locale
			freelocale(locale);
#endif
			return;
		}

		out.Write("float4 c0 = " I_COLORS "[1], c1 = " I_COLORS "[2], c2 = " I_COLORS "[3], prev = " I_COLORS "[0];\n"
			"float4 tex_t = float4(0.0,0.0,0.0,0.0), ras_t = float4(0.0,0.0,0.0,0.0), konst_t = float4(0.0,0.0,0.0,0.0);\n"
			"float3 c16 = float3(1.0,256.0,0.0), c24 = float3(1.0,256.0,256.0*256.0);\n"
			"float a_bump=0.0;\n"
			"float3 tevcoord=float3(0.0,0.0,0.0);\n"
			"float2 wrappedcoord=float2(0.0,0.0), t_coord=float2(0.0,0.0),ittmpexp=float2(0.0,0.0);\n"
			"float4 tin_a = float4(0.0,0.0,0.0,0.0), tin_b = float4(0.0,0.0,0.0,0.0), tin_c = float4(0.0,0.0,0.0,0.0), tin_d = float4(0.0,0.0,0.0,0.0);\n\n");

		if (ApiType == API_OPENGL)
		{
			// On Mali, global variables must be initialized as constants.
			// This is why we initialize these variables locally instead.
			out.Write("float4 rawpos = gl_FragCoord;\n");
			out.Write("float4 colors_0 = colors_02;\n");
			out.Write("float4 colors_1 = colors_12;\n");
			// compute window position if needed because binding semantic WPOS is not widely supported
			// Let's set up attributes
			if (numTexgen < 7)
			{
				if (numTexgen)
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("float3 uv%d = uv%d_2;\n", i, i);
					}
				}
				out.Write("float4 clipPos = clipPos_2;\n");
				if (enable_pl)
				{
					out.Write("float4 Normal = Normal_2;\n");
				}
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (enable_pl)
				{
					for (u32 i = 0; i < 8; ++i)
					{
						out.Write("float4 uv%d = uv%d_2;\n", i, i);
					}
				}
				else
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("float%d uv%d = uv%d_2;\n", i < 4 ? 4 : 3, i, i);
					}
				}
			}
		}
	}
	
	if (enable_pl)
	{
		if (Write_Code)
		{
			if (xfregs.numChan.numColorChans > 0)
			{
				if (numTexgen < 7)
				{
					out.Write("float3 _norm0 = normalize(Normal.xyz);\n\n");
					out.Write("float3 pos = float3(clipPos.x,clipPos.y,Normal.w);\n");
				}
				else
				{
					out.Write("float3 _norm0 = normalize(float3(uv4.w,uv5.w,uv6.w));\n\n");
					out.Write("float3 pos = float3(uv0.w,uv1.w,uv7.w);\n");
				}

				out.Write("float4 mat, lacc;\n"
					"float3 ldir, h;\n"
					"float dist, dist2, attn;\n");
			}
		}
		uid_data.components = components;
		GenerateLightingShader<T, Write_Code>(out, uid_data.lighting, components, I_PMATERIALS, I_PLIGHTS, "colors_", "colors_");		
	}
	if (Write_Code)
	{
		if (numTexgen < 7)
			out.Write("clipPos = float4(rawpos.x, rawpos.y, clipPos.z, clipPos.w);\n");
		else
			out.Write("clipPos = float4(rawpos.x, rawpos.y, uv2.w, uv3.w);\n");
	}


	// HACK to handle cases where the tex gen is not enabled
	if (numTexgen == 0)
	{
		if (Write_Code)
			out.Write("float3 uv0 = float3(0.0,0.0,0.0);\n");
	}
	else
	{
		for (unsigned int i = 0; i < numTexgen; ++i)
		{
			// optional perspective divides
			uid_data.texMtxInfo_n_projection |= xfregs.texMtxInfo[i].projection << i;
			if (Write_Code)
			{
				if (xfregs.texMtxInfo[i].projection == XF_TEXPROJ_STQ)
				{
					out.Write("if (uv%d.z != 0.0)", i);
					out.Write("\tuv%d.xy = uv%d.xy / uv%d.z;\n", i, i, i);
				}
				out.Write("uv%d.xy = round(128.0 * uv%d.xy * " I_TEXDIMS"[%d].zw);\n", i, i, i);
			}
		}
	}

	// indirect texture map lookup
	int nIndirectStagesUsed = 0;
	if (bpmem.genMode.numindstages > 0)
	{
		for (unsigned int i = 0; i < numStages; ++i)
		{
			if (bpmem.tevind[i].IsActive() && bpmem.tevind[i].bt < bpmem.genMode.numindstages)
				nIndirectStagesUsed |= 1 << bpmem.tevind[i].bt;
		}
	}

	uid_data.nIndirectStagesUsed = nIndirectStagesUsed;
	for (u32 i = 0; i < bpmem.genMode.numindstages; ++i)
	{
		if (nIndirectStagesUsed & (1 << i))
		{
			unsigned int texcoord = bpmem.tevindref.getTexCoord(i);
			unsigned int texmap = bpmem.tevindref.getTexMap(i);

			uid_data.SetTevindrefValues(i, texcoord, texmap);
			if (Write_Code)
			{
				if (texcoord < numTexgen)
				{
					out.Write("t_coord = BSHR(uv%d.xy, " I_INDTEXSCALE "[%d].xy, " I_INDTEXSCALE "[%d].zw);\n", texcoord, i, i);				
				}
				else
				{
					out.Write("t_coord = float2(0.0,0.0);\n");
				}
				out.Write("float3 indtex%d = ", i);
				SampleTexture<T, Write_Code, ApiType>(out, "(round(t_coord) * (1.0/128.0))", "abg", texmap);
			}
		}
	}

	// Uid fields for BuildSwapModeTable are set in WriteStage
	char swapModeTable[4][5];
	const char* swapColors = "rgba";
	for (int i = 0; i < 4; i++)
	{
		swapModeTable[i][0] = swapColors[bpmem.tevksel[i * 2].swap1];
		swapModeTable[i][1] = swapColors[bpmem.tevksel[i * 2].swap2];
		swapModeTable[i][2] = swapColors[bpmem.tevksel[i * 2 + 1].swap1];
		swapModeTable[i][3] = swapColors[bpmem.tevksel[i * 2 + 1].swap2];
		swapModeTable[i][4] = '\0';
	}

	for (unsigned int i = 0; i < numStages; i++)
		WriteStage<T, Write_Code, ApiType>(out, uid_data, i, swapModeTable); // build the equation for this stage

	if (Write_Code)
	{
		u32 colorCdest = bpmem.combiners[numStages - 1].colorC.dest;
		u32 alphaCdest = bpmem.combiners[numStages - 1].alphaC.dest;
		if (numStages)
		{
			// The results of the last texenv stage are put onto the screen,
			// regardless of the used destination register			
			if (colorCdest != 0)
			{
				out.Write("prev.rgb = %s;\n", tevCOutputTable[colorCdest]);
			}			
			if (alphaCdest != 0)
			{
				out.Write("prev.a = %s;\n", tevAOutputTable[alphaCdest]);
			}
		}
		// emulation of unsigned 8 overflow
		if(TevOverflowState[tevCOutputSourceMap[colorCdest]] || TevOverflowState[tevAOutputSourceMap[alphaCdest]])
			out.Write("prev = CHK_O_U8(prev);\n");
	}

	AlphaTest::TEST_RESULT Pretest = bpmem.alpha_test.TestResult();
	uid_data.Pretest = Pretest;

	// NOTE: Fragment may not be discarded if alpha test always fails and early depth test is enabled 
	// (in this case we need to write a depth value if depth test passes regardless of the alpha testing result)
	if (Pretest != AlphaTest::PASS)
		WriteAlphaTest<T, Write_Code, ApiType>(out, uid_data, dstAlphaMode, per_pixel_depth);


	// D3D9 doesn't support readback of depth in pixel shader, so we always have to calculate it again.
	// This shouldn't be a performance issue as the written depth is usually still from perspective division
	// but this isn't true for z-textures, so there will be depth issues between enabled and disabled z-textures fragments
	if ((ApiType == API_OPENGL || ApiType == API_D3D11) && g_ActiveConfig.bFastDepthCalc)
	{
		if (Write_Code)
		{
			out.Write("float zCoord = rawpos.z;\n");
		}
	}
	else
	{
		// the screen space depth value = far z + (clip z / clip w) * z range
		if (Write_Code)
		{
			out.Write("float zCoord = " I_ZBIAS "[1].x + (clipPos.z / clipPos.w) * " I_ZBIAS "[1].y;\n");
		}
	}

	// depth texture can safely be ignored if the result won't be written to the depth buffer (early_ztest) and isn't used for fog either
	const bool skip_ztexture = !per_pixel_depth && !bpmem.fog.c_proj_fsel.fsel;

	uid_data.ztex_op = bpmem.ztex2.op;
	uid_data.per_pixel_depth = per_pixel_depth;
	uid_data.forced_early_z = forced_early_z;
	uid_data.fast_depth_calc = g_ActiveConfig.bFastDepthCalc;
	uid_data.early_ztest = bpmem.UseEarlyDepthTest();
	uid_data.fog_fsel = bpmem.fog.c_proj_fsel.fsel;

	// Note: z-textures are not written to depth buffer if early depth test is used
	if (per_pixel_depth && bpmem.UseEarlyDepthTest() && Write_Code)
		out.Write("depth = zCoord;\n");

	// Note: depth texture output is only written to depth buffer if late depth test is used
	// theoretical final depth value is used for fog calculation, though, so we have to emulate ztextures anyway
	if (bpmem.ztex2.op != ZTEXTURE_DISABLE && !skip_ztexture)
	{
		// use the texture input of the last texture stage (tex_t), hopefully this has been read and is in correct format...
		if (Write_Code)
		{
			out.Write("zCoord = dot(" I_ZBIAS"[0].xyzw, tex_t.xyzw) + " I_ZBIAS "[1].w %s;\n",
				(bpmem.ztex2.op == ZTEXTURE_ADD) ? "+ zCoord" : "");
			// U24 overflow emulation disabled because problems caused by float rounding
			//out.Write("zCoord = CHK_O_U24(zCoord);\n");		
		}

	}

	if (per_pixel_depth && bpmem.UseLateDepthTest() && Write_Code)
		out.Write("depth = zCoord;\n");

	if (dstAlphaMode == DSTALPHA_ALPHA_PASS)
	{
		if (Write_Code)
			out.Write("ocol0 = float4(prev.rgb," I_ALPHA ".a) * (1.0/255.0);\n");
	}
	else
	{
		WriteFog<T, Write_Code>(out, uid_data);
		if (Write_Code)
			out.Write("ocol0 = prev * (1.0/255.0);\n");
	}

	// Use dual-source color blending to perform dst alpha in a single pass
	if (dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND)
	{
		if (Write_Code)
		{
			if (ApiType & API_D3D9)
			{
				// alpha component must be 0 or the shader will not compile (Direct3D 9Ex restriction)
				// Colors will be blended against the color from ocol1 in D3D 9...
				out.Write("ocol1 = float4(prev.a, prev.a, prev.a, 0.0) * (1.0/255.0);\n");
			}
			else
			{
				// Colors will be blended against the alpha from ocol1...
				out.Write("ocol1 = prev * (1.0/255.0);\n");
			}
			// ...and the alpha from ocol0 will be written to the framebuffer.
			out.Write("ocol0.a = " I_ALPHA ".a*(1.0/255.0);\n");
		}
	}

	if (Write_Code)
	{
		out.Write("}\n");
		if (text[sizeof(text)-1] != 0x7C)
			PanicAlert("PixelShader generator - buffer too small, canary has been eaten!");

#ifndef ANDROID
		uselocale(old_locale); // restore locale
		freelocale(locale);
#endif
	}
}

template<class T, bool Write_Code, API_TYPE ApiType>
static inline void WriteStage(T& out, pixel_shader_uid_data& uid_data, int n, const char swapModeTable[4][5])
{
	int texcoord = bpmem.tevorders[n / 2].getTexCoord(n & 1);
	bool bHasTexCoord = (u32)texcoord < bpmem.genMode.numtexgens;
	bool bHasIndStage = bpmem.tevind[n].IsActive() && bpmem.tevind[n].bt < bpmem.genMode.numindstages;
	// HACK to handle cases where the tex gen is not enabled
	if (!bHasTexCoord)
		texcoord = 0;
	if (Write_Code)
	{
		out.Write("// TEV stage %d\n", n);
	}


	uid_data.stagehash[n].hasindstage = bHasIndStage;
	uid_data.stagehash[n].tevorders_texcoord = texcoord;
	if (bHasIndStage)
	{
		uid_data.stagehash[n].tevind = bpmem.tevind[n].hex & 0x7FFFFF;
		if (Write_Code)
		{
			out.Write("// indirect op\n");
			// perform the indirect op on the incoming regular coordinates using indtex%d as the offset coords
			if (bpmem.tevind[n].bs != ITBA_OFF)
			{
				// lest explain a little what is done here
				// a_bump is taked from the upper bits that are not taked for indirect texturing
				// if all the bits are used for indirect texturing then only the upper 5 bits are used for a_bump
				// so to do this bit masking is used for example 
				// a_bump = x & 0xE0; for 3 bits
				// a_bump = x & 0xF0; for 4 bits
				// a_bump = x & 0xF8; for 5 bits
				// there is no support for bitmasking in older hardware
				// and in newer hardware is slower than using pure float operations
				// so we have to emulate it
				// the exact formula for float masking emulation of the upper bits of a number is: 
				// having x as the number to mask stored in a float
				// nb as the number of bits to mask
				// and n the (wordlen - nb) in this case (8 - nb)
				// then result = floor(x * (255.0f/(2^n))) * ((2^n) / 255.0f)
				// so for nb = 3 bit this will be n = 5  result = floor(x * (255.0/32.0)) * (32.0/255.0f);
				// to optimize a litle al the coeficient are precalculated to avoid slowing thigs more than needed

				static const char *tevIndAlphaSel[] = { "", "x", "y", "z" };
				static const char *tevIndAlphaScale[] = { "(1.0/8.0)", "(1.0/32.0)", "(1.0/16.0)", "(1.0/8.0)" };
				static const char *tevIndAlphaNormFactor[] =
				{
					"8.0",	// 5 bits
					"32.0",	// 3 bits
					"16.0",	// 4 bits
					"8.0"	// 5 bits
				};

				out.Write("a_bump = floor(indtex%d.%s * %s) * %s;\n",
					bpmem.tevind[n].bt,
					tevIndAlphaSel[bpmem.tevind[n].bs],
					tevIndAlphaScale[bpmem.tevind[n].fmt],
					tevIndAlphaNormFactor[bpmem.tevind[n].fmt]);
			}

			static const char *tevIndFmtScale[] =
			{
				"(1.0/256.0)",	// 8 bits (& 0xFF)
				"(1.0/32.0)",	// 5 bits (& 0x1F)
				"(1.0/16.0)",	// 4 bits (& 0x0F)
				"(1.0/8.0)"		// 3 bits (& 0x07)
			};

			static const char *tevIndFmtNormFactor[] =
			{
				"256.0",// 8 bits
				"32.0",	// 5 bits
				"16.0",	// 4 bits
				"8.0"	// 3 bits
			};

			// format
			// to mask the lower bits the formula is:
			// having x as number to mask stored in a float
			// nb as the number of bits to mask			
			// then result = frac(x * 255.0 / (2^nb) * (2^nb)
			// for 3 bits result = frac(x * 255.0 / 8.0) * 8.0
			if (bpmem.tevind[n].fmt > 0)
			{
				out.Write("float3 indtevcrd%d = round(frac(indtex%d * %s) * %s);\n",
					n,
					bpmem.tevind[n].bt,
					tevIndFmtScale[bpmem.tevind[n].fmt],
					tevIndFmtNormFactor[bpmem.tevind[n].fmt]);
			}
			else
			{
				out.Write("float3 indtevcrd%d = indtex%d;\n",
					n,
					bpmem.tevind[n].bt);
			}
			

			static const char *tevIndBiasField[] = { "", "x", "y", "xy", "z", "xz", "yz", "xyz" }; // indexed by bias
			static const char *tevIndBiasAdd[] = { "-128.0", "1.0", "1.0", "1.0" }; // indexed by fmt
			// bias
			if (bpmem.tevind[n].bias == ITB_S || bpmem.tevind[n].bias == ITB_T || bpmem.tevind[n].bias == ITB_U)
				out.Write("indtevcrd%d.%s += %s;\n", n, tevIndBiasField[bpmem.tevind[n].bias], tevIndBiasAdd[bpmem.tevind[n].fmt]);
			else if (bpmem.tevind[n].bias == ITB_ST || bpmem.tevind[n].bias == ITB_SU || bpmem.tevind[n].bias == ITB_TU)
				out.Write("indtevcrd%d.%s += float2(%s, %s);\n", n, tevIndBiasField[bpmem.tevind[n].bias], tevIndBiasAdd[bpmem.tevind[n].fmt], tevIndBiasAdd[bpmem.tevind[n].fmt]);
			else if (bpmem.tevind[n].bias == ITB_STU)
				out.Write("indtevcrd%d.%s += float3(%s, %s, %s);\n", n, tevIndBiasField[bpmem.tevind[n].bias], tevIndBiasAdd[bpmem.tevind[n].fmt], tevIndBiasAdd[bpmem.tevind[n].fmt], tevIndBiasAdd[bpmem.tevind[n].fmt]);

			// multiply by offset matrix and scale
			if (bpmem.tevind[n].mid != 0)
			{
				if (bpmem.tevind[n].mid <= 3)
				{
					int mtxidx = 2 * (bpmem.tevind[n].mid - 1);
					out.Write("float2 indtevtrans%d = float2(dot(" I_INDTEXMTX "[%d].xyz, indtevcrd%d), dot(" I_INDTEXMTX "[%d].xyz, indtevcrd%d));\n",
						n, mtxidx, n, mtxidx + 1, n);
					out.Write("indtevtrans%d = BSHR(indtevtrans%d, float2(7.0,7.0), float2(1.0,1.0)/8.0);\n", n, n);
					out.Write("indtevtrans%d = BSH(indtevtrans%d, " I_INDTEXMTX "[%d].ww);\n", n, n, mtxidx);
				}
				else if (bpmem.tevind[n].mid <= 7 && bHasTexCoord)
				{ // s matrix
					_assert_(bpmem.tevind[n].mid >= 5);
					int mtxidx = 2 * (bpmem.tevind[n].mid - 5);
					out.Write("float2 indtevtrans%d = uv%d.xy * indtevcrd%d.xx;\n", n, texcoord, n);
					out.Write("indtevtrans%d = BSHR(indtevtrans%d, float2(255.0,255.0), float2(1.0,1.0)/256.0);\n", n, n);
					out.Write("indtevtrans%d = BSH(indtevtrans%d, " I_INDTEXMTX "[%d].ww);\n", n, n, mtxidx);
				}
				else if (bpmem.tevind[n].mid <= 11 && bHasTexCoord)
				{ // t matrix
					_assert_(bpmem.tevind[n].mid >= 9);
					int mtxidx = 2 * (bpmem.tevind[n].mid - 9);
					out.Write("float2 indtevtrans%d = uv%d.xy * indtevcrd%d.yy;\n", n, texcoord, n);
					out.Write("indtevtrans%d = BSHR(indtevtrans%d, float2(255.0,255.0), float2(1.0,1.0)/256.0);\n", n, n);
					out.Write("indtevtrans%d = BSH(indtevtrans%d, " I_INDTEXMTX "[%d].ww);\n", n, n, mtxidx);
				}
				else
				{
					out.Write("float2 indtevtrans%d = float2(0.0,0.0);\n", n);
				}
			}
			else
			{
				out.Write("float2 indtevtrans%d = float2(0.0,0.0);\n", n);
			}
			// ---------
			// Wrapping
			// ---------
			static const char *tevIndWrapStart[] = { "0.0", "(256.0*128.0)", "(128.0*128.0)", "(64.0*128.0)", "(32.0*128.0)", "(16.0*128.0)", "1.0" };
			// wrap S
			if (bpmem.tevind[n].sw == ITW_OFF)
				out.Write("wrappedcoord.x = uv%d.x;\n", texcoord);
			else if (bpmem.tevind[n].sw == ITW_0)
				out.Write("wrappedcoord.x = 0.0;\n");
			else
				out.Write("wrappedcoord.x = fastmod(uv%d.x, %s);\n", texcoord, tevIndWrapStart[bpmem.tevind[n].sw]);

			// wrap T
			if (bpmem.tevind[n].tw == ITW_OFF)
				out.Write("wrappedcoord.y = uv%d.y;\n", texcoord);
			else if (bpmem.tevind[n].tw == ITW_0)
				out.Write("wrappedcoord.y = 0.0;\n");
			else
				out.Write("wrappedcoord.y = fastmod(uv%d.y, %s);\n", texcoord, tevIndWrapStart[bpmem.tevind[n].tw]);

			if (bpmem.tevind[n].fb_addprev) // add previous tevcoord
				out.Write("tevcoord.xy += wrappedcoord + indtevtrans%d;\n", n);
			else
				out.Write("tevcoord.xy = wrappedcoord + indtevtrans%d;\n", n);
		}
	}

	TevStageCombiner::ColorCombiner &cc = bpmem.combiners[n].colorC;
	TevStageCombiner::AlphaCombiner &ac = bpmem.combiners[n].alphaC;

	uid_data.stagehash[n].cc = cc.hex & 0xFFFFFF;
	uid_data.stagehash[n].ac = ac.hex & 0xFFFFF0; // Storing rswap and tswap later

	if (cc.UsedAsInput(TEVCOLORARG_RASA) || cc.UsedAsInput(TEVCOLORARG_RASC) || ac.UsedAsInput(TEVALPHAARG_RASA))
	{
		const int i = bpmem.combiners[n].alphaC.rswap;
		uid_data.stagehash[n].ac |= bpmem.combiners[n].alphaC.rswap;
		uid_data.stagehash[n].tevksel_swap1a = bpmem.tevksel[i * 2].swap1;
		uid_data.stagehash[n].tevksel_swap2a = bpmem.tevksel[i * 2].swap2;
		uid_data.stagehash[n].tevksel_swap1b = bpmem.tevksel[i * 2 + 1].swap1;
		uid_data.stagehash[n].tevksel_swap2b = bpmem.tevksel[i * 2 + 1].swap2;
		uid_data.stagehash[n].tevorders_colorchan = bpmem.tevorders[n / 2].getColorChan(n & 1);

		const char *rasswap = swapModeTable[bpmem.combiners[n].alphaC.rswap];
		if (Write_Code)
		{
			int rasindex = bpmem.tevorders[n / 2].getColorChan(n & 1);
			if (rasindex == 0 && !tevRascolor0_Expanded)
			{
				out.Write("colors_0 = round(colors_0 * 255.0);\n");
				tevRascolor0_Expanded = true;
			}
			if (rasindex == 1 && !tevRascolor1_Expanded)
			{
				out.Write("colors_1 = round(colors_1 * 255.0);\n");
				tevRascolor1_Expanded = true;
			}
			out.Write("ras_t = %s.%s;\n", tevRasTable[rasindex], rasswap);
			TevOverflowState[tevSources::RASC] = rasindex < 2;
			TevOverflowState[tevSources::RASA] = rasindex < 2;
		}
	}

	uid_data.stagehash[n].tevorders_enable = bpmem.tevorders[n / 2].getEnable(n & 1);
	if (bpmem.tevorders[n / 2].getEnable(n & 1))
	{
		if (Write_Code)
		{
			if (!bHasIndStage)
			{
				// calc tevcord
				if (bHasTexCoord)
					out.Write("tevcoord.xy = uv%d.xy;\n", texcoord);
				else
					out.Write("tevcoord.xy = float2(0.0,0.0);\n");
			}
		}
		const int i = bpmem.combiners[n].alphaC.tswap;
		uid_data.stagehash[n].ac |= bpmem.combiners[n].alphaC.tswap << 2;
		uid_data.stagehash[n].tevksel_swap1c = bpmem.tevksel[i * 2].swap1;
		uid_data.stagehash[n].tevksel_swap2c = bpmem.tevksel[i * 2].swap2;
		uid_data.stagehash[n].tevksel_swap1d = bpmem.tevksel[i * 2 + 1].swap1;
		uid_data.stagehash[n].tevksel_swap2d = bpmem.tevksel[i * 2 + 1].swap2;

		uid_data.stagehash[n].tevorders_texmap = bpmem.tevorders[n / 2].getTexMap(n & 1);

		const char *texswap = swapModeTable[bpmem.combiners[n].alphaC.tswap];
		int texmap = bpmem.tevorders[n / 2].getTexMap(n & 1);
		uid_data.SetTevindrefTexmap(i, texmap);
		if (Write_Code)
			out.Write("tex_t = ");

		SampleTexture<T, Write_Code, ApiType>(out, "(round(tevcoord) * (1.0/128.0))", texswap, texmap);
	}
	else if (Write_Code)
	{
		out.Write("tex_t = float4(255.0,255.0,255.0,255.0);\n");
	}


	if (cc.UsedAsInput(TEVCOLORARG_KONST) || ac.UsedAsInput(TEVALPHAARG_KONST))
	{
		int kc = bpmem.tevksel[n / 2].getKC(n & 1);
		int ka = bpmem.tevksel[n / 2].getKA(n & 1);
		uid_data.stagehash[n].tevksel_kc = kc;
		uid_data.stagehash[n].tevksel_ka = ka;
		if (Write_Code)
		{
			out.Write("konst_t = float4(%s,%s);\n", tevKSelTableC[kc], tevKSelTableA[ka]);
			TevOverflowState[tevSources::KONST] = kc > 7 || ka > 7;
		}
	}
	if (Write_Code)
	{
		out.Write("tin_a = %s(float4(%s,%s));\n", TevOverflowState[cc.a] || TevOverflowState[AInputSourceMap[ac.a]] ? "CHK_O_U8" : "", tevCInputTable[cc.a], tevAInputTable[ac.a]);
		out.Write("tin_b = %s(float4(%s,%s));\n", TevOverflowState[cc.b] || TevOverflowState[AInputSourceMap[ac.b]] ? "CHK_O_U8" : "", tevCInputTable[cc.b], tevAInputTable[ac.b]);
		out.Write("tin_c = %s(float4(%s,%s));\n", TevOverflowState[cc.c] || TevOverflowState[AInputSourceMap[ac.c]] ? "CHK_O_U8" : "", tevCInputTable[cc.c], tevAInputTable[ac.c]);
		
		bool normalize_c_rgb = cc.c != TEVCOLORARG_ZERO &&  cc.bias != TevBias_COMPARE;
		bool normalize_c_a = ac.c != TEVALPHAARG_ZERO &&  ac.bias != TevBias_COMPARE;
		if (normalize_c_rgb && normalize_c_a)
		{
			out.Write("tin_c = tin_c+trunc(tin_c*(1.0/128.0));\n");
		}
		else if (normalize_c_rgb)
		{
			out.Write("tin_c.rgb = tin_c.rgb+trunc(tin_c.rgb*(1.0/128.0));\n");
		}
		else if (normalize_c_a)
		{
			out.Write("tin_c.a = tin_c.a+trunc(tin_c.a*(1.0/128.0));\n");
		}
		
		if (!(cc.d == TEVCOLORARG_ZERO && cc.op == TEVOP_ADD && !(cc.shift & 3)) || !(ac.d == TEVALPHAARG_ZERO && ac.op == TEVOP_ADD && !(ac.shift & 3)))
		{
			out.Write("tin_d = float4(%s,%s);\n", tevCInputTable[cc.d], tevAInputTable[ac.d]);
		}
		TevOverflowState[tevCOutputSourceMap[cc.dest]] = !cc.clamp;
		TevOverflowState[tevAOutputSourceMap[ac.dest]] = !ac.clamp;

		out.Write("// color combine\n");
		out.Write("%s = round(clamp(", tevCOutputTable[cc.dest]);		
		// combine the color channel
		if (cc.bias != TevBias_COMPARE) // if not compare
		{
			//normal color combiner goes here
			WriteTevRegular<TEVCOLORARG_ZERO, TEVCOLORARG_ONE>(out, ".rgb", cc.bias, cc.op, cc.clamp, cc.shift, cc.a, cc.b, cc.c, cc.d);
		}
		else
		{
			//compare color combiner goes here
			int cmp = (cc.shift << 1) | cc.op; // comparemode stored here
			WriteTevCompare(out, 1, cmp);
		}
		if (cc.clamp)
		{
			out.Write(",0.0,255.0));\n");
		}
		else
		{
			out.Write(",-1024.0,1023.0));\n");
		}

		out.Write("// alpha combine\n");
		out.Write("%s = round(clamp(", tevAOutputTable[ac.dest]);		
		if (ac.bias != TevBias_COMPARE) // if not compare
		{
			// 8 is used because alpha stage don't have ONE input so a number outside range is used
			WriteTevRegular<TEVALPHAARG_ZERO, 8>(out, ".a", ac.bias, ac.op, ac.clamp, ac.shift, ac.a, ac.b, ac.c, ac.d);
		}
		else
		{
			//compare alpha combiner goes here			
			int cmp = (ac.shift << 1) | ac.op; // comparemode stored here
			WriteTevCompare(out, 0, cmp);
		}
		if (ac.clamp)
		{
			out.Write(",0.0,255.0));\n\n");
		}
		else
		{
			out.Write(",-1024.0,1023.0));\n\n");
		}
		out.Write("// TEV done\n");
	}
}

template<int TEVARG_ZERO, int TEVARG_ONE, class T>
static inline void WriteTevRegular(T& out, const char* components, int bias, int op, int clamp, int shift, int a, int b, int c, int d)
{
	const char *tevScaleTableLeft[] =
	{
		"",       // SCALE_1
		" * 2.0",  // SCALE_2
		" * 4.0",  // SCALE_4
		"",       // DIVIDE_2
	};

	const char *tevScaleTableRight[] =
	{
		"",       // SCALE_1
		"",       // SCALE_2
		"",       // SCALE_4
		" * 0.5",  // DIVIDE_2
	};

	const char *tevLerpBias[] = // indexed by 2*op+(shift==3)
	{
		"",
		" + 128.0",
		"",
		" + 127.0",
	};

	const char *tevBiasTable[] =
	{
		"",        // ZERO,
		" + 128.0",  // ADDHALF,
		" - 128.0",  // SUBHALF,
		"",
	};

	const char *tevOpTable[] = {
		"+",      // TEVOP_ADD = 0,
		"-",      // TEVOP_SUB = 1,
	};

	// Regular TEV stage: (d + bias + lerp(a,b,c)) * scale
	// The GC/Wii GPU uses a very sophisticated algorithm for scale-lerping:
	// - c is scaled from 0..255 to 0..256, which allows dividing the result by 256 instead of 255
	// - if scale is bigger than one, it is moved inside the lerp calculation for increased accuracy
	// - a rounding bias is added before dividing by 256
	bool d_component_used = !(d == TEVARG_ZERO && op == TEVOP_ADD && !(shift & 3));
	bool right_side_zero = (a == b && a == c &&  a == TEVARG_ZERO);
	out.Write("%s", shift == 3 ? "trunc(" : "");
	if (d_component_used)
	{
		out.Write("(((tin_d%s%s)%s)", components, tevBiasTable[bias], tevScaleTableLeft[shift]);
		if (!right_side_zero)
		{
			out.Write(" %s ", tevOpTable[op]);
		}
	}

	if (right_side_zero && !d_component_used)
	{
		out.Write("float4(0.0,0.0,0.0,0.0)%s", components);
	}
	else if (right_side_zero)
	{
		// no need to write here, we use only d component
	}
	else if (a == b || c == TEVARG_ZERO)
	{
		out.Write("(tin_a%s%s)", components, tevScaleTableLeft[shift]);
	}
	else if (c == TEVARG_ONE)
	{
		out.Write("(tin_b%s%s)", components, tevScaleTableLeft[shift]);
	}
	else if (a == TEVARG_ZERO)
	{
		out.Write("trunc((((tin_b%s*tin_c%s)%s)%s)*(1.0/256.0))",
			components, components,
			tevScaleTableLeft[shift], tevLerpBias[2 * op + (shift != 3)]);
	}
	else if (b == TEVARG_ZERO)
	{
		out.Write("trunc((((tin_a%s*(256.0 - tin_c%s))%s)%s)*(1.0/256.0))",
			components, components,
			tevScaleTableLeft[shift], tevLerpBias[2 * op + (shift != 3)]);
	}
	else
	{
		out.Write("trunc((((tin_a%s*256.0 + (tin_b%s-tin_a%s)*tin_c%s)%s)%s)*(1.0/256.0))",
			components, components, components, components,
			tevScaleTableLeft[shift], tevLerpBias[2 * op + (shift != 3)]);
	}
	out.Write("%s%s", d_component_used ? ")" : "", tevScaleTableRight[shift]);
	out.Write("%s", shift == 3 ? ")" : "");
}

template<class T>
static inline void WriteTevCompare(T& out, int components, int cmp)
{
	static const char *TEVCMPComponents[] =
	{
		".a",
		".rgb",
		""
	};

	static const char *TEVCMPZero[] =
	{
		"0.0",
		"float3(0.0,0.0,0.0)",
		"float4(0.0,0.0,0.0,0.0)"
	};

	static const char *TEVCMPOPTable[] =
	{
		"((tin_a.r >= (tin_b.r + 0.5)) ? tin_c%s : %s)", // TEVCMP_R8_GT
		"((abs(tin_a.r - tin_b.r) < 0.5) ? tin_c%s : %s)", // TEVCMP_R8_EQ
		"((dot(tin_a.rgb, c16) >=  (dot(tin_b.rgb, c16) + 0.5)) ? tin_c%s : %s)", // TEVCMP_GR16_GT
		"((abs(dot(tin_a.rgb, c16) - dot(tin_b.rgb, c16)) < 0.5) ? tin_c%s : %s)", // TEVCMP_GR16_EQ
		"((dot(tin_a.rgb, c24) >=  (dot(tin_b.rgb, c24) + 0.5)) ? tin_c%s : %s)", // TEVCMP_BGR24_GT
		"((abs(dot(tin_a.rgb, c24) - dot(tin_b.rgb, c24)) < 0.5) ? tin_c%s : %s)", // TEVCMP_BGR24_EQ
		
		"(max(sign(tin_a.rgb - tin_b.rgb - 0.5), float3(0.0,0.0,0.0)) * tin_c.rgb)", // TEVCMP_RGB8_GT
		"((float3(1.0,1.0,1.0) - max(sign(abs(tin_a.rgb - tin_b.rgb) - 0.5),float3(0.0,0.0,0.0))) * tin_c.rgb)", // TEVCMP_RGB8_EQ
		
		"((tin_a.a >= (tin_b.a + 0.5)) ? tin_c.a : 0.0)", // TEVCMP_A8_GT
		"((abs(tin_a.a - tin_b.a) < 0.5) ? tin_c.a : 0.0)", // TEVCMP_A8_EQ
		
		"(max(sign(tin_a - tin_b - 0.5), float4(0.0,0.0,0.0,0.0)) * tin_c)", // TEVCMP_RGBA8_GT
		"((float4(1.0,1.0,1.0,1.0) - max(sign(abs(tin_a - tin_b) - 0.5),float3(0.0,0.0,0.0,0.0))) * tin_c)" // TEVCMP_RGBA8_EQ
	};
	out.Write("tin_d%s+", TEVCMPComponents[components]);
	if (cmp < 6)
	{
		out.Write(TEVCMPOPTable[cmp], TEVCMPComponents[components], TEVCMPZero[components]);
	}
	else
	{
		cmp += components == 0 ? 2 : (components == 3 ? 4 : 0);
		out.Write(TEVCMPOPTable[cmp]);
	}
}

template<class T, bool Write_Code, API_TYPE ApiType>
void SampleTexture(T& out, const char *texcoords, const char *texswap, int texmap)
{
	if (Write_Code)
	{
		if (ApiType == API_D3D11)
		{
			char * texfmtswp = NULL;
			switch (TextureCache::stagemap[texmap]->pcformat)
			{
			case PC_TEX_FMT_I4_AS_I8:
			case PC_TEX_FMT_I8:
				texfmtswp = "rrrr";
				break;
			case PC_TEX_FMT_IA4_AS_IA8:
			case PC_TEX_FMT_IA8:
				texfmtswp = "rrrg";
				break;
				break;
			default:
				texfmtswp = "rgba";
				break;
			}
			out.Write("round((Tex%d.Sample(samp%d,%s.xy * " I_TEXDIMS"[%d].xy).%s).%s * 255.0);\n", texmap, texmap, texcoords, texmap, texfmtswp, texswap);
		}
		else
			out.Write("round(%s(samp%d,%s.xy * " I_TEXDIMS"[%d].xy).%s * 255.0);\n", ApiType == API_OPENGL ? "texture" : "tex2D", texmap, texcoords, texmap, texswap);
	}
}

static const char *tevAlphaFuncsTable[] =
{
	"(false)",					// NEVER
	"(prev.a <  %s)",			// LESS
	"(prev.a == %s)",			// EQUAL
	"(prev.a <= %s)",			// LEQUAL
	"(prev.a >  %s)",			// GREATER
	"(prev.a != %s)",			// NEQUAL
	"(prev.a >= %s)",			// GEQUAL
	"(true)"					// ALWAYS
};

static const char *tevAlphaFunclogicTable[] =
{
	" && ", // and
	" || ", // or
	" != ", // xor
	" == "  // xnor
};

template<class T, bool Write_Code, API_TYPE ApiType>
static inline void WriteAlphaTest(T& out, pixel_shader_uid_data& uid_data, DSTALPHA_MODE dstAlphaMode, bool per_pixel_depth)
{
	static const char *alphaRef[2] =
	{
		I_ALPHA".r",
		I_ALPHA".g"
	};

	uid_data.alpha_test_comp0 = bpmem.alpha_test.comp0;
	uid_data.alpha_test_comp1 = bpmem.alpha_test.comp1;
	uid_data.alpha_test_logic = bpmem.alpha_test.logic;
	uid_data.alpha_test_use_zcomploc_hack = bpmem.UseEarlyDepthTest() && bpmem.zmode.updateenable && !g_ActiveConfig.backend_info.bSupportsEarlyZ;
	if (Write_Code)
	{
		// using discard then return works the same in cg and dx9 but not in dx11
		out.Write("if(!( ");

		// Lookup the first component from the alpha function table
		int compindex = bpmem.alpha_test.comp0;
		out.Write(tevAlphaFuncsTable[compindex], alphaRef[0]);

		out.Write("%s", tevAlphaFunclogicTable[bpmem.alpha_test.logic]);//lookup the logic op

		// Lookup the second component from the alpha function table
		compindex = bpmem.alpha_test.comp1;
		out.Write(tevAlphaFuncsTable[compindex], alphaRef[1]);
		out.Write(")) {\n");

		out.Write("ocol0 = float4(0.0,0.0,0.0,0.0);\n");
		if (dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND)
			out.Write("ocol1 = float4(0.0,0.0,0.0,0.0);\n");
		if (per_pixel_depth)
			out.Write("depth = 1.f;\n");

		// HAXX: zcomploc (aka early_ztest) is a way to control whether depth test is done before
		// or after texturing and alpha test. PC graphics APIs have no way to support this
		// feature properly as of 2012: Depth buffer and depth test are not
		// programmable and the depth test is always done after texturing.
		// Most importantly, they do not allow writing to the z-buffer without
		// writing a color value (unless color writing is disabled altogether).
		// We implement "depth test before texturing" by disabling alpha test when early-z is in use.
		// It seems to be less buggy than not to update the depth buffer if alpha test fails,
		// but both ways wouldn't be accurate.

		// OpenGL 4.2 has a flag which allows the driver to still update the depth buffer 
		// if alpha test fails. The driver doesn't have to, but I assume they all do because
		// it's the much faster code path for the GPU.		
		if (!uid_data.alpha_test_use_zcomploc_hack)
		{
			out.Write("discard;\n");
			if (ApiType != API_D3D11)
				out.Write("return;\n");
		}
		out.Write("}\n");
	}
}

static const char *tevFogFuncsTable[] =
{
	"",																// No Fog
	"",																// ?
	"",																// Linear
	"",																// ?
	"fog = 1.0 - exp2(-8.0 * fog);\n",						// exp
	"fog = 1.0 - exp2(-8.0 * fog * fog);\n",				// exp2
	"fog = exp2(-8.0 * (1.0 - fog));\n",					// backward exp
	"fog = 1.0 - fog;\n   fog = exp2(-8.0 * fog * fog);\n"	// backward exp2
};

template<class T, bool Write_Code>
static inline void WriteFog(T& out, pixel_shader_uid_data& uid_data)
{
	uid_data.fog_fsel = bpmem.fog.c_proj_fsel.fsel;
	if (bpmem.fog.c_proj_fsel.fsel == 0)
		return; // no Fog

	uid_data.fog_proj = bpmem.fog.c_proj_fsel.proj;
	uid_data.fog_RangeBaseEnabled = bpmem.fogRange.Base.Enabled;

	if (Write_Code)
	{
		if (bpmem.fog.c_proj_fsel.proj == 0)
		{
			// perspective
			// ze = A/(B - (Zs >> B_SHF)
			out.Write("float ze = " I_FOG "[1].x / (" I_FOG "[1].y - (zCoord * " I_FOG "[1].w));\n");
		}
		else
		{
			// orthographic
			// ze = a*Zs	(here, no B_SHF)
			out.Write("float ze = " I_FOG"[1].x * zCoord;\n");
		}

		// x_adjust = sqrt((x-center)^2 + k^2)/k
		// ze *= x_adjust
		// this is completely theoretical as the real hardware seems to use a table intead of calculating the values.

		if (bpmem.fogRange.Base.Enabled)
		{
			out.Write("float x_adjust = (2.0 * (clipPos.x / " I_FOG "[2].y)) - 1.0 - " I_FOG "[2].x;\n");
			out.Write("x_adjust = sqrt(x_adjust * x_adjust + " I_FOG "[2].z * " I_FOG "[2].z) / " I_FOG "[2].z;\n");
			out.Write("ze *= x_adjust;\n");
		}

		out.Write("float fog = clamp(ze - " I_FOG "[1].z, 0.0, 1.0);\n");

		if (bpmem.fog.c_proj_fsel.fsel > 3)
		{
			out.Write("%s", tevFogFuncsTable[bpmem.fog.c_proj_fsel.fsel]);
		}
		else
		{
			if (bpmem.fog.c_proj_fsel.fsel != 2 && out.GetBuffer() != NULL)
				WARN_LOG(VIDEO, "Unknown Fog Type! %08x", bpmem.fog.c_proj_fsel.fsel);
		}

		out.Write("prev.rgb = round(lerp(prev.rgb, " I_FOG "[0].rgb, fog));\n");
	}

}

void GetPixelShaderUidD3D9(PixelShaderUid& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<PixelShaderUid, false, API_D3D9>(object, dstAlphaMode, components);
}

void GeneratePixelShaderCodeD3D9(ShaderCode& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderCode, true, API_D3D9>(object, dstAlphaMode, components);
}

void GeneratePixelShaderCodeD3D9SM2(ShaderCode& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderCode, true, API_D3D9_SM20>(object, dstAlphaMode, components);
}

void GetPixelShaderUidD3D11(PixelShaderUid& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<PixelShaderUid, false, API_D3D11>(object, dstAlphaMode, components);
}

void GeneratePixelShaderCodeD3D11(ShaderCode& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderCode, true, API_D3D11>(object, dstAlphaMode, components);
}

void GetPixelShaderUidGL(PixelShaderUid& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<PixelShaderUid, false, API_OPENGL>(object, dstAlphaMode, components);
}

void GeneratePixelShaderCodeGL(ShaderCode& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderCode, true, API_OPENGL>(object, dstAlphaMode, components);
}

