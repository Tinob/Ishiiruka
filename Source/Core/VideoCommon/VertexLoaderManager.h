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
	void Init();
	void Shutdown();

	void MarkAllDirty();

	int GetVertexSize(int vtx_attr_group);
	bool RunVertices(int vtx_attr_group, int primitive, int count, size_t buf_size);

	// For debugging
	void AppendListToString(std::string *dest);

	NativeVertexFormat* GetNativeVertexFormat(const PortableVertexDeclaration& format, u32 components);
};

void RecomputeCachedArraybases();