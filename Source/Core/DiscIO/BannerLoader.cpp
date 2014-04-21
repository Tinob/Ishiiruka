// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "DiscIO/BannerLoader.h"
#include "BannerLoaderWii.h"
#include "BannerLoaderGC.h"

#include "DiscIO/VolumeCreator.h"
#include "Common/FileUtil.h"

namespace DiscIO
{

IBannerLoader* CreateBannerLoader(DiscIO::IFileSystem& _rFileSystem, DiscIO::IVolume *pVolume)
{
	if (IsVolumeWiiDisc(pVolume) || IsVolumeWadFile(pVolume))
	{
		return new CBannerLoaderWii(pVolume);
	}
	if (_rFileSystem.IsValid())
	{
		return new CBannerLoaderGC(_rFileSystem, pVolume);
	}

	return NULL;
}

} // namespace
