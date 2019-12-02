// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <string>
#include "Common/Version.h"
#include "Common/scmrev.h"

namespace Common
{
#define VERSION_STR "FPM v1.0 BETA"
#ifdef _DEBUG
#define BUILD_TYPE_STR "Debug "
#elif defined DEBUGFAST
#define BUILD_TYPE_STR "DebugFast "
#else
#define BUILD_TYPE_STR ""
#endif

const std::string scm_rev_str = "Ishiiruka-Dolphin"
#if !SCM_IS_MASTER
"[" SCM_BRANCH_STR "] "
#endif

#ifdef __INTEL_COMPILER
" " BUILD_TYPE_STR " " VERSION_STR "-ICC";
#else
" " BUILD_TYPE_STR " " VERSION_STR;
#endif

const std::string scm_rev_git_str = SCM_REV_STR;
const std::string scm_rev_cache_str = "201911022212";
const std::string scm_desc_str = VERSION_STR;
const std::string scm_branch_str = SCM_BRANCH_STR;
const std::string scm_distributor_str = SCM_DISTRIBUTOR_STR;

#ifdef _WIN32
const std::string netplay_dolphin_ver = VERSION_STR " Win";
#elif __APPLE__
const std::string netplay_dolphin_ver = VERSION_STR " Mac";
#else
const std::string netplay_dolphin_ver = VERSION_STR " Lin";
#endif


}
