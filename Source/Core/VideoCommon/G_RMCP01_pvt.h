#pragma once
#include <map>
#include "VideoCommon/VertexLoader.h"
class G_RMCP01_pvt
{
public:
static void Initialize(std::map<u64, TCompiledLoaderFunction> &pvlmap);
};
