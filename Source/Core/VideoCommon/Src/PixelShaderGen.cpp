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
#include "PixelShaderGen.h"
#include "XFMemory.h"  // for texture projection mode
#include "BPMemory.h"
#include "VideoConfig.h"
#include "NativeVertexFormat.h"


//   old tev->pixelshader notes
//
//   color for this stage (alpha, color) is given by bpmem.tevorders[0].colorchan0
//   konstant for this stage (alpha, color) is given by bpmem.tevksel
//   inputs are given by bpmem.combiners[0].colorC.a/b/c/d     << could be current channel color
//   according to GXTevColorArg table above
//   output is given by .outreg
//   tevtemp is set according to swapmodetables and

#define C_31_BY_255 "(31.0/255.0)"
#define C_63_BY_255 "(63.0/255.0)"
#define C_95_BY_255 "(95.0/255.0)"
#define C_127_BY_255 "(127.0/255.0)"
#define C_159_BY_255 "(159.0/255.0)"
#define C_191_BY_255 "(191.0/255.0)"
#define C_223_BY_255 "(223.0/255.0)"
#define C_1023_BY_255 "(1023.0/255.0)"
#define C_1024_BY_255 "(1024.0/255.0)"


static const char *tevKSelTableC[] = // KCSEL
{
	"1.0,1.0,1.0",       // 1   = 0x00
	C_223_BY_255 "," C_223_BY_255 "," C_223_BY_255, // 7_8 = 0x01
	C_191_BY_255 "," C_191_BY_255 "," C_191_BY_255,    // 3_4 = 0x02
	C_159_BY_255 "," C_159_BY_255 "," C_159_BY_255, // 5_8 = 0x03
	C_127_BY_255 "," C_127_BY_255 "," C_127_BY_255,       // 1_2 = 0x04
	C_95_BY_255 "," C_95_BY_255 "," C_95_BY_255, // 3_8 = 0x05
	C_63_BY_255 "," C_63_BY_255 "," C_63_BY_255,    // 1_4 = 0x06
	C_31_BY_255 "," C_31_BY_255 "," C_31_BY_255, // 1_8 = 0x07
	"ERROR1", // 0x08
	"ERROR2", // 0x09
	"ERROR3", // 0x0a
	"ERROR4", // 0x0b
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
	"1.0",  // 1   = 0x00
	C_223_BY_255,// 7_8 = 0x01
	C_191_BY_255, // 3_4 = 0x02
	C_159_BY_255,// 5_8 = 0x03
	C_127_BY_255,  // 1_2 = 0x04
	C_95_BY_255,// 3_8 = 0x05
	C_63_BY_255, // 1_4 = 0x06
	C_31_BY_255,// 1_8 = 0x07
	"ERROR5", // 0x08
	"ERROR6", // 0x09
	"ERROR7", // 0x0a
	"ERROR8", // 0x0b
	"ERROR9", // 0x0c
	"ERROR10", // 0x0d
	"ERROR11", // 0x0e
	"ERROR12", // 0x0f
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

static const char *tevScaleTable[] = // CS
{
	"1.0",  // SCALE_1
	"2.0",  // SCALE_2
	"4.0",  // SCALE_4
	"0.5",  // DIVIDE_2
};

static const char *tevBiasTable[] = // TB
{
	"",       // ZERO,
	"+" C_127_BY_255,  // ADDHALF,
	"-" C_127_BY_255,  // SUBHALF,
	"",
};

static const char *tevOpTable[] = { // TEV
	"+",      // TEVOP_ADD = 0,
	"-",      // TEVOP_SUB = 1,
};

static const char *tevCInputTable[] = // CC
{
	"(prev.rgb)",         // CPREV,
	"(prev.aaa)",         // APREV,
	"(c0.rgb)",           // C0,
	"(c0.aaa)",           // A0,
	"(c1.rgb)",           // C1,
	"(c1.aaa)",           // A1,
	"(c2.rgb)",           // C2,
	"(c2.aaa)",           // A2,
	"(tex_t.rgb)",      // TEXC,
	"(tex_t.aaa)",      // TEXA,
	"(ras_t.rgb)",      // RASC,
	"(ras_t.aaa)",      // RASA,
	"float3(1.0,1.0,1.0)",              // ONE
	"float3(" C_127_BY_255 "," C_127_BY_255 "," C_127_BY_255 ")",              // HALF
	"(konst_t.rgb)", //"konst_t.rgb",    // KONST
	"float3(0.0,0.0,0.0)",              // ZERO
	///added extra values to map clamped values
	"(cprev.rgb)",        // CPREV,
	"(cprev.aaa)",        // APREV,
	"(cc0.rgb)",          // C0,
	"(cc0.aaa)",          // A0,
	"(cc1.rgb)",          // C1,
	"(cc1.aaa)",          // A1,
	"(cc2.rgb)",          // C2,
	"(cc2.aaa)",          // A2,
	"(tex_t.rgb)",      // TEXC,
	"(tex_t.aaa)",      // TEXA,
	"(cras_t.rgb)",     // RASC,
	"(cras_t.aaa)",     // RASA,
	"float3(1.0,1.0,1.0)",              // ONE
	"float3(" C_127_BY_255 "," C_127_BY_255 "," C_127_BY_255 ")",              // HALF
	"(ckonst_t.rgb)", //"konst_t.rgb",   // KONST
	"float3(0.0,0.0,0.0)",              // ZERO
	"PADERROR1", "PADERROR2", "PADERROR3", "PADERROR4"
};

static const char *tevAInputTable[] = // CA
{
	"prev",            // APREV,
	"c0",              // A0,
	"c1",              // A1,
	"c2",              // A2,
	"tex_t",         // TEXA,
	"ras_t",         // RASA,
	"konst_t",       // KONST,  (hw1 had quarter)
	"float4(0.0,0.0,0.0,0.0)", // ZERO
	///added extra values to map clamped values
	"cprev",            // APREV,
	"cc0",              // A0,
	"cc1",              // A1,
	"cc2",              // A2,
	"tex_t",          // TEXA,
	"cras_t",         // RASA,
	"ckonst_t",       // KONST,  (hw1 had quarter)
	"float4(0.0,0.0,0.0,0.0)", // ZERO
	"PADERROR5", "PADERROR6", "PADERROR7", "PADERROR8",
	"PADERROR9", "PADERROR10", "PADERROR11", "PADERROR12",
};

static const char *tevRasTable[] =
{
	"colors_0",
	"colors_1",
	"ERROR13", //2
	"ERROR14", //3
	"ERROR15", //4
	"float4(a_bump,a_bump,a_bump,a_bump)", // use bump alpha
	"(float4(a_bump,a_bump,a_bump,a_bump)*(255.0/248.0))", //normalized
	"float4(0.0, 0.0, 0.0, 0.0)", // zero
};

//static const char *tevTexFunc[] = { "tex2D", "texRECT" };

static const char *tevCOutputTable[]  = { "prev.rgb", "c0.rgb", "c1.rgb", "c2.rgb" };
static const char *tevAOutputTable[]  = { "prev.a", "c0.a", "c1.a", "c2.a" };
static const char *tevIndAlphaSel[]   = {"", "x", "y", "z"};
static const char *tevIndAlphaScale[] = {"(1.0/8.0)", "(1.0/32.0)", "(1.0/16.0)", "(1.0/8.0)"};
static const char *tevIndAlphaNormFactor[] = 
{
	"(8.0/255.0)",	// 5 bits
	"(32.0/255.0)",	// 3 bits
	"(16.0/255.0)",	// 4 bits
	"(8.0/255.0)"		// 5 bits
};
static const char *tevIndBiasField[]  = {"", "x", "y", "xy", "z", "xz", "yz", "xyz"}; // indexed by bias
static const char *tevIndBiasAdd[]    = {"-128.0", "1.0", "1.0", "1.0" }; // indexed by fmt
static const char *tevIndWrapStart[]  = {"0.0", "256.0", "128.0", "64.0", "32.0", "16.0", "0.001f" };
static const char *tevIndFmtScale[]   = 
{
	"255.0",					// 8 bits
	"((255.0*8.0)/256.0)",	// 5 bits 
	"((255.0*16.0)/256.0)",	// 4 bits
	"((255.0*32.0)/256.0)"	// 3 bits
};

static const char *tevIndFmtNormFactor[]   = 
{
	"",					// 8 bits
	"(256.0/8.0)",	// 5 bits 
	"(256.0/16.0)",	// 4 bits
	"(256.0/32.0)"	// 3 bits
};

struct RegisterState
{
	bool ColorNeedOverflowControl;
	bool AlphaNeedOverflowControl;
	bool AuxStored;
};

static char text[16384];

template<class T, bool Write_Code, API_TYPE ApiType> inline void WriteStage(T& out, pixel_shader_uid_data& uid_data, int n, RegisterState RegisterStates[4], const char swapModeTable[4][5]);
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
	if(Write_Code)
	{
		out.SetBuffer(text);
		#ifndef ANDROID			
			locale = newlocale(LC_NUMERIC_MASK, "C", NULL); // New locale for compilation
			old_locale = uselocale(locale); // Apply the locale for this thread			
		#endif
		text[sizeof(text) - 1] = 0x7C;  // canary
	}

	unsigned int numStages = bpmem.genMode.numtevstages + 1;
	unsigned int numTexgen = bpmem.genMode.numtexgens;

	const bool forced_early_z = g_ActiveConfig.backend_info.bSupportsEarlyZ && bpmem.UseEarlyDepthTest();
	const bool per_pixel_depth = (bpmem.ztex2.op != ZTEXTURE_DISABLE && bpmem.UseLateDepthTest()) || (!g_ActiveConfig.bFastDepthCalc && bpmem.zmode.testenable && !forced_early_z && dstAlphaMode != DSTALPHA_NULL);

	uid_data.dstAlphaMode = dstAlphaMode;
	uid_data.genMode_numindstages = bpmem.genMode.numindstages;
	uid_data.genMode_numtevstages = bpmem.genMode.numtevstages;
	uid_data.genMode_numtexgens = bpmem.genMode.numtexgens;
	
	if (Write_Code)
	{
		out.Write("//Pixel Shader for TEV stages\n");
		out.Write("//%i TEV stages, %i texgens, %i IND stages\n",
			numStages, numTexgen, bpmem.genMode.numindstages);
		out.Write("#define F_P_U8(x) (round((x) * 255.0) / 255.0)\n");
		out.Write("#define CHK_O_U8(x) (frac(((x) + " C_1024_BY_255 ") * (255.0/256.0)) * (256.0/255.0))\n");
		//out.Write("#define CHK_O_U24(x) (frac((x) * (16777215.0/16777216.0)) * (16777216.0/16777215.0))\n");	
		// Fastmod implementation with the restriction that "y" must be always positive
		out.Write("float fastmod( float x, float y )\n");
		out.Write("{\n");
		out.Write("\ty = sign(x) * y;\n");
		out.Write("\treturn frac(x/y)*y;\n");
		out.Write("}\n");		

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
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_INDTEXSCALE, "float4", I_INDTEXSCALE "[2]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_INDTEXMTX, "float4", I_INDTEXMTX "[6]");
		DeclareUniform<T, ApiType>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_FOG, "float4", I_FOG "[3]");

		if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
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
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					out.Write("VARYIN float4 Normal_2;\n");
				}
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
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
						out.Write("VARYIN float%d uv%d_2;\n", i < 4 ? 4 : 3 , i);
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
			if(ApiType != API_D3D11)
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
				if(g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
					out.Write(",\n  in %s float4 Normal : TEXCOORD%d", optCentroid, numTexgen + 1);
				out.Write("        ) {\n");
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if(g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					for (u32 i = 0; i < 8; ++i)
						out.Write(",\n  in float4 uv%d : TEXCOORD%d", i, i);
				}
				else
				{
					for (u32 i = 0; i < numTexgen; ++i)
						out.Write(",\n  in float%d uv%d : TEXCOORD%d", i < 4 ? 4 : 3 , i, i);
				}
				out.Write("        ) {\n");
				out.Write("\tfloat4 clipPos = float4(0.0, 0.0, 0.0, 0.0);");
			}
		}
		if (dstAlphaMode == DSTALPHA_NULL)
		{
			out.Write("ocol0 = float4(0.0, 0.0, 0.0, 0.0);\n");
			out.Write("}\n");
			if (text[sizeof(text) - 1] != 0x7C)
				PanicAlert("PixelShader generator - buffer too small, canary has been eaten!");

#ifndef ANDROID
			uselocale(old_locale); // restore locale
			freelocale(locale);
#endif
			return;
		}
		out.Write("  float4 c0 = " I_COLORS "[1], c1 = " I_COLORS "[2], c2 = " I_COLORS "[3], prev = " I_COLORS "[0], tex_t = float4(0.0,0.0,0.0,0.0), ras_t = float4(0.0,0.0,0.0,0.0), konst_t = float4(0.0,0.0,0.0,0.0);\n"
				"  float3 comp16 = float3(1.0,255.0,0.0), comp24 = float3(1.0,255.0,255.0*255.0);\n"
				"  float a_bump=0.0;\n"
				"  float3 tevcoord=float3(0.0, 0.0, 0.0);\n"
				"  float2 wrappedcoord=float2(0.0,0.0), t_coord=float2(0.0,0.0);\n"
				"  float4 cc0=float4(0.0,0.0,0.0,0.0), cc1=float4(0.0,0.0,0.0,0.0);\n"
				"  float4 cc2=float4(0.0,0.0,0.0,0.0), cprev=float4(0.0,0.0,0.0,0.0);\n"
				"  float4 cras_t=float4(0.0,0.0,0.0,0.0),ckonst_t=float4(0.0,0.0,0.0,0.0);\n\n");

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
				if(numTexgen)
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("float3 uv%d = uv%d_2;\n", i, i);
					}
				}
				out.Write("float4 clipPos = clipPos_2;\n");
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					out.Write("float4 Normal = Normal_2;\n");
				}
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("float4 uv%d = uv%d_2;\n", i, i);
					}
				}
				else
				{
					for (u32 i = 0; i < numTexgen; ++i)
					{
						out.Write("float%d uv%d = uv%d_2;\n", i < 4 ? 4 : 3 , i, i);
					}
				}
			}
		}
	}
	

	if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
	{
		if (Write_Code)
		{
			if (numTexgen < 7)
			{
				out.Write("\tfloat3 _norm0 = normalize(Normal.xyz);\n\n");
				out.Write("\tfloat3 pos = float3(clipPos.x,clipPos.y,Normal.w);\n");
			}
			else
			{
				out.Write("\tfloat3 _norm0 = normalize(float3(uv4.w,uv5.w,uv6.w));\n\n");
				out.Write("\tfloat3 pos = float3(uv0.w,uv1.w,uv7.w);\n");
			}

			out.Write("\tfloat4 mat, lacc;\n"
					"\tfloat3 ldir, h;\n"
					"\tfloat dist, dist2, attn;\n");
		}
		out.SetConstantsUsed(C_PLIGHTS, C_PLIGHTS+39); // TODO: Can be optimized further
		out.SetConstantsUsed(C_PMATERIALS, C_PMATERIALS+3);
		uid_data.components = components;
		GenerateLightingShader<T, Write_Code>(out, uid_data.lighting, components, I_PMATERIALS, I_PLIGHTS, "colors_", "colors_");
	}
	if (Write_Code)
	{
		if (numTexgen < 7)
			out.Write("\tclipPos = float4(rawpos.x, rawpos.y, clipPos.z, clipPos.w);\n");
		else
			out.Write("\tclipPos = float4(rawpos.x, rawpos.y, uv2.w, uv3.w);\n");
	}
	

	// HACK to handle cases where the tex gen is not enabled
	if (numTexgen == 0)
	{
		if (Write_Code)
			out.Write("\tfloat3 uv0 = float3(0.0, 0.0, 0.0);\n");
	}
	else
	{
		out.SetConstantsUsed(C_TEXDIMS, C_TEXDIMS+numTexgen-1);
		for (unsigned int i = 0; i < numTexgen; ++i)
		{
			// optional perspective divides
			uid_data.texMtxInfo_n_projection |= xfregs.texMtxInfo[i].projection << i;
			if (Write_Code)
			{
				if (xfregs.texMtxInfo[i].projection == XF_TEXPROJ_STQ)
				{
					out.Write("\tif (uv%d.z != 0.0)", i);
					out.Write("\t\tuv%d.xy = uv%d.xy / uv%d.z;\n", i, i, i);
				}
				out.Write("uv%d.xy = uv%d.xy * " I_TEXDIMS"[%d].zw;\n", i, i, i);
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
			if (texcoord < numTexgen)
			{
				out.SetConstantsUsed(C_INDTEXSCALE+i/2,C_INDTEXSCALE+i/2);
				if (Write_Code)
				{
					out.Write("\tt_coord = uv%d.xy * " I_INDTEXSCALE "[%d].%s;\n", texcoord, i/2, (i&1)?"zw":"xy");
				}				
			}
			else if (Write_Code)
			{
				out.Write("\tt_coord = float2(0.0, 0.0);\n");
			}
			
			if (Write_Code)
			{
				out.Write("float3 indtex%d = ", i);
			}
			SampleTexture<T, Write_Code, ApiType>(out, "t_coord", "abg", texmap);
		}
	}

	RegisterState RegisterStates[4];
	RegisterStates[0].AlphaNeedOverflowControl = false;
	RegisterStates[0].ColorNeedOverflowControl = false;
	RegisterStates[0].AuxStored = false;
	for(int i = 1; i < 4; i++)
	{
		RegisterStates[i].AlphaNeedOverflowControl = true;
		RegisterStates[i].ColorNeedOverflowControl = true;
		RegisterStates[i].AuxStored = false;
	}

	// Uid fields for BuildSwapModeTable are set in WriteStage
	char swapModeTable[4][5];
	const char* swapColors = "rgba";
	for (int i = 0; i < 4; i++)
	{
		swapModeTable[i][0] = swapColors[bpmem.tevksel[i*2].swap1];
		swapModeTable[i][1] = swapColors[bpmem.tevksel[i*2].swap2];
		swapModeTable[i][2] = swapColors[bpmem.tevksel[i*2+1].swap1];
		swapModeTable[i][3] = swapColors[bpmem.tevksel[i*2+1].swap2];
		swapModeTable[i][4] = '\0';
	}

	for (unsigned int i = 0; i < numStages; i++)
		WriteStage<T, Write_Code, ApiType>(out, uid_data, i, RegisterStates, swapModeTable); // build the equation for this stage

	bool enable_pl = g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting;
	uid_data.pixel_lighting = enable_pl;

	if (Write_Code)
	{
		if (numStages)
		{
			// The results of the last texenv stage are put onto the screen,
			// regardless of the used destination register
			if(bpmem.combiners[numStages - 1].colorC.dest != 0)
			{
				bool retrieveFromAuxRegister = !RegisterStates[bpmem.combiners[numStages - 1].colorC.dest].ColorNeedOverflowControl && RegisterStates[bpmem.combiners[numStages - 1].colorC.dest].AuxStored;
				out.Write("\tprev.rgb = %s%s;\n", retrieveFromAuxRegister ? "c" : "" , tevCOutputTable[bpmem.combiners[numStages - 1].colorC.dest]);
				RegisterStates[0].ColorNeedOverflowControl = RegisterStates[bpmem.combiners[numStages - 1].colorC.dest].ColorNeedOverflowControl;
			}
			if(bpmem.combiners[numStages - 1].alphaC.dest != 0)
			{
				bool retrieveFromAuxRegister = !RegisterStates[bpmem.combiners[numStages - 1].alphaC.dest].AlphaNeedOverflowControl && RegisterStates[bpmem.combiners[numStages - 1].alphaC.dest].AuxStored;
				out.Write("\tprev.a = %s%s;\n", retrieveFromAuxRegister ? "c" : "" , tevAOutputTable[bpmem.combiners[numStages - 1].alphaC.dest]);
				RegisterStates[0].AlphaNeedOverflowControl = RegisterStates[bpmem.combiners[numStages - 1].alphaC.dest].AlphaNeedOverflowControl;
			}
		}
		// emulation of unsigned 8 overflow when casting if needed
		if(RegisterStates[0].AlphaNeedOverflowControl || RegisterStates[0].ColorNeedOverflowControl)
			out.Write("\tprev = CHK_O_U8(prev);\n");
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
		out.SetConstantsUsed(C_ZBIAS+1, C_ZBIAS+1);
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
		out.SetConstantsUsed(C_ZBIAS, C_ZBIAS+1);
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
		out.SetConstantsUsed(C_ALPHA, C_ALPHA);
		if (Write_Code)
			out.Write("\tocol0 = float4(prev.rgb, " I_ALPHA ".a);\n");
	}
	else
	{
		WriteFog<T, Write_Code>(out, uid_data);
		if (Write_Code)
			out.Write("\tocol0 = prev;\n");
	}

	// Use dual-source color blending to perform dst alpha in a single pass
	if (dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND)
	{
		out.SetConstantsUsed(C_ALPHA, C_ALPHA);
		if (Write_Code)
		{
			if(ApiType & API_D3D9)
			{
				// alpha component must be 0 or the shader will not compile (Direct3D 9Ex restriction)
				// Colors will be blended against the color from ocol1 in D3D 9...
				out.Write("\tocol1 = float4(prev.a, prev.a, prev.a, 0.0);\n");
			}
			else
			{
				// Colors will be blended against the alpha from ocol1...
				out.Write("\tocol1 = prev;\n");
			}
			// ...and the alpha from ocol0 will be written to the framebuffer.
			out.Write("\tocol0.a = " I_ALPHA ".a;\n");
		}
	}

	if (Write_Code)
	{
		out.Write("}\n");
		if (text[sizeof(text) - 1] != 0x7C)
			PanicAlert("PixelShader generator - buffer too small, canary has been eaten!");

#ifndef ANDROID
		uselocale(old_locale); // restore locale
		freelocale(locale);
#endif
	}
}



//table with the color compare operations
static const char *TEVCMPColorOPTable[16] =
{
	"float3(0.0, 0.0, 0.0)",//0
	"float3(0.0, 0.0, 0.0)",//1
	"float3(0.0, 0.0, 0.0)",//2
	"float3(0.0, 0.0, 0.0)",//3
	"float3(0.0, 0.0, 0.0)",//4
	"float3(0.0, 0.0, 0.0)",//5
	"float3(0.0, 0.0, 0.0)",//6
	"float3(0.0, 0.0, 0.0)",//7
	"   %s + ((%s.r >= %s.r + (0.25f/255.0)) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_R8_GT 8
	"   %s + ((abs(%s.r - %s.r) < (0.5/255.0)) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_R8_EQ 9
	"   %s + (( dot(%s.rgb, comp16) >= (dot(%s.rgb, comp16) + (0.25f/255.0))) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_GR16_GT 10
	"   %s + (abs(dot(%s.rgb, comp16) - dot(%s.rgb, comp16)) < (0.5/255.0) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_GR16_EQ 11
	"   %s + (( dot(%s.rgb, comp24) >= (dot(%s.rgb, comp24) + (0.25f/255.0))) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_BGR24_GT 12
	"   %s + (abs(dot(%s.rgb, comp24) - dot(%s.rgb, comp24)) < (0.5/255.0) ? %s : float3(0.0, 0.0, 0.0))",//#define TEVCMP_BGR24_EQ 13
	"   %s + (max(sign(%s.rgb - %s.rgb - (0.25f/255.0)), float3(0.0, 0.0, 0.0)) * %s)",//#define TEVCMP_RGB8_GT  14
	"   %s + ((float3(1.0, 1.0, 1.0) - max(sign(abs(%s.rgb - %s.rgb) - (0.5/255.0)), float3(0.0, 0.0, 0.0))) * %s)"//#define TEVCMP_RGB8_EQ  15
};

//table with the alpha compare operations
static const char *TEVCMPAlphaOPTable[16] =
{
	"0.0",//0
	"0.0",//1
	"0.0",//2
	"0.0",//3
	"0.0",//4
	"0.0",//5
	"0.0",//6
	"0.0",//7
	"   %s.a + ((%s.r >= (%s.r + (0.25f/255.0))) ? %s.a : 0.0)",//#define TEVCMP_R8_GT 8
	"   %s.a + (abs(%s.r - %s.r) < (0.5/255.0) ? %s.a : 0.0)",//#define TEVCMP_R8_EQ 9
	"   %s.a + ((dot(%s.rgb, comp16) >= (dot(%s.rgb, comp16) + (0.25/255.0))) ? %s.a : 0.0)",//#define TEVCMP_GR16_GT 10
	"   %s.a + (abs(dot(%s.rgb, comp16) - dot(%s.rgb, comp16)) < (0.5/255.0) ? %s.a : 0.0)",//#define TEVCMP_GR16_EQ 11
	"   %s.a + ((dot(%s.rgb, comp24) >= (dot(%s.rgb, comp24) + (0.25/255.0))) ? %s.a : 0.0)",//#define TEVCMP_BGR24_GT 12
	"   %s.a + (abs(dot(%s.rgb, comp24) - dot(%s.rgb, comp24)) < (0.5/255.0) ? %s.a : 0.0)",//#define TEVCMP_BGR24_EQ 13
	"   %s.a + ((%s.a >= (%s.a + (0.25/255.0))) ? %s.a : 0.0)",//#define TEVCMP_A8_GT 14
	"   %s.a + (abs(%s.a - %s.a) < (0.5/255.0) ? %s.a : 0.0)"//#define TEVCMP_A8_EQ 15
};

template<class T, bool Write_Code, API_TYPE ApiType>
static inline void WriteStage(T& out, pixel_shader_uid_data& uid_data, int n, RegisterState RegisterStates[4], const char swapModeTable[4][5])
{
	int texcoord = bpmem.tevorders[n/2].getTexCoord(n&1);
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
				// a_bump is taked from the remaining upper bits that are not taked for indirect texturing
				// if all the bits are used for indirect texturing then only the upper 5 bits are used for a_bump
				// so to do this bit masking is used for example 
				// a_bump = x & 0xE0; for 3 bits
				// a_bump = x & 0xF0; for 4 bits
				// a_bump = x & 0xF8; for 5 bits
				// there is no support for bitmasking in older hardware
				// and in newer hardware is slower than using pure float operations
				// so we have to emulate it
				// the exact formula for float masking emulation of the upper bits of a number is: 
				// having x as the integer number to mask stored in a float
				// nb as the number of bits to mask
				// and n the (wordlen - nb) in this case (8 - nb)
				// then result = floor((x + 0.5) / (2^n)) * (2^n)
				// so for 3 bit this will be n = 5  result = floor((x + 0.5) / 32.0) * 32.0;
				// to optimize a litle al the coeficient are precalculated to avoid slowing thigs more than needed
				out.Write("a_bump = floor((indtex%d.%s * 255.0 + 0.5) * %s) * %s;\n",
					bpmem.tevind[n].bt,
					tevIndAlphaSel[bpmem.tevind[n].bs],					
					tevIndAlphaScale[bpmem.tevind[n].fmt],
					tevIndAlphaNormFactor[bpmem.tevind[n].fmt]);
			
			}
			// format
			if (bpmem.tevind[n].fmt)
			{
				// to mask the lower bits the formula is:
				// having x as the integer number to mask stored in a float
				// nb as the number of bits to mask
				// and n the (wordlen - nb) in this case (8 - nb)
				// then result = frac(x * (2^n) / 256.0) * (256.0/(2^n))
				// for 3 bits n = 5 then result = frac(x * 32.0 / 256.0) * (256.0/32.0)
				out.Write("float3 indtevcrd%d = frac(indtex%d * %s) * %s;\n", n, bpmem.tevind[n].bt, tevIndFmtScale[bpmem.tevind[n].fmt], tevIndFmtNormFactor[bpmem.tevind[n].fmt]);
			}
			else
			{
				// if we take all the bits is just a multiplication
				out.Write("float3 indtevcrd%d = indtex%d  * %s;\n", n, bpmem.tevind[n].bt, tevIndFmtScale[bpmem.tevind[n].fmt]);
			}
			// bias
			if (bpmem.tevind[n].bias != ITB_NONE )
				out.Write("indtevcrd%d.%s += %s;\n", n, tevIndBiasField[bpmem.tevind[n].bias], tevIndBiasAdd[bpmem.tevind[n].fmt]);
		}
		

		// multiply by offset matrix and scale
		if (bpmem.tevind[n].mid != 0)
		{
			if (bpmem.tevind[n].mid <= 3)
			{
				int mtxidx = 2*(bpmem.tevind[n].mid-1);
				out.SetConstantsUsed(C_INDTEXMTX+mtxidx, C_INDTEXMTX+mtxidx);
				if (Write_Code)
					out.Write("float2 indtevtrans%d = float2(dot(" I_INDTEXMTX "[%d].xyz, indtevcrd%d), dot(" I_INDTEXMTX "[%d].xyz, indtevcrd%d));\n",
							n, mtxidx, n, mtxidx+1, n);
			}
			else if (bpmem.tevind[n].mid <= 7 && bHasTexCoord)
			{ // s matrix
				_assert_(bpmem.tevind[n].mid >= 5);
				int mtxidx = 2*(bpmem.tevind[n].mid-5);
				out.SetConstantsUsed(C_INDTEXMTX+mtxidx, C_INDTEXMTX+mtxidx);
				if (Write_Code)
					out.Write("float2 indtevtrans%d = " I_INDTEXMTX "[%d].ww * uv%d.xy * indtevcrd%d.xx;\n", n, mtxidx, texcoord, n);
			}
			else if (bpmem.tevind[n].mid <= 11 && bHasTexCoord)
			{ // t matrix
				_assert_(bpmem.tevind[n].mid >= 9);
				int mtxidx = 2*(bpmem.tevind[n].mid-9);
				out.SetConstantsUsed(C_INDTEXMTX+mtxidx, C_INDTEXMTX+mtxidx);
				if (Write_Code)
					out.Write("float2 indtevtrans%d = " I_INDTEXMTX "[%d].ww * uv%d.xy * indtevcrd%d.yy;\n", n, mtxidx, texcoord, n);
			}
			else
			{
				if (Write_Code)
					out.Write("float2 indtevtrans%d = float2(0.0, 0.0);\n", n);
			}
		}
		else
		{
			if (Write_Code)
				out.Write("float2 indtevtrans%d = float2(0.0, 0.0);\n", n);
		}

		// ---------
		// Wrapping
		// ---------

		// wrap S
		if (Write_Code)
		{
			if (bpmem.tevind[n].sw == ITW_OFF)
				out.Write("wrappedcoord.x = uv%d.x;\n", texcoord);
			else if (bpmem.tevind[n].sw == ITW_0)
				out.Write("wrappedcoord.x = 0.0;\n");
			else
				out.Write("wrappedcoord.x = fastmod( uv%d.x, %s );\n", texcoord, tevIndWrapStart[bpmem.tevind[n].sw]);

			// wrap T
			if (bpmem.tevind[n].tw == ITW_OFF)
				out.Write("wrappedcoord.y = uv%d.y;\n", texcoord);
			else if (bpmem.tevind[n].tw == ITW_0)
				out.Write("wrappedcoord.y = 0.0;\n");
			else
				out.Write("wrappedcoord.y = fastmod( uv%d.y, %s );\n", texcoord, tevIndWrapStart[bpmem.tevind[n].tw]);

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

	if(cc.a == TEVCOLORARG_RASA || cc.a == TEVCOLORARG_RASC
		|| cc.b == TEVCOLORARG_RASA || cc.b == TEVCOLORARG_RASC
		|| cc.c == TEVCOLORARG_RASA || cc.c == TEVCOLORARG_RASC
		|| cc.d == TEVCOLORARG_RASA || cc.d == TEVCOLORARG_RASC
		|| ac.a == TEVALPHAARG_RASA || ac.b == TEVALPHAARG_RASA
		|| ac.c == TEVALPHAARG_RASA || ac.d == TEVALPHAARG_RASA)
	{
		const int i = bpmem.combiners[n].alphaC.rswap;
		uid_data.stagehash[n].ac |= bpmem.combiners[n].alphaC.rswap;
		uid_data.stagehash[n].tevksel_swap1a = bpmem.tevksel[i*2].swap1;
		uid_data.stagehash[n].tevksel_swap2a = bpmem.tevksel[i*2].swap2;
		uid_data.stagehash[n].tevksel_swap1b = bpmem.tevksel[i*2+1].swap1;
		uid_data.stagehash[n].tevksel_swap2b = bpmem.tevksel[i*2+1].swap2;
		uid_data.stagehash[n].tevorders_colorchan = bpmem.tevorders[n / 2].getColorChan(n & 1);

		const char *rasswap = swapModeTable[bpmem.combiners[n].alphaC.rswap];
		if (Write_Code)
		{
			out.Write("ras_t = %s.%s;\n", tevRasTable[bpmem.tevorders[n / 2].getColorChan(n & 1)], rasswap);
			out.Write("cras_t = CHK_O_U8(ras_t);\n");
		}
	}

	uid_data.stagehash[n].tevorders_enable = bpmem.tevorders[n / 2].getEnable(n & 1);
	if (bpmem.tevorders[n/2].getEnable(n&1))
	{
		if (Write_Code)
		{
			if (!bHasIndStage)
			{
				// calc tevcord
				if(bHasTexCoord)
					out.Write("tevcoord.xy = uv%d.xy;\n", texcoord);
				else
					out.Write("tevcoord.xy = float2(0.0, 0.0);\n");
			}
		}
		const int i = bpmem.combiners[n].alphaC.tswap;
		uid_data.stagehash[n].ac |= bpmem.combiners[n].alphaC.tswap << 2;
		uid_data.stagehash[n].tevksel_swap1c = bpmem.tevksel[i*2].swap1;
		uid_data.stagehash[n].tevksel_swap2c = bpmem.tevksel[i*2].swap2;
		uid_data.stagehash[n].tevksel_swap1d = bpmem.tevksel[i*2+1].swap1;
		uid_data.stagehash[n].tevksel_swap2d = bpmem.tevksel[i*2+1].swap2;

		uid_data.stagehash[n].tevorders_texmap= bpmem.tevorders[n/2].getTexMap(n&1);

		const char *texswap = swapModeTable[bpmem.combiners[n].alphaC.tswap];
		int texmap = bpmem.tevorders[n/2].getTexMap(n&1);
		uid_data.SetTevindrefTexmap(i, texmap);
		if (Write_Code)
			out.Write("tex_t = ");

		SampleTexture<T, Write_Code, ApiType>(out, "tevcoord", texswap, texmap);
	}
	else if (Write_Code)
	{
		out.Write("tex_t = float4(1.0,1.0,1.0,1.0);\n");
	}


	if (cc.a == TEVCOLORARG_KONST || cc.b == TEVCOLORARG_KONST || cc.c == TEVCOLORARG_KONST || cc.d == TEVCOLORARG_KONST
		|| ac.a == TEVALPHAARG_KONST || ac.b == TEVALPHAARG_KONST || ac.c == TEVALPHAARG_KONST || ac.d == TEVALPHAARG_KONST)
	{
		int kc = bpmem.tevksel[n / 2].getKC(n & 1);
		int ka = bpmem.tevksel[n / 2].getKA(n & 1);
		uid_data.stagehash[n].tevksel_kc = kc;
		uid_data.stagehash[n].tevksel_ka = ka;
		if (Write_Code)
		{
			out.Write("konst_t = float4(%s, %s);\n", tevKSelTableC[kc], tevKSelTableA[ka]);
			if(kc > 7 || ka > 7)
			{
				out.Write("ckonst_t = CHK_O_U8(konst_t);\n");
			}
			else
			{
				out.Write("ckonst_t = konst_t;\n");
			}
		}

		if (kc > 7)
			out.SetConstantsUsed(C_KCOLORS+((kc-0xc)%4),C_KCOLORS+((kc-0xc)%4));
		if (ka > 7)
			out.SetConstantsUsed(C_KCOLORS+((ka-0xc)%4),C_KCOLORS+((ka-0xc)%4));
	}
	if (Write_Code)
	{
		if(cc.a == TEVCOLORARG_CPREV || cc.a == TEVCOLORARG_APREV
			|| cc.b == TEVCOLORARG_CPREV || cc.b == TEVCOLORARG_APREV
			|| cc.c == TEVCOLORARG_CPREV || cc.c == TEVCOLORARG_APREV
			|| ac.a == TEVALPHAARG_APREV || ac.b == TEVALPHAARG_APREV || ac.c == TEVALPHAARG_APREV)
		{
			if(RegisterStates[0].AlphaNeedOverflowControl || RegisterStates[0].ColorNeedOverflowControl)
			{
				out.Write("cprev = CHK_O_U8(prev);\n");
				RegisterStates[0].AlphaNeedOverflowControl = false;
				RegisterStates[0].ColorNeedOverflowControl = false;
			}
			else
			{
				out.Write("cprev = prev;\n");
			}
			RegisterStates[0].AuxStored = true;
		}
	}

	if(cc.a == TEVCOLORARG_C0 || cc.a == TEVCOLORARG_A0
	|| cc.b == TEVCOLORARG_C0 || cc.b == TEVCOLORARG_A0
	|| cc.c == TEVCOLORARG_C0 || cc.c == TEVCOLORARG_A0
	|| ac.a == TEVALPHAARG_A0 || ac.b == TEVALPHAARG_A0 || ac.c == TEVALPHAARG_A0)
	{
		out.SetConstantsUsed(C_COLORS+1,C_COLORS+1);
		if (Write_Code)
		{
			if(RegisterStates[1].AlphaNeedOverflowControl || RegisterStates[1].ColorNeedOverflowControl)
			{
				out.Write("cc0 = CHK_O_U8(c0);\n");
				RegisterStates[1].AlphaNeedOverflowControl = false;
				RegisterStates[1].ColorNeedOverflowControl = false;
			}
			else
			{
				out.Write("cc0 = c0;\n");
			}
			RegisterStates[1].AuxStored = true;
		}
	}

	if(cc.a == TEVCOLORARG_C1 || cc.a == TEVCOLORARG_A1
	|| cc.b == TEVCOLORARG_C1 || cc.b == TEVCOLORARG_A1
	|| cc.c == TEVCOLORARG_C1 || cc.c == TEVCOLORARG_A1
	|| ac.a == TEVALPHAARG_A1 || ac.b == TEVALPHAARG_A1 || ac.c == TEVALPHAARG_A1)
	{
		out.SetConstantsUsed(C_COLORS+2,C_COLORS+2);
		if (Write_Code)
		{
			if(RegisterStates[2].AlphaNeedOverflowControl || RegisterStates[2].ColorNeedOverflowControl)
			{
				out.Write("cc1 = CHK_O_U8(c1);\n");
				RegisterStates[2].AlphaNeedOverflowControl = false;
				RegisterStates[2].ColorNeedOverflowControl = false;
			}
			else
			{
				out.Write("cc1 = c1;\n");
			}
			RegisterStates[2].AuxStored = true;
		}
	}

	if(cc.a == TEVCOLORARG_C2 || cc.a == TEVCOLORARG_A2
	|| cc.b == TEVCOLORARG_C2 || cc.b == TEVCOLORARG_A2
	|| cc.c == TEVCOLORARG_C2 || cc.c == TEVCOLORARG_A2
	|| ac.a == TEVALPHAARG_A2 || ac.b == TEVALPHAARG_A2 || ac.c == TEVALPHAARG_A2)
	{
		out.SetConstantsUsed(C_COLORS+3,C_COLORS+3);
		if (Write_Code)
		{
			if(RegisterStates[3].AlphaNeedOverflowControl || RegisterStates[3].ColorNeedOverflowControl)
			{
				out.Write("cc2 = CHK_O_U8(c2);\n");
				RegisterStates[3].AlphaNeedOverflowControl = false;
				RegisterStates[3].ColorNeedOverflowControl = false;
			}
			else
			{
				out.Write("cc2 = c2;\n");
			}
			RegisterStates[3].AuxStored = true;
		}
	}

	RegisterStates[cc.dest].ColorNeedOverflowControl = (cc.clamp == 0);
	RegisterStates[cc.dest].AuxStored = false;

	if (cc.d == TEVCOLORARG_C0 || cc.d == TEVCOLORARG_A0 || ac.d == TEVALPHAARG_A0)
		out.SetConstantsUsed(C_COLORS+1,C_COLORS+1);

	if (cc.d == TEVCOLORARG_C1 || cc.d == TEVCOLORARG_A1 || ac.d == TEVALPHAARG_A1)
		out.SetConstantsUsed(C_COLORS+2,C_COLORS+2);

	if (cc.d == TEVCOLORARG_C2 || cc.d == TEVCOLORARG_A2 || ac.d == TEVALPHAARG_A2)
		out.SetConstantsUsed(C_COLORS+3,C_COLORS+3);

	if (cc.dest >= GX_TEVREG0 && cc.dest <= GX_TEVREG2)
		out.SetConstantsUsed(C_COLORS+cc.dest, C_COLORS+cc.dest);

	if (ac.dest >= GX_TEVREG0 && ac.dest <= GX_TEVREG2)
		out.SetConstantsUsed(C_COLORS+ac.dest, C_COLORS+ac.dest);

	if (Write_Code)
	{
		out.Write("// color combine\n");
		out.Write("%s = F_P_U8(clamp(", tevCOutputTable[cc.dest]);
		// combine the color channel
		if (cc.bias != TevBias_COMPARE) // if not compare
		{
			//normal color combiner goes here
			if (cc.shift > TEVSCALE_1)
				out.Write("%s*(", tevScaleTable[cc.shift]);

			if(!(cc.d == TEVCOLORARG_ZERO && cc.op == TEVOP_ADD))
				out.Write("%s%s", tevCInputTable[cc.d], tevOpTable[cc.op]);

			if (cc.a == cc.b)
				out.Write("%s", tevCInputTable[cc.a + 16]);
			else if (cc.c == TEVCOLORARG_ZERO)
				out.Write("%s", tevCInputTable[cc.a + 16]);
			else if (cc.c == TEVCOLORARG_ONE)
				out.Write("%s", tevCInputTable[cc.b + 16]);
			else if (cc.a == TEVCOLORARG_ZERO)
				out.Write("%s*%s", tevCInputTable[cc.b + 16], tevCInputTable[cc.c + 16]);
			else if (cc.b == TEVCOLORARG_ZERO)
				out.Write("%s*(float3(1.0, 1.0, 1.0)-%s)", tevCInputTable[cc.a + 16], tevCInputTable[cc.c + 16]);
			else
				out.Write("lerp(%s, %s, %s)", tevCInputTable[cc.a + 16], tevCInputTable[cc.b + 16], tevCInputTable[cc.c + 16]);

			out.Write("%s", tevBiasTable[cc.bias]);

			if (cc.shift > TEVSCALE_1)
				out.Write(")");
		}
		else
		{
			int cmp = (cc.shift<<1)|cc.op|8; // comparemode stored here
			out.Write(TEVCMPColorOPTable[cmp],//lookup the function from the op table
					tevCInputTable[cc.d],
					tevCInputTable[cc.a + 16],
					tevCInputTable[cc.b + 16],
					tevCInputTable[cc.c + 16]);
		}
		if (cc.clamp)
		{
			out.Write(",0.0,1.0));\n");
		}
		else
		{
			out.Write(", -" C_1024_BY_255 ", " C_1023_BY_255 "));\n");
		}
		RegisterStates[ac.dest].AlphaNeedOverflowControl = (ac.clamp == 0);
		RegisterStates[ac.dest].AuxStored = false;

		out.Write("// alpha combine\n");
		out.Write("%s = F_P_U8(clamp(", tevAOutputTable[ac.dest]);	

		if (ac.bias != TevBias_COMPARE) // if not compare
		{
			//normal alpha combiner goes here
			if (ac.shift > TEVSCALE_1)
				out.Write("%s*(", tevScaleTable[ac.shift]);

			if(!(ac.d == TEVALPHAARG_ZERO && ac.op == TEVOP_ADD))
				out.Write("%s.a%s", tevAInputTable[ac.d], tevOpTable[ac.op]);

			if (ac.a == ac.b)
				out.Write("%s.a", tevAInputTable[ac.a + 8]);
			else if (ac.c == TEVALPHAARG_ZERO)
				out.Write("%s.a", tevAInputTable[ac.a + 8]);
			else if (ac.a == TEVALPHAARG_ZERO)
				out.Write("%s.a*%s.a", tevAInputTable[ac.b + 8], tevAInputTable[ac.c + 8]);
			else if (ac.b == TEVALPHAARG_ZERO)
				out.Write("%s.a*(1.0-%s.a)", tevAInputTable[ac.a + 8], tevAInputTable[ac.c + 8]);
			else
				out.Write("lerp(%s.a, %s.a, %s.a)", tevAInputTable[ac.a + 8], tevAInputTable[ac.b + 8], tevAInputTable[ac.c + 8]);

			out.Write("%s",tevBiasTable[ac.bias]);

			if (ac.shift>0)
				out.Write(")");

		}
		else
		{
			//compare alpha combiner goes here
			int cmp = (ac.shift<<1)|ac.op|8; // comparemode stored here
			out.Write(TEVCMPAlphaOPTable[cmp],
					tevAInputTable[ac.d],
					tevAInputTable[ac.a + 8],
					tevAInputTable[ac.b + 8],
					tevAInputTable[ac.c + 8]);
		}
		if (ac.clamp)
		{
			out.Write(",0.0,1.0));\n\n");
		}
		else
		{
			out.Write(", -" C_1024_BY_255 ", " C_1023_BY_255 "));\n\n");
		}		
		out.Write("// TEV done\n");
	}
}

template<class T, bool Write_Code, API_TYPE ApiType>
void SampleTexture(T& out, const char *texcoords, const char *texswap, int texmap)
{
	out.SetConstantsUsed(C_TEXDIMS+texmap,C_TEXDIMS+texmap);
	if(Write_Code)
	{
		if (ApiType == API_D3D11)
			out.Write("Tex%d.Sample(samp%d,%s.xy * " I_TEXDIMS"[%d].xy).%s;\n", texmap,texmap, texcoords, texmap, texswap);
		else
			out.Write("%s(samp%d,%s.xy * " I_TEXDIMS"[%d].xy).%s;\n", ApiType == API_OPENGL ? "texture" : "tex2D", texmap, texcoords, texmap, texswap);
	}
}

static const char *tevAlphaFuncsTable[] =
{
	"(false)",									// NEVER
	"(prev.a <= %s - (0.25/255.0))",			// LESS
	"(abs( prev.a - %s ) < (0.5/255.0))",		// EQUAL
	"(prev.a < %s + (0.25/255.0))",			// LEQUAL
	"(prev.a >= %s + (0.25/255.0))",			// GREATER
	"(abs( prev.a - %s ) >= (0.5/255.0))",	// NEQUAL
	"(prev.a > %s - (0.25/255.0))",			// GEQUAL
	"(true)"									// ALWAYS
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

	out.SetConstantsUsed(C_ALPHA, C_ALPHA);

	uid_data.alpha_test_comp0 = bpmem.alpha_test.comp0;
	uid_data.alpha_test_comp1 = bpmem.alpha_test.comp1;
	uid_data.alpha_test_logic = bpmem.alpha_test.logic;
	uid_data.alpha_test_use_zcomploc_hack = bpmem.UseEarlyDepthTest() && bpmem.zmode.updateenable && !g_ActiveConfig.backend_info.bSupportsEarlyZ;
	if (Write_Code)
	{
		// using discard then return works the same in cg and dx9 but not in dx11
		out.Write("\tif(!( ");

		// Lookup the first component from the alpha function table
		int compindex = bpmem.alpha_test.comp0;
		out.Write(tevAlphaFuncsTable[compindex], alphaRef[0]);

		out.Write("%s", tevAlphaFunclogicTable[bpmem.alpha_test.logic]);//lookup the logic op

		// Lookup the second component from the alpha function table
		compindex = bpmem.alpha_test.comp1;
		out.Write(tevAlphaFuncsTable[compindex], alphaRef[1]);
		out.Write(")) {\n");

		out.Write("\t\tocol0 = float4(0.0, 0.0, 0.0, 0.0);\n");
		if (dstAlphaMode == DSTALPHA_DUAL_SOURCE_BLEND)
			out.Write("\t\tocol1 = float4(0.0, 0.0, 0.0, 0.0);\n");
		if(per_pixel_depth)
			out.Write("\t\tdepth = 1.f;\n");

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
			out.Write("\t\tdiscard;\n");
			if (ApiType != API_D3D11)
				out.Write("\t\treturn;\n");
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
	"\tfog = 1.0 - exp2(-8.0 * fog);\n",						// exp
	"\tfog = 1.0 - exp2(-8.0 * fog * fog);\n",				// exp2
	"\tfog = exp2(-8.0 * (1.0 - fog));\n",					// backward exp
	"\tfog = 1.0 - fog;\n   fog = exp2(-8.0 * fog * fog);\n"	// backward exp2
};

template<class T, bool Write_Code>
static inline void WriteFog(T& out, pixel_shader_uid_data& uid_data)
{
	uid_data.fog_fsel = bpmem.fog.c_proj_fsel.fsel;
	if(bpmem.fog.c_proj_fsel.fsel == 0)
		return; // no Fog

	uid_data.fog_proj = bpmem.fog.c_proj_fsel.proj;
	uid_data.fog_RangeBaseEnabled = bpmem.fogRange.Base.Enabled;

	out.SetConstantsUsed(C_FOG, C_FOG+1);
	if (Write_Code)
	{
		if (bpmem.fog.c_proj_fsel.proj == 0)
		{
			// perspective
			// ze = A/(B - (Zs >> B_SHF)
			out.Write("\tfloat ze = " I_FOG "[1].x / (" I_FOG "[1].y - (zCoord / " I_FOG "[1].w));\n");
		}
		else
		{
			// orthographic
			// ze = a*Zs	(here, no B_SHF)
			out.Write("\tfloat ze = " I_FOG"[1].x * zCoord;\n");
		}

		// x_adjust = sqrt((x-center)^2 + k^2)/k
		// ze *= x_adjust
		// this is completely theoretical as the real hardware seems to use a table intead of calculating the values.
	
		if (bpmem.fogRange.Base.Enabled)
		{
			out.SetConstantsUsed(C_FOG+2, C_FOG+2);
			out.Write("\tfloat x_adjust = (2.0 * (clipPos.x / " I_FOG "[2].y)) - 1.0 - " I_FOG "[2].x;\n");
			out.Write("\tx_adjust = sqrt(x_adjust * x_adjust + " I_FOG "[2].z * " I_FOG "[2].z) / " I_FOG "[2].z;\n");
			out.Write("\tze *= x_adjust;\n");
		}

		out.Write("\tfloat fog = clamp(ze - " I_FOG "[1].z, 0.0, 1.0);\n");

		if (bpmem.fog.c_proj_fsel.fsel > 3)
		{
			out.Write("%s", tevFogFuncsTable[bpmem.fog.c_proj_fsel.fsel]);
		}
		else
		{
			if (bpmem.fog.c_proj_fsel.fsel != 2 && out.GetBuffer() != NULL)
				WARN_LOG(VIDEO, "Unknown Fog Type! %08x", bpmem.fog.c_proj_fsel.fsel);
		}

		out.Write("\tprev.rgb = lerp(prev.rgb, " I_FOG "[0].rgb, F_P_U8(fog));\n");
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

void GetPixelShaderConstantProfileD3D9(ShaderConstantProfile& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderConstantProfile, false, API_D3D9>(object, dstAlphaMode, components);
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

void GetPixelShaderConstantProfileD3D11(ShaderConstantProfile& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderConstantProfile, false, API_D3D11>(object, dstAlphaMode, components);
}

void GetPixelShaderUidGL(PixelShaderUid& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<PixelShaderUid, false, API_OPENGL>(object, dstAlphaMode, components);
}

void GeneratePixelShaderCodeGL(ShaderCode& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderCode, true, API_OPENGL>(object, dstAlphaMode, components);
}

void GetPixelShaderConstantProfileGL(ShaderConstantProfile& object, DSTALPHA_MODE dstAlphaMode, u32 components)
{
	GeneratePixelShader<ShaderConstantProfile, false, API_OPENGL>(object, dstAlphaMode, components);
}


