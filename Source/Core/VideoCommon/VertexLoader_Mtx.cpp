// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added for Ishiiruka by Tino
#include "VideoCommon/VertexLoader_Mtx.h"

void LOADERDECL Vertexloader_Mtx::PosMtx_ReadDirect_UByte()
{
	g_PipelineState.curposmtx = g_PipelineState.Read<u8>() & 0x3f;
}

void LOADERDECL Vertexloader_Mtx::PosMtx_Write()
{
	g_PipelineState.Write<u32>(g_PipelineState.curposmtx);
}

void LOADERDECL Vertexloader_Mtx::PosMtxDisabled_Write()
{
	g_PipelineState.Write<u32>(0);
}

void LOADERDECL Vertexloader_Mtx::TexMtx_ReadDirect_UByte()
{
	g_PipelineState.curtexmtx[g_PipelineState.texmtxread++] = g_PipelineState.Read<u8>() & 0x3f;
}

void LOADERDECL Vertexloader_Mtx::TexMtx_Write_Float()
{
	g_PipelineState.Write(float(g_PipelineState.curtexmtx[g_PipelineState.texmtxwrite++]));
}

void LOADERDECL Vertexloader_Mtx::TexMtx_Write_Float2()
{
	g_PipelineState.Write(0.f);
	g_PipelineState.Write(float(g_PipelineState.curtexmtx[g_PipelineState.texmtxwrite++]));
}

void LOADERDECL Vertexloader_Mtx::TexMtx_Write_Float4()
{
	g_PipelineState.Write(0.f);
	g_PipelineState.Write(0.f);
	g_PipelineState.Write(float(g_PipelineState.curtexmtx[g_PipelineState.texmtxwrite++]));
	// Just to fill out with 0.
	g_PipelineState.Write(0.f);
}

void Vertexloader_Mtx::PosMtx_ReadDirect_UByteSTR(std::string *dest)
{
	dest->append("\tpipelinestate.curposmtx = pipelinestate.Read<u8>() & 0x3f;\n");
}

void Vertexloader_Mtx::PosMtx_WriteSTR(std::string *dest)
{
	dest->append("\tpipelinestate.Write<u32>(pipelinestate.curposmtx);\n");
}

void Vertexloader_Mtx::PosMtxDisabled_WriteSTR(std::string *dest)
{
	dest->append("\tpipelinestate.Write<u32>(0);\n");
}

void Vertexloader_Mtx::TexMtx_ReadDirect_UByteSTR(std::string *dest)
{
	dest->append("\tpipelinestate.curtexmtx[pipelinestate.texmtxread] = pipelinestate.Read<u8>() & 0x3f;\n"
		"\tpipelinestate.texmtxread++;\n");
}

void Vertexloader_Mtx::TexMtx_Write_FloatSTR(std::string *dest)
{
	dest->append("\tpipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));\n");
}

void Vertexloader_Mtx::TexMtx_Write_Float2STR(std::string *dest)
{
	dest->append("\tpipelinestate.Write(0.f);\n"
		"\tpipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++]));\n");
}

void Vertexloader_Mtx::TexMtx_Write_Float4STR(std::string *dest)
{
	dest->append("\tpipelinestate.Write(0.f);\n"
		"\tpipelinestate.Write(0.f); \n"
		"\tpipelinestate.Write(float(pipelinestate.curtexmtx[pipelinestate.texmtxwrite++])); \n"
		"\tpipelinestate.Write(0.f); \n");
}