// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino
#pragma once
#include <string>
#include "Common/Common.h"
#include "VideoCommon/NativeVertexFormat.h"

namespace VertexLoaderManager
{
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
	void Init();
	void Shutdown();
	int GetVertexSize(const VertexLoaderParameters &parameters);
	bool ConvertVertices(const VertexLoaderParameters &parameters, u32 &readsize, u32 &writesize);

	// For debugging
	void AppendListToString(std::string *dest);

	NativeVertexFormat* GetNativeVertexFormat(const PortableVertexDeclaration& format, u32 components);
};