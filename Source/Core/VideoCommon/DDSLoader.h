#pragma once
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/TextureDecoder.h"
namespace DDSLoader
{
	enum DDSCompression
	{
		DDSC_NONE,
		DDSC_DXT1,		
		DDSC_DXT3,		
		DDSC_DXT5
	};
	class DDS
	{
	public:
		static DDSCompression Load_Image(const char *filename, u32 *width, u32 *height, u8* dst, u32 dstsize, u32 *requiredsize, u32* mipmapcount);
	};
};