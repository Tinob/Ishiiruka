// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Core/ConfigManager.h"
#include "Core/HW/Memmap.h"

#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoader.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexShaderManager.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"
// Compiled loaders
#include "VideoCommon/G_RMGP01_pvt.h"
#include "VideoCommon/G_R5WEA4_pvt.h"
#include "VideoCommon/G_GZ2P01_pvt.h"
#include "VideoCommon/G_GMSE01_pvt.h"

static int s_attr_dirty;  // bitfield

static VertexLoader *g_VertexLoaders[8];

namespace std
{

	template <>
	struct hash<VertexLoaderUID>
	{
		size_t operator()(const VertexLoaderUID& uid) const
		{
			return uid.GetplatformHash();
		}
	};

}

typedef std::map<u64, TCompiledLoaderFunction> PrecompiledVertexLoaderMap;
typedef std::unordered_map<VertexLoaderUID, VertexLoader*> VertexLoaderMap;
typedef std::map<PortableVertexDeclaration, std::unique_ptr<NativeVertexFormat>> NativeVertexLoaderMap;

namespace VertexLoaderManager
{
	static bool s_PrecompiledLoadersInitialized = false;
	static PrecompiledVertexLoaderMap g_PrecompiledVertexLoaderMap;
	static VertexLoaderMap g_VertexLoaderMap;
	static NativeVertexLoaderMap s_native_vertex_map;
	// TODO - change into array of pointers. Keep a map of all seen so far.

	namespace
	{
		struct entry
		{
			std::string text;
			u64 num_verts;
			bool operator < (const entry &other) const
			{
				return num_verts > other.num_verts;
			}
		};

		struct codeentry
		{
			std::string name;
			std::string text;
			u64 num_verts;
			std::string hash;
			bool operator < (const codeentry &other) const
			{
				return num_verts > other.num_verts;
			}
		};
	}

	void DumpLoadersCode()
	{
		std::vector<codeentry> entries;
		for (VertexLoaderMap::const_iterator iter = g_VertexLoaderMap.begin(); iter != g_VertexLoaderMap.end(); ++iter)
		{
			if (!iter->second->IsPrecompiled())
			{
				codeentry e;
				iter->second->DumpCode(&e.text);
				iter->second->GetName(&e.name);
				e.num_verts = iter->second->GetNumLoadedVerts();
				e.hash = std::to_string(iter->first.GetHash());
				entries.push_back(e);
			}
		}
		if (entries.size() == 0)
		{
			return;
		}
		char filename[MAX_PATH];
		const char* gamename = SConfig::GetInstance().m_LocalCoreStartupParameter.m_strUniqueID.c_str();
		const char* dumpfolder = File::GetUserPath(D_DUMP_IDX).c_str();

		sprintf(filename, "%sG_%s_pvt.h", dumpfolder, gamename);
		std::string header;
		header.append("#pragma once\n");
		header.append("#include <map>\n");
		header.append("#include \"VideoCommon/VertexLoader.h\"\n");
		header.append("class G_");
		header.append(gamename);
		header.append("_pvt\n{\npublic:\n");
		header.append("static void Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap);\n");
		header.append("};\n");
		std::ofstream headerfile(filename);
		headerfile << header;
		headerfile.close();
		sprintf(filename, "%sG_%s_pvt.cpp", dumpfolder, gamename);
		sort(entries.begin(), entries.end());
		std::string sourcecode;
		sourcecode.append("#include \"VideoCommon/G_");
		sourcecode.append(gamename);
		sourcecode.append("_pvt.h\"\n");
		sourcecode.append("#include \"VideoCommon/VertexLoader_ColorFuncs.h\"\n");
		sourcecode.append("#include \"VideoCommon/VertexLoader_NormalFuncs.h\"\n");
		sourcecode.append("#include \"VideoCommon/VertexLoader_PositionFuncs.h\"\n");
		sourcecode.append("#include \"VideoCommon/VertexLoader_TextCoordFuncs.h\"\n");
		sourcecode.append("#include \"VideoCommon/VertexLoader_BBox.h\"\n");
		sourcecode.append("#include \"VideoCommon/VideoConfig.h\"\n\n");
		for (std::vector<codeentry>::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
		{
			sourcecode.append(iter->text);
		}
		sourcecode.append("\nvoid G_");
		sourcecode.append(gamename);
		sourcecode.append("_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)\n{\n");
		for (std::vector<codeentry>::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
		{
			sourcecode.append("\t// num_verts= ");
			sourcecode.append(std::to_string(iter->num_verts));
			sourcecode.append("\n#if _M_SSE >= 0x401\n");
			sourcecode.append("\tif (cpu_info.bSSE4_1)\n");
			sourcecode.append("\t{\n");
			sourcecode.append("\tpvlmap[");
			sourcecode.append(iter->hash);
			sourcecode.append("] = ");
			sourcecode.append(iter->name);
			sourcecode.append("<0x401>;\n");
			sourcecode.append("\t}\n\telse\n");
			sourcecode.append("#endif\n");
			sourcecode.append("#if _M_SSE >= 0x301\n");
			sourcecode.append("\tif (cpu_info.bSSSE3)\n");
			sourcecode.append("\t{\n");
			sourcecode.append("\tpvlmap[");
			sourcecode.append(iter->hash);
			sourcecode.append("] = ");
			sourcecode.append(iter->name);
			sourcecode.append("<0x301>;\n");
			sourcecode.append("\t}\n\telse\n");
			sourcecode.append("#endif\n");
			sourcecode.append("\t{\n");
			sourcecode.append("\tpvlmap[");
			sourcecode.append(iter->hash);
			sourcecode.append("] = ");
			sourcecode.append(iter->name);
			sourcecode.append("<0>;\n");
			sourcecode.append("\t}\n");
		}
		sourcecode.append("}\n");
		std::ofstream out(filename);
		out << sourcecode;
		out.close();
	}

	void AppendListToString(std::string *dest)
	{
		std::vector<entry> entries;

		size_t total_size = 0;
		for (VertexLoaderMap::const_iterator iter = g_VertexLoaderMap.begin(); iter != g_VertexLoaderMap.end(); ++iter)
		{
			entry e;
			iter->second->AppendToString(&e.text);
			e.num_verts = iter->second->GetNumLoadedVerts();
			entries.push_back(e);
			total_size += e.text.size() + 1;
		}
		sort(entries.begin(), entries.end());
		dest->reserve(dest->size() + total_size);
		for (std::vector<entry>::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
		{
			dest->append(iter->text);
		}
	}

	void Init()
	{
		MarkAllDirty();
		for (VertexLoader*& vertexLoader : g_VertexLoaders)
			vertexLoader = nullptr;
		RecomputeCachedArraybases();
		if (!s_PrecompiledLoadersInitialized)
		{
			G_RMGP01_pvt::Initialize(g_PrecompiledVertexLoaderMap);
			G_R5WEA4_pvt::Initialize(g_PrecompiledVertexLoaderMap);
			G_GZ2P01_pvt::Initialize(g_PrecompiledVertexLoaderMap);
			G_GMSE01_pvt::Initialize(g_PrecompiledVertexLoaderMap);
			s_PrecompiledLoadersInitialized = true;
		}
	}

	void Shutdown()
	{
		if (g_VertexLoaderMap.size() > 0 && g_ActiveConfig.bDumpVertexLoaders)
			DumpLoadersCode();
		for (auto& p : g_VertexLoaderMap)
		{
			delete p.second;
		}
		g_VertexLoaderMap.clear();
		s_native_vertex_map.clear();
	}

	void MarkAllDirty()
	{
		s_attr_dirty = 0xff;
	}

	static VertexLoader* RefreshLoader(int vtx_attr_group)
	{
		if ((s_attr_dirty >> vtx_attr_group) & 1)
		{
			VertexLoaderUID uid;
			uid.InitFromCurrentState(vtx_attr_group);
			VertexLoaderMap::iterator iter = g_VertexLoaderMap.find(uid);
			if (iter != g_VertexLoaderMap.end())
			{
				g_VertexLoaders[vtx_attr_group] = iter->second;
			}
			else
			{
				PrecompiledVertexLoaderMap::iterator piter = g_PrecompiledVertexLoaderMap.find(uid.GetHash());
				TCompiledLoaderFunction precompiledfunc = nullptr;
				if (piter != g_PrecompiledVertexLoaderMap.end())
				{
					precompiledfunc = piter->second;
				}
				VertexLoader *loader = new VertexLoader(g_VtxDesc, g_VtxAttr[vtx_attr_group], precompiledfunc);
				g_VertexLoaderMap[uid] = loader;
				g_VertexLoaders[vtx_attr_group] = loader;
				INCSTAT(stats.numVertexLoaders);
			}
		}
		s_attr_dirty &= ~(1 << vtx_attr_group);
		return g_VertexLoaders[vtx_attr_group];
	}

	void RunVertices(int vtx_attr_group, int primitive, int count)
	{
		if (!count)
			return;

		RefreshLoader(vtx_attr_group)->RunVertices(vtx_attr_group, primitive, count);
	}

	void RunCompiledVertices(int vtx_attr_group, int primitive, int count, u8* Data)
	{
		if (!count || !Data)
			return;
		RefreshLoader(vtx_attr_group)->RunCompiledVertices(vtx_attr_group, primitive, count, Data);
	}

	int GetVertexSize(int vtx_attr_group)
	{
		return RefreshLoader(vtx_attr_group)->GetVertexSize();
	}

	NativeVertexFormat* GetNativeVertexFormat(const PortableVertexDeclaration& format, u32 components)
	{
		auto& native = s_native_vertex_map[format];
		if (!native)
		{
			auto raw_pointer = g_vertex_manager->CreateNativeVertexFormat();
			native = std::unique_ptr<NativeVertexFormat>(raw_pointer);
			native->m_components = components;
			native->Initialize(format);
		}
		return native.get();
	}

}  // namespace

void LoadCPReg(u32 sub_cmd, u32 value)
{
	switch (sub_cmd & 0xF0)
	{
	case 0x30:
		VertexShaderManager::SetTexMatrixChangedA(value);
		break;

	case 0x40:
		VertexShaderManager::SetTexMatrixChangedB(value);
		break;

	case 0x50:
		g_VtxDesc.Hex &= ~0x1FFFF;  // keep the Upper bits
		g_VtxDesc.Hex |= value;
		s_attr_dirty = 0xFF;
		break;

	case 0x60:
		g_VtxDesc.Hex &= 0x1FFFF;  // keep the lower 17Bits
		g_VtxDesc.Hex |= (u64)value << 17;
		s_attr_dirty = 0xFF;
		break;

	case 0x70:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g0.Hex = value;
		s_attr_dirty |= 1 << (sub_cmd & 7);
		break;

	case 0x80:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g1.Hex = value;
		s_attr_dirty |= 1 << (sub_cmd & 7);
		break;

	case 0x90:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g2.Hex = value;
		s_attr_dirty |= 1 << (sub_cmd & 7);
		break;

		// Pointers to vertex arrays in GC RAM
	case 0xA0:
		arraybases[sub_cmd & 0xF] = value;
		cached_arraybases[sub_cmd & 0xF] = Memory::GetPointer(value);
		break;

	case 0xB0:
		arraystrides[sub_cmd & 0xF] = value & 0xFF;
		break;
	}
}

void FillCPMemoryArray(u32 *memory)
{
	memory[0x30] = MatrixIndexA.Hex;
	memory[0x40] = MatrixIndexB.Hex;
	memory[0x50] = (u32)g_VtxDesc.Hex;
	memory[0x60] = (u32)(g_VtxDesc.Hex >> 17);

	for (int i = 0; i < 8; ++i)
	{
		memory[0x70 + i] = g_VtxAttr[i].g0.Hex;
		memory[0x80 + i] = g_VtxAttr[i].g1.Hex;
		memory[0x90 + i] = g_VtxAttr[i].g2.Hex;
	}

	for (int i = 0; i < 16; ++i)
	{
		memory[0xA0 + i] = arraybases[i];
		memory[0xB0 + i] = arraystrides[i];
	}
}

void RecomputeCachedArraybases()
{
	for (int i = 0; i < 16; i++)
	{
		cached_arraybases[i] = Memory::GetPointer(arraybases[i]);
	}
}
