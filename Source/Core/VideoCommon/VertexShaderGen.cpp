// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <cmath>
#include <locale.h>
#ifdef __APPLE__
	#include <xlocale.h>
#endif

#include "VideoCommon/NativeVertexFormat.h"

#include "VideoCommon/BPMemory.h"
#include "VideoCommon/CPMemory.h"
#include "VideoCommon/DriverDetails.h"
#include "VideoCommon/LightingShaderGen.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/VideoConfig.h"

static char text[16768];
static const char *texOffsetMemberSelector[]   = {"x", "y", "z", "w"};
template<class T, API_TYPE api_type>
void DefineVSOutputStructMember(T& object, const char* type, const char* name, int var_index, const char* semantic, int semantic_index = -1)
{
	object.Write("  %s %s", type, name);
	if (var_index != -1)
		object.Write("%d", var_index);

	if (api_type == API_OPENGL)
		object.Write(";\n");
	else
	{
		if (semantic_index != -1)
			object.Write(" : %s%d;\n", semantic, semantic_index);
		else
			object.Write(" : %s;\n", semantic);
	}
}

template<class T, API_TYPE api_type>
inline void GenerateVSOutputStruct(T& object)
{
	object.Write("struct VS_OUTPUT {\n");
	DefineVSOutputStructMember<T, api_type>(object, "float4", "pos", -1, "POSITION");
	DefineVSOutputStructMember<T, api_type>(object, "float4", "colors_", 0, "COLOR", 0);
	DefineVSOutputStructMember<T, api_type>(object, "float4", "colors_", 1, "COLOR", 1);

	if (xfregs.numTexGen.numTexGens < 7)
	{
		for (unsigned int i = 0; i < xfregs.numTexGen.numTexGens; ++i)
			DefineVSOutputStructMember<T, api_type>(object, "float3", "tex", i, "TEXCOORD", i);

		DefineVSOutputStructMember<T, api_type>(object, "float4", "clipPos", -1, "TEXCOORD", xfregs.numTexGen.numTexGens);

		if(g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
			DefineVSOutputStructMember<T, api_type>(object, "float4", "Normal", -1, "TEXCOORD", xfregs.numTexGen.numTexGens + 1);
	}
	else
	{
		// Store clip position in the w component of first 4 texcoords
		bool ppl = g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting;
		int num_texcoords = ppl ? 8 : xfregs.numTexGen.numTexGens;
		for (int i = 0; i < num_texcoords; ++i)
			DefineVSOutputStructMember<T, api_type>(object, (ppl || i < 4) ? "float4" : "float3", "tex", i, "TEXCOORD", i);
	}
	object.Write("};\n");
}

template<class T, bool Write_Code, API_TYPE api_type>
inline void GenerateVertexShader(T& out, u32 components)
{
	// Non-uid template parameters will write to the dummy data (=> gets optimized out)
	vertex_shader_uid_data dummy_data;
	vertex_shader_uid_data& uid_data = (&out.template GetUidData<vertex_shader_uid_data>() != NULL)
											? out.template GetUidData<vertex_shader_uid_data>() : dummy_data;
	#ifndef ANDROID
	locale_t locale;
	locale_t old_locale;
	#endif
	_assert_(bpmem.genMode.numtexgens == xfregs.numTexGen.numTexGens);
	_assert_(bpmem.genMode.numcolchans == xfregs.numChan.numColorChans);
	uid_data.numTexGens = xfregs.numTexGen.numTexGens;
	uid_data.components = components;
	uid_data.pixel_lighting = (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting);
	uid_data.numColorChans = xfregs.numChan.numColorChans;

	if (Write_Code)
	{
		out.SetBuffer(text);
	#ifndef ANDROID	
		locale = newlocale(LC_NUMERIC_MASK, "C", NULL); // New locale for compilation
		old_locale = uselocale(locale); // Apply the locale for this thread		
	#endif
		text[sizeof(text) - 1] = 0x7C;  // canary
		// uniforms
		if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
			out.Write("layout(std140) uniform VSBlock {\n");

		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_POSNORMALMATRIX, "float4", I_POSNORMALMATRIX"[6]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_PROJECTION, "float4", I_PROJECTION"[4]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_MATERIALS, "float4", I_MATERIALS"[4]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_LIGHTS, "float4", I_LIGHTS"[40]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_TEXMATRICES, "float4", I_TEXMATRICES"[24]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_TRANSFORMMATRICES, "float4", I_TRANSFORMMATRICES"[64]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_NORMALMATRICES, "float4", I_NORMALMATRICES"[32]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_POSTTRANSFORMMATRICES, "float4", I_POSTTRANSFORMMATRICES"[64]");
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_DEPTHPARAMS, "float4", I_DEPTHPARAMS);
		DeclareUniform<T, api_type>(out, g_ActiveConfig.backend_info.bSupportsGLSLUBO, C_PLOFFSETPARAMS, "float4", I_PLOFFSETPARAMS"[13]");

		if (g_ActiveConfig.backend_info.bSupportsGLSLUBO)
			out.Write("};\n");

		GenerateVSOutputStruct<T, api_type>(out);
		if(api_type == API_OPENGL)
		{
			out.Write("ATTRIN float4 rawpos; // ATTR%d,\n", SHADER_POSITION_ATTRIB);
			out.Write("ATTRIN float fposmtx; // ATTR%d,\n", SHADER_POSMTX_ATTRIB);
			if (components & VB_HAS_NRM0)
				out.Write("ATTRIN float3 rawnorm0; // ATTR%d,\n", SHADER_NORM0_ATTRIB);
			if (components & VB_HAS_NRM1)
				out.Write("ATTRIN float3 rawnorm1; // ATTR%d,\n", SHADER_NORM1_ATTRIB);
			if (components & VB_HAS_NRM2)
				out.Write("ATTRIN float3 rawnorm2; // ATTR%d,\n", SHADER_NORM2_ATTRIB);

			if (components & VB_HAS_COL0)
				out.Write("ATTRIN float4 color0; // ATTR%d,\n", SHADER_COLOR0_ATTRIB);
			if (components & VB_HAS_COL1)
				out.Write("ATTRIN float4 color1; // ATTR%d,\n", SHADER_COLOR1_ATTRIB);

			for (int i = 0; i < 8; ++i)
			{
				u32 hastexmtx = (components & (VB_HAS_TEXMTXIDX0<<i));
				if ((components & (VB_HAS_UV0<<i)) || hastexmtx)
					out.Write("ATTRIN float%d tex%d; // ATTR%d,\n", hastexmtx ? 3 : 2, i, SHADER_TEXTURE0_ATTRIB + i);
			}

			// Let's set up attributes
			if (xfregs.numTexGen.numTexGens < 7)
			{
				for (int i = 0; i < 8; ++i)
					out.Write("VARYOUT  float3 uv%d_2;\n", i);
				out.Write("VARYOUT   float4 clipPos_2;\n");
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
					out.Write("VARYOUT   float4 Normal_2;\n");
			}
			else
			{
				// wpos is in w of first 4 texcoords
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					for (int i = 0; i < 8; ++i)
						out.Write("VARYOUT   float4 uv%d_2;\n", i);
				}
				else
				{
					for (unsigned int i = 0; i < xfregs.numTexGen.numTexGens; ++i)
						out.Write("VARYOUT   float%d uv%d_2;\n", i < 4 ? 4 : 3 , i);
				}
			}
			out.Write("VARYOUT   float4 colors_02;\n");
			out.Write("VARYOUT   float4 colors_12;\n");

			out.Write("void main()\n{\n");
		}
		else
		{
			out.Write("VS_OUTPUT main(\n");

			// inputs
			if (components & VB_HAS_NRM0)
				out.Write("  float3 rawnorm0 : NORMAL0,\n");
			if (components & VB_HAS_NRM1)
				out.Write("  float3 rawnorm1 : NORMAL1,\n");
			if (components & VB_HAS_NRM2)
				out.Write("  float3 rawnorm2 : NORMAL2,\n");
			if (components & VB_HAS_COL0)
				out.Write("  float4 color0 : COLOR0,\n");
			if (components & VB_HAS_COL1)
				out.Write("  float4 color1 : COLOR1,\n");
			for (int i = 0; i < 8; ++i)
			{
				u32 hastexmtx = (components & (VB_HAS_TEXMTXIDX0<<i));
				if ((components & (VB_HAS_UV0<<i)) || hastexmtx)
					out.Write("  float%d tex%d : TEXCOORD%d,\n", hastexmtx ? 3 : 2, i, i);
			}
			out.Write("  float4 blend_indices : BLENDINDICES,\n");
			out.Write("  float4 rawpos : POSITION) {\n");
		}
		out.Write("VS_OUTPUT o;\n");
		if (api_type & API_D3D9)
		{
			out.Write("int4 indices = D3DCOLORtoUBYTE4(blend_indices);\n");		
		}
		// transforms
		if (components & VB_HAS_POSMTXIDX)
		{
			if (api_type & API_D3D9)
			{
				out.Write("int posmtx = indices.x;\n");
			}
			else if (api_type == API_D3D11)
			{
				out.Write("int posmtx = blend_indices.x * 255.0;\n");
			}
			else
			{
				out.Write("int posmtx = int(fposmtx);\n");
			}

			if ((DriverDetails::HasBug(DriverDetails::BUG_NODYNUBOACCESS) && !DriverDetails::HasBug(DriverDetails::BUG_ANNIHILATEDUBOS)) )
			{
				// This'll cause issues, but  it can't be helped
				out.Write("float4 pos = float4(dot(" I_TRANSFORMMATRICES"[0], rawpos), dot(" I_TRANSFORMMATRICES"[1], rawpos), dot(" I_TRANSFORMMATRICES"[2], rawpos), 1);\n");
				if (components & VB_HAS_NRMALL)
					out.Write("float3 N0 = " I_NORMALMATRICES"[0].xyz, N1 = " I_NORMALMATRICES"[1].xyz, N2 = " I_NORMALMATRICES"[2].xyz;\n");
			}
			else
			{
				out.Write("float4 pos = float4(dot(" I_TRANSFORMMATRICES"[posmtx], rawpos), dot(" I_TRANSFORMMATRICES"[posmtx+1], rawpos), dot(" I_TRANSFORMMATRICES"[posmtx+2], rawpos), 1);\n");

				if (components & VB_HAS_NRMALL) {
					out.Write("int normidx = posmtx >= 32 ? (posmtx-32) : posmtx;\n");
					out.Write("float3 N0 = " I_NORMALMATRICES"[normidx].xyz, N1 = " I_NORMALMATRICES"[normidx+1].xyz, N2 = " I_NORMALMATRICES"[normidx+2].xyz;\n");
				}
			}
			if (components & VB_HAS_NRM0)
				out.Write("float3 _norm0 = normalize(float3(dot(N0, rawnorm0), dot(N1, rawnorm0), dot(N2, rawnorm0)));\n");
			if (components & VB_HAS_NRM1)
				out.Write("float3 _norm1 = float3(dot(N0, rawnorm1), dot(N1, rawnorm1), dot(N2, rawnorm1));\n");
			if (components & VB_HAS_NRM2)
				out.Write("float3 _norm2 = float3(dot(N0, rawnorm2), dot(N1, rawnorm2), dot(N2, rawnorm2));\n");
		}
		else
		{
			out.Write("float4 pos = float4(dot(" I_POSNORMALMATRIX"[0], rawpos), dot(" I_POSNORMALMATRIX"[1], rawpos), dot(" I_POSNORMALMATRIX"[2], rawpos), 1.0);\n");
			if (components & VB_HAS_NRM0)
				out.Write("float3 _norm0 = normalize(float3(dot(" I_POSNORMALMATRIX"[3].xyz, rawnorm0), dot(" I_POSNORMALMATRIX"[4].xyz, rawnorm0), dot(" I_POSNORMALMATRIX"[5].xyz, rawnorm0)));\n");
			if (components & VB_HAS_NRM1)
				out.Write("float3 _norm1 = float3(dot(" I_POSNORMALMATRIX"[3].xyz, rawnorm1), dot(" I_POSNORMALMATRIX"[4].xyz, rawnorm1), dot(" I_POSNORMALMATRIX"[5].xyz, rawnorm1));\n");
			if (components & VB_HAS_NRM2)
				out.Write("float3 _norm2 = float3(dot(" I_POSNORMALMATRIX"[3].xyz, rawnorm2), dot(" I_POSNORMALMATRIX"[4].xyz, rawnorm2), dot(" I_POSNORMALMATRIX"[5].xyz, rawnorm2));\n");
		}

		if (!(components & VB_HAS_NRM0))
			out.Write("float3 _norm0 = float3(0.0, 0.0, 0.0);\n");


		out.Write("o.pos = float4(dot(" I_PROJECTION"[0], pos), dot(" I_PROJECTION"[1], pos), dot(" I_PROJECTION"[2], pos), dot(" I_PROJECTION"[3], pos));\n");
		if (api_type & API_D3D9)
		{
			//Write Pos offset for Point/Line Rendering
			out.Write("o.pos.xy = o.pos.xy + " I_PLOFFSETPARAMS"[indices.z].xy * o.pos.w;\n");
		}
		out.Write("float4 mat, lacc;\n"
				"float3 ldir, h;\n"
				"float dist, dist2, attn;\n");
		if (xfregs.numChan.numColorChans == 0)
		{
			if (components & VB_HAS_COL0)
				out.Write("o.colors_0 = color0;\n");
			else
				out.Write("o.colors_0 = float4(1.0, 1.0, 1.0, 1.0);\n");
		}
	}

	GenerateLightingShader<T, Write_Code>(out, uid_data.lighting, components, I_MATERIALS, I_LIGHTS, "color", "o.colors_");

	// special case if only pos and tex coord 0 and tex coord input is AB11
	// donko - this has caused problems in some games. removed for now.
	bool texGenSpecialCase = false;
	/*bool texGenSpecialCase =
		((g_VtxDesc.Hex & 0x60600L) == g_VtxDesc.Hex) && // only pos and tex coord 0
		(g_VtxDesc.Tex0Coord != NOT_PRESENT) &&
		(xfregs.texcoords[0].texmtxinfo.inputform == XF_TEXINPUT_AB11);
		*/
	if(Write_Code)
	{
		if (xfregs.numChan.numColorChans < 2)
		{
			if (components & VB_HAS_COL1)
				out.Write("o.colors_1 = color1;\n");
			else
				out.Write("o.colors_1 = o.colors_0;\n");
		}
		// transform texcoords
		out.Write("float4 coord = float4(0.0, 0.0, 1.0, 1.0);\n");
	}

	
	for (unsigned int i = 0; i < xfregs.numTexGen.numTexGens; ++i)
	{
		TexMtxInfo& texinfo = xfregs.texMtxInfo[i];
		uid_data.texMtxInfo[i].sourcerow = xfregs.texMtxInfo[i].sourcerow;
		if (Write_Code)
		{
			out.Write("{\n");
			out.Write("coord = float4(0.0, 0.0, 1.0, 1.0);\n");		
			switch (texinfo.sourcerow)
			{
			case XF_SRCGEOM_INROW:
				_assert_( texinfo.inputform == XF_TEXINPUT_ABC1 );
				out.Write("coord = rawpos;\n"); // pos.w is 1
				break;
			case XF_SRCNORMAL_INROW:
				if (components & VB_HAS_NRM0)
				{
					_assert_( texinfo.inputform == XF_TEXINPUT_ABC1 );
					out.Write("coord = float4(rawnorm0.xyz, 1.0);\n");
				}
				break;
			case XF_SRCCOLORS_INROW:
				_assert_( texinfo.texgentype == XF_TEXGEN_COLOR_STRGBC0 || texinfo.texgentype == XF_TEXGEN_COLOR_STRGBC1 );
				break;
			case XF_SRCBINORMAL_T_INROW:
				if (components & VB_HAS_NRM1)
				{
					_assert_( texinfo.inputform == XF_TEXINPUT_ABC1 );
					out.Write("coord = float4(rawnorm1.xyz, 1.0);\n");
				}
				break;
			case XF_SRCBINORMAL_B_INROW:
				if (components & VB_HAS_NRM2)
				{
					_assert_( texinfo.inputform == XF_TEXINPUT_ABC1 );
					out.Write("coord = float4(rawnorm2.xyz, 1.0);\n");
				}
				break;
			default:
				_assert_(texinfo.sourcerow <= XF_SRCTEX7_INROW);
				if (components & (VB_HAS_UV0<<(texinfo.sourcerow - XF_SRCTEX0_INROW)) )
					out.Write("coord = float4(tex%d.x, tex%d.y, 1.0, 1.0);\n", texinfo.sourcerow - XF_SRCTEX0_INROW, texinfo.sourcerow - XF_SRCTEX0_INROW);
				break;
			}
		}		

		// first transformation
		uid_data.texMtxInfo[i].texgentype = xfregs.texMtxInfo[i].texgentype;
		switch (texinfo.texgentype)
		{
			case XF_TEXGEN_EMBOSS_MAP: // calculate tex coords into bump map

				if (components & (VB_HAS_NRM1|VB_HAS_NRM2))
				{
					// transform the light dir into tangent space
					uid_data.texMtxInfo[i].embosslightshift = xfregs.texMtxInfo[i].embosslightshift;
					uid_data.texMtxInfo[i].embosssourceshift = xfregs.texMtxInfo[i].embosssourceshift;
					if (Write_Code)
					{
						out.Write("ldir = normalize(" LIGHT_POS".xyz - pos.xyz);\n", LIGHT_POS_PARAMS(I_LIGHTS, texinfo.embosslightshift));
						out.Write("o.tex%d.xyz = o.tex%d.xyz + float3(dot(ldir, _norm1), dot(ldir, _norm2), 0.0);\n", i, texinfo.embosssourceshift);
					}					
				}
				else
				{
					_assert_(0); // should have normals
					uid_data.texMtxInfo[i].embosssourceshift = xfregs.texMtxInfo[i].embosssourceshift;
					if (Write_Code)
					{
						out.Write("o.tex%d.xyz = o.tex%d.xyz;\n", i, texinfo.embosssourceshift);
					}
				}

				break;
			case XF_TEXGEN_COLOR_STRGBC0:
				_assert_(texinfo.sourcerow == XF_SRCCOLORS_INROW);
				if (Write_Code)
					out.Write("o.tex%d.xyz = float3(o.colors_0.x, o.colors_0.y, 1);\n", i);
				break;
			case XF_TEXGEN_COLOR_STRGBC1:
				_assert_(texinfo.sourcerow == XF_SRCCOLORS_INROW);
				if (Write_Code)
					out.Write("o.tex%d.xyz = float3(o.colors_1.x, o.colors_1.y, 1);\n", i);
				break;
			case XF_TEXGEN_REGULAR:
			default:
				uid_data.texMtxInfo_n_projection |= xfregs.texMtxInfo[i].projection << i;
				if (Write_Code)
				{
					if (components & (VB_HAS_TEXMTXIDX0<<i))
					{
						out.Write("int tmp = int(tex%d.z);\n", i);
						if (texinfo.projection == XF_TEXPROJ_STQ)
							out.Write("o.tex%d.xyz = float3(dot(coord, " I_TRANSFORMMATRICES"[tmp]), dot(coord, " I_TRANSFORMMATRICES"[tmp+1]), dot(coord, " I_TRANSFORMMATRICES"[tmp+2]));\n", i);
						else
							out.Write("o.tex%d.xyz = float3(dot(coord, " I_TRANSFORMMATRICES"[tmp]), dot(coord, " I_TRANSFORMMATRICES"[tmp+1]), 1);\n", i);
					}
					else
					{
						if (texinfo.projection == XF_TEXPROJ_STQ)
							out.Write("o.tex%d.xyz = float3(dot(coord, " I_TEXMATRICES"[%d]), dot(coord, " I_TEXMATRICES"[%d]), dot(coord, " I_TEXMATRICES"[%d]));\n", i, 3*i, 3*i+1, 3*i+2);
						else
							out.Write("o.tex%d.xyz = float3(dot(coord, " I_TEXMATRICES"[%d]), dot(coord, " I_TEXMATRICES"[%d]), 1);\n", i, 3*i, 3*i+1);
					}
				}
				break;
		}

		uid_data.dualTexTrans_enabled = xfregs.dualTexTrans.enabled;
		// CHECKME: does this only work for regular tex gen types?
		if (xfregs.dualTexTrans.enabled && texinfo.texgentype == XF_TEXGEN_REGULAR)
		{
			const PostMtxInfo& postInfo = xfregs.postMtxInfo[i];

			uid_data.postMtxInfo[i].index = xfregs.postMtxInfo[i].index;
			int postidx = postInfo.index;
			if (Write_Code)
			{
				out.Write("float4 P0 = " I_POSTTRANSFORMMATRICES"[%d];\n"
					"float4 P1 = " I_POSTTRANSFORMMATRICES"[%d];\n"
					"float4 P2 = " I_POSTTRANSFORMMATRICES"[%d];\n",
					postidx&0x3f, (postidx+1)&0x3f, (postidx+2)&0x3f);
			}
			if (texGenSpecialCase)
			{
				// no normalization
				// q of input is 1
				// q of output is unknown

				// multiply by postmatrix
				if (Write_Code)
					out.Write("o.tex%d.xyz = float3(dot(P0.xy, o.tex%d.xy) + P0.z + P0.w, dot(P1.xy, o.tex%d.xy) + P1.z + P1.w, 0.0);\n", i, i, i);
			}
			else
			{
				uid_data.postMtxInfo[i].normalize = xfregs.postMtxInfo[i].normalize;
				if (Write_Code)
				{
					if (postInfo.normalize)
						out.Write("o.tex%d.xyz = normalize(o.tex%d.xyz);\n", i, i);

					// multiply by postmatrix
					out.Write("o.tex%d.xyz = float3(dot(P0.xyz, o.tex%d.xyz) + P0.w, dot(P1.xyz, o.tex%d.xyz) + P1.w, dot(P2.xyz, o.tex%d.xyz) + P2.w);\n", i, i, i, i);
				}
			}
		}
		if (Write_Code)
			out.Write("}\n");
	}
	if (Write_Code)
	{
		// clipPos/w needs to be done in pixel shader, not here
		if (xfregs.numTexGen.numTexGens < 7)
		{
			out.Write("o.clipPos = float4(pos.x,pos.y,o.pos.z,o.pos.w);\n");
		}
		else
		{
			out.Write("o.tex0.w = pos.x;\n");
			out.Write("o.tex1.w = pos.y;\n");
			out.Write("o.tex2.w = o.pos.z;\n");
			out.Write("o.tex3.w = o.pos.w;\n");
		}

		if(g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
		{
			if (xfregs.numTexGen.numTexGens < 7)
			{
				out.Write("o.Normal = float4(_norm0.x,_norm0.y,_norm0.z,pos.z);\n");
			}
			else
			{
				out.Write("o.tex4.w = _norm0.x;\n");
				out.Write("o.tex5.w = _norm0.y;\n");
				out.Write("o.tex6.w = _norm0.z;\n");
				if (xfregs.numTexGen.numTexGens < 8)
					out.Write("o.tex7 = pos.xyzz;\n");
				else
					out.Write("o.tex7.w = pos.z;\n");
			}

			if (components & VB_HAS_COL0)
				out.Write("o.colors_0 = color0;\n");

			if (components & VB_HAS_COL1)
				out.Write("o.colors_1 = color1;\n");
		}

		//write the true depth value, if the game uses depth textures pixel shaders will override with the correct values
		//if not early z culling will improve speed
		if (api_type & API_D3D9 || api_type == API_D3D11)
		{
			out.Write("o.pos.z = " I_DEPTHPARAMS".x * o.pos.w + o.pos.z * " I_DEPTHPARAMS".y;\n");
		}
		else
		{
			// this results in a scale from -1..0 to -1..1 after perspective
			// divide
			out.Write("o.pos.z = o.pos.w + o.pos.z * 2.0;\n");

			// the next steps of the OGL pipeline are:
			// (x_c,y_c,z_c,w_c) = o.pos  //switch to OGL spec terminology
			// clipping to -w_c <= (x_c,y_c,z_c) <= w_c
			// (x_d,y_d,z_d) = (x_c,y_c,z_c)/w_c//perspective divide
			// z_w = (f-n)/2*z_d + (n+f)/2
			// z_w now contains the value to go to the 0..1 depth buffer

			//trying to get the correct semantic while not using glDepthRange
			//seems to get rather complicated
		}
		
		// The console GPU places the pixel center at 7/12 in screen space unless
		// antialiasing is enabled, while D3D11 and OpenGL place it at 0.5, and D3D9 at 0. This results
		// in some primitives being placed one pixel too far to the bottom-right,
		// which in turn can be critical if it happens for clear quads.
		// Hence, we compensate for this pixel center difference so that primitives
		// get rasterized correctly.
		out.Write("o.pos.xy = o.pos.xy + " I_DEPTHPARAMS".zw;\n");

		if (api_type & API_D3D9)
		{
			// Write Texture Offsets for Point/Line Rendering
			for (unsigned int i = 0; i < xfregs.numTexGen.numTexGens; ++i)
			{
				out.Write("o.tex%d.xy = o.tex%d.xy + (" I_PLOFFSETPARAMS"[indices.w].zw * " I_PLOFFSETPARAMS"[indices.y + %d].%s );\n", i, i, ((i / 4) + 1), texOffsetMemberSelector[i % 4]);
			}
		}

		if(api_type == API_OPENGL)
		{
			// Bit ugly here
			// TODO: Make pretty
			// Will look better when we bind uniforms in GLSL 1.3
			// clipPos/w needs to be done in pixel shader, not here

			if (xfregs.numTexGen.numTexGens < 7)
			{
				for (unsigned int i = 0; i < 8; ++i)
				{
					if(i < xfregs.numTexGen.numTexGens)
						out.Write(" uv%d_2.xyz =  o.tex%d;\n", i, i);
					else
						out.Write(" uv%d_2.xyz =  float3(0.0, 0.0, 0.0);\n", i);
				}
				out.Write("  clipPos_2 = o.clipPos;\n");
				if(g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
					out.Write("  Normal_2 = o.Normal;\n");
			}
			else
			{
				// clip position is in w of first 4 texcoords
				if (g_ActiveConfig.bEnablePixelLighting && g_ActiveConfig.backend_info.bSupportsPixelLighting)
				{
					for (int i = 0; i < 8; ++i)
						out.Write(" uv%d_2 = o.tex%d;\n", i, i);
				}
				else
				{
					for (unsigned int i = 0; i < xfregs.numTexGen.numTexGens; ++i)
						out.Write("  uv%d_2%s = o.tex%d;\n", i, i < 4 ? ".xyzw" : ".xyz" , i);
				}
			}
			out.Write("colors_02 = o.colors_0;\n");
			out.Write("colors_12 = o.colors_1;\n");
			out.Write("gl_Position = o.pos;\n");
			out.Write("}\n");
		}
		else
		{
			out.Write("return o;\n}\n");
		}

		if (text[sizeof(text) - 1] != 0x7C)
			PanicAlert("VertexShader generator - buffer too small, canary has been eaten!");

		#ifndef ANDROID
		uselocale(old_locale); // restore locale
		freelocale(locale);
		#endif		
	}
}

void GetVertexShaderUidD3D9(VertexShaderUid& object, u32 components)
{
	GenerateVertexShader<VertexShaderUid, false, API_D3D9>(object, components);
}

void GenerateVertexShaderCodeD3D9(ShaderCode& object, u32 components)
{
	GenerateVertexShader<ShaderCode, true, API_D3D9>(object, components);
}

void GenerateVSOutputStructForGSD3D9(ShaderCode& object)
{
	GenerateVSOutputStruct<ShaderCode, API_D3D9>(object);
}

void GetVertexShaderUidD3D11(VertexShaderUid& object, u32 components)
{
	GenerateVertexShader<VertexShaderUid, false, API_D3D11>(object, components);
}

void GenerateVertexShaderCodeD3D11(ShaderCode& object, u32 components)
{
	GenerateVertexShader<ShaderCode, true, API_D3D11>(object, components);
}

void GenerateVSOutputStructForGSD3D11(ShaderCode& object)
{
	GenerateVSOutputStruct<ShaderCode, API_D3D11>(object);
}

void GetVertexShaderUidGL(VertexShaderUid& object, u32 components)
{
	GenerateVertexShader<VertexShaderUid, false, API_OPENGL>(object, components);
}

void GenerateVertexShaderCodeGL(ShaderCode& object, u32 components)
{
	GenerateVertexShader<ShaderCode, true, API_OPENGL>(object, components);
}

void GenerateVSOutputStructForGSGL(ShaderCode& object)
{
	GenerateVSOutputStruct<ShaderCode, API_OPENGL>(object);
}
