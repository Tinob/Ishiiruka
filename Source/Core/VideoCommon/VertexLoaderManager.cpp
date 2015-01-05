// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino

#include <map>
#include <memory>
#include <unordered_map>


#include "Core/ConfigManager.h"
#include "Common/ThreadPool.h"

#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VideoConfig.h"

// Precompiled Loaders
#include "VideoCommon/G_G4BP08_pvt.h"
#include "VideoCommon/G_GB4P51_pvt.h"
#include "VideoCommon/G_GFZE01_pvt.h"
#include "VideoCommon/G_GLMP01_pvt.h"
#include "VideoCommon/G_GM8E01_pvt.h"
#include "VideoCommon/G_GNUEDA_pvt.h"
#include "VideoCommon/G_GSAE01_pvt.h"
#include "VideoCommon/G_GZ2P01_pvt.h"
#include "VideoCommon/G_RBUP08_pvt.h"
#include "VideoCommon/G_R5WEA4_pvt.h"
#include "VideoCommon/G_RMCP01_pvt.h"
#include "VideoCommon/G_RMGP01_pvt.h"
#include "VideoCommon/G_RSBP01_pvt.h"
#include "VideoCommon/G_SDWP18_pvt.h"
#include "VideoCommon/G_SMNP01_pvt.h"
#include "VideoCommon/G_SPDE52_pvt.h"
#include "VideoCommon/G_SPXP41_pvt.h"
#include "VideoCommon/G_SX4E01_pvt.h"

NativeVertexFormat *g_nativeVertexFmt;
static VertexLoader *s_VertexLoaders[8];
static VertexLoader *s_CPULoaders[8];
static std::string LastGameCode;
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
	static PrecompiledVertexLoaderMap s_PrecompiledVertexLoaderMap;
	static VertexLoaderMap s_VertexLoaderMap;
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
			std::string conf;
			u64 num_verts;
			std::string hash;
			bool operator < (const codeentry &other) const
			{
				return num_verts > other.num_verts;
			}
		};
	}

	std::string To_HexString(u32 in) {
		char hexString[2 * sizeof(u32) + 8];
		sprintf(hexString, "0x%08xu", in);
		return std::string(hexString);
	}

	void DumpLoadersCode()
	{
		std::vector<codeentry> entries;
		for (VertexLoaderMap::const_iterator iter = s_VertexLoaderMap.begin(); iter != s_VertexLoaderMap.end(); ++iter)
		{
			if (!iter->second->IsPrecompiled())
			{
				codeentry e;
				e.conf.append(To_HexString(iter->first.GetElement(0)));
				e.conf.append(", ");
				e.conf.append(To_HexString(iter->first.GetElement(1)));
				e.conf.append(", ");
				e.conf.append(To_HexString(iter->first.GetElement(2)));
				e.conf.append(", ");
				e.conf.append(To_HexString(iter->first.GetElement(3)));				
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
		const char* gamename = LastGameCode.c_str();
		const char* dumpfolder = File::GetUserPath(D_DUMP_IDX).c_str();

		sprintf(filename, "%sG_%s_pvt.h", dumpfolder, gamename);
		std::string header;
		header.append("// Copyright 2013 Dolphin Emulator Project\n");
		header.append("// Licensed under GPLv2\n");
		header.append("// Refer to the license.txt file included.\n");
		header.append("// Added for Ishiiruka by Tino\n");
		header.append("#pragma once\n");
		header.append("#include <map>\n");
		header.append("#include \"VideoCommon/NativeVertexFormat.h\"\n");
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
		sourcecode.append("#include \"VideoCommon/VertexLoader_Template.h\"\n\n");
		sourcecode.append("\n\nvoid G_");
		sourcecode.append(gamename);
		sourcecode.append("_pvt::Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap)\n{\n");
		for (std::vector<codeentry>::const_iterator iter = entries.begin(); iter != entries.end(); ++iter)
		{
			sourcecode.append("\t// ");
			sourcecode.append(iter->name);
			sourcecode.append("\n// num_verts= ");
			sourcecode.append(std::to_string(iter->num_verts));			
			sourcecode.append("#if _M_SSE >= 0x301\n");
			sourcecode.append("\tif (cpu_info.bSSSE3)\n");
			sourcecode.append("\t{\n");
			sourcecode.append("\t\tpvlmap[");
			sourcecode.append(iter->hash);
			sourcecode.append("] = ");
			sourcecode.append("TemplatedLoader");
			sourcecode.append("<0x301, ");
			sourcecode.append(iter->conf);
			sourcecode.append(">;\n");
			sourcecode.append("\t}\n\telse\n");
			sourcecode.append("#endif\n");
			sourcecode.append("\t{\n");
			sourcecode.append("\t\tpvlmap[");
			sourcecode.append(iter->hash);
			sourcecode.append("] = ");
			sourcecode.append("TemplatedLoader");
			sourcecode.append("<0, ");
			sourcecode.append(iter->conf);
			sourcecode.append(">;\n");
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
		for (VertexLoaderMap::const_iterator iter = s_VertexLoaderMap.begin(); iter != s_VertexLoaderMap.end(); ++iter)
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
		MarkAllAttrDirty();
		for (VertexLoader*& vertexLoader : s_VertexLoaders)
			vertexLoader = nullptr;
		RecomputeCachedArraybases();
		if (!s_PrecompiledLoadersInitialized)
		{
			s_PrecompiledLoadersInitialized = true;
			G_G4BP08_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GB4P51_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GFZE01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GLMP01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GM8E01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GNUEDA_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GSAE01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_GZ2P01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_R5WEA4_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_RBUP08_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_RMCP01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_RMGP01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_RSBP01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_SDWP18_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_SMNP01_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_SPDE52_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_SPXP41_pvt::Initialize(s_PrecompiledVertexLoaderMap);
			G_SX4E01_pvt::Initialize(s_PrecompiledVertexLoaderMap);			
		}
		LastGameCode = SConfig::GetInstance().m_LocalCoreStartupParameter.m_strUniqueID;
	}

	void Shutdown()
	{
		if (s_VertexLoaderMap.size() > 0 && g_ActiveConfig.bDumpVertexLoaders)
			DumpLoadersCode();
		for (auto& p : s_VertexLoaderMap)
		{
			delete p.second;
		}
		s_VertexLoaderMap.clear();
		s_native_vertex_map.clear();
	}
	
	inline VertexLoader *GetOrAddLoader(const TVtxDesc &VtxDesc, const VAT &VtxAttr)
	{
		VertexLoaderUID uid(VtxDesc, VtxAttr);
		VertexLoaderMap::iterator iter = s_VertexLoaderMap.find(uid);
		if (iter == s_VertexLoaderMap.end())
		{
			PrecompiledVertexLoaderMap::iterator piter = s_PrecompiledVertexLoaderMap.find(uid.GetHash());
			TCompiledLoaderFunction precompiledfunc = nullptr;
			if (piter != s_PrecompiledVertexLoaderMap.end())
			{
				precompiledfunc = piter->second;
			}
			VertexLoader *loader = new VertexLoader(VtxDesc, VtxAttr, precompiledfunc);
			s_VertexLoaderMap[uid] = loader;
			INCSTAT(stats.numVertexLoaders);
			return loader;
		}
		return iter->second;
	}

	void GetVertexSizeAndComponents(const VertexLoaderParameters &parameters, u32 &vertexsize, u32 &components)
	{
		if (parameters.needloaderrefresh)
		{
			s_CPULoaders[parameters.vtx_attr_group] = GetOrAddLoader(*parameters.VtxDesc, *parameters.VtxAttr);
		}
		vertexsize = s_CPULoaders[parameters.vtx_attr_group]->GetVertexSize();
		components = s_CPULoaders[parameters.vtx_attr_group]->GetNativeVertexFormat()->m_components;
	}

	inline void UpdateLoader(const VertexLoaderParameters &parameters)
	{
		s_VertexLoaders[parameters.vtx_attr_group] = GetOrAddLoader(*parameters.VtxDesc, *parameters.VtxAttr);
	}

	bool ConvertVertices(VertexLoaderParameters &parameters, u32 &readsize, u32 &writesize)
	{
		if (parameters.needloaderrefresh)
		{
			UpdateLoader(parameters);
		}
		auto loader = s_VertexLoaders[parameters.vtx_attr_group];
		readsize = parameters.count * loader->GetVertexSize();
		if (parameters.buf_size < readsize)
			return false;
		if (parameters.skip_draw)
		{
			return true;
		}
		NativeVertexFormat *nativefmt = loader->GetNativeVertexFormat();
		// Flush if our vertex format is different from the currently set.
		if (g_nativeVertexFmt != nullptr && g_nativeVertexFmt != nativefmt)
		{
			VertexManager::Flush();
		}
		VertexManager::PrepareForAdditionalData(parameters.primitive, parameters.count, nativefmt->GetVertexStride());
		parameters.destination = VertexManager::s_pCurBufferPointer;
		writesize = nativefmt->GetVertexStride() * parameters.count;
		g_nativeVertexFmt = nativefmt;
		IndexGenerator::AddIndices(parameters.primitive, loader->RunVertices(parameters));
		ADDSTAT(stats.thisFrame.numPrims, parameters.count);
		INCSTAT(stats.thisFrame.numPrimitiveJoins);
		return true;
	}

	int GetVertexSize(const VertexLoaderParameters &parameters)
	{
		if (parameters.needloaderrefresh)
		{
			UpdateLoader(parameters);
		}
		return s_VertexLoaders[parameters.vtx_attr_group]->GetVertexSize();
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