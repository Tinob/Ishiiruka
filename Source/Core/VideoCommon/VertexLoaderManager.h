// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Modified for Ishiiruka by Tino
#pragma once
#include <string>
#include <map>
#include "Common/Common.h"
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/VertexLoaderBase.h"

namespace VertexLoaderManager
{
using NativeVertexFormatMap = std::map<PortableVertexDeclaration, std::unique_ptr<NativeVertexFormat>>;

void Init();
void Shutdown();

void MarkAllDirty();

// Creates or obtains a pointer to a VertexFormat representing decl.
// If this results in a VertexFormat being created, if the game later uses a matching vertex
// declaration, the one that was previously created will be used.
NativeVertexFormat* GetOrCreateMatchingFormat(const PortableVertexDeclaration& decl);

// For vertex ubershaders, all attributes need to be present, even when the vertex
// format does not contain them. This function returns a vertex format with dummy
// offsets set to the unused attributes.
NativeVertexFormat* GetUberVertexFormat(const PortableVertexDeclaration& decl);

int GetVertexSize(const VertexLoaderParameters &parameters);

bool ConvertVertices(VertexLoaderParameters &parameters, u32 &readsize, u32 &writesize);

void GetVertexSizeAndComponents(const VertexLoaderParameters &parameters, u32 &vertexsize, u32 &components);

// For debugging
void AppendListToString(std::string *dest);

void UpdateVertexArrayPointers();

NativeVertexFormat* GetCurrentVertexFormat();

extern u32 g_current_components;
};
