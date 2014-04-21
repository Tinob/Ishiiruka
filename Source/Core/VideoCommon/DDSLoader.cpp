#include "DDSLoader.h"
#include "Common/Common.h"
namespace DDSLoader
{
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
	((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
	/*
	* FOURCC codes for DX compressed-texture pixel formats
	*/
#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))
	/*
	* Flags
	*/
#define DDSD_CAPS	0x00000001
#define DDSD_HEIGHT	0x00000002
#define DDSD_WIDTH	0x00000004
#define DDSD_PITCH	0x00000008
#define DDSD_PIXELFORMAT	0x00001000
#define DDSD_MIPMAPCOUNT	0x00020000
#define DDSD_LINEARSIZE	0x00080000
#define DDSD_DEPTH	0x00800000

	/*
	* Pixel Fomat flags
	*/
#define DDPF_ALPHAPIXELS	0x00000001
#define DDPF_FOURCC	0x00000004
#define DDPF_RGB	0x00000040
const u32 DDSSignature = ('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24);

typedef struct _DDPIXELFORMAT
{
	u32       dwSize;                 // size of structure
	u32       dwFlags;                // pixel format flags
	u32       dwFourCC;               // (FOURCC code)
	union
	{
		u32   dwRGBBitCount;          // how many bits per pixel
		u32   dwYUVBitCount;          // how many bits per pixel
		u32   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
		u32   dwAlphaBitDepth;        // how many bits for alpha channels
		u32   dwLuminanceBitCount;    // how many bits per pixel
		u32   dwBumpBitCount;         // how many bits per "buxel", total
		u32   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
		// format list and if DDPF_D3DFORMAT is set
	};
	union
	{
		u32   dwRBitMask;             // mask for red bit
		u32   dwYBitMask;             // mask for Y bits
		u32   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
		u32   dwLuminanceBitMask;     // mask for luminance bits
		u32   dwBumpDuBitMask;        // mask for bump map U delta bits
		u32   dwOperations;           // DDPF_D3DFORMAT Operations
	};
	union
	{
		u32   dwGBitMask;             // mask for green bits
		u32   dwUBitMask;             // mask for U bits
		u32   dwZBitMask;             // mask for Z bits
		u32   dwBumpDvBitMask;        // mask for bump map V delta bits
		struct
		{
			u16    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
			u16    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
		} MultiSampleCaps;

	};
	union
	{
		u32   dwBBitMask;             // mask for blue bits
		u32   dwVBitMask;             // mask for V bits
		u32   dwStencilBitMask;       // mask for stencil bits
		u32   dwBumpLuminanceBitMask; // mask for luminance in bump map
	};
	union
	{
		u32   dwRGBAlphaBitMask;      // mask for alpha channel
		u32   dwYUVAlphaBitMask;      // mask for alpha channel
		u32   dwLuminanceAlphaBitMask;// mask for alpha channel
		u32   dwRGBZBitMask;          // mask for Z channel
		u32   dwYUVZBitMask;          // mask for Z channel
	};
} DDPIXELFORMAT;

typedef struct _DDCOLORKEY
{
	u32       dwColorSpaceLowValue;   // low boundary of color space that is to
	// be treated as Color Key, inclusive
	u32       dwColorSpaceHighValue;  // high boundary of color space that is
	// to be treated as Color Key, inclusive
} DDCOLORKEY;

typedef struct _DDSCAPS2
{
	u32       dwCaps;         // capabilities of surface wanted
	u32       dwCaps2;
	u32       dwCaps3;
	union
	{
		u32       dwCaps4;
		u32       dwVolumeDepth;
	};
} DDSCAPS2;

typedef struct _DDSHeader {
	u32      dwSignature;
	u32      dwSize;
	u32      dwFlags;
	u32      dwHeight;
	u32      dwWidth;
	union {
		s32  lPitch;
		u32 dwLinearSize;
	};
	union {
		u32 dwBackBufferCount;
		u32 dwDepth;
	};
	union {
		u32 dwMipMapCount;
		u32 dwRefreshRate;
		u32 dwSrcVBHandle;
	};
	u32      dwAlphaBitDepth;
	u32      dwReserved;
	u32     lpSurface;
	union {
		DDCOLORKEY ddckCKDestOverlay;
		u32      dwEmptyFaceColor;
	};
	DDCOLORKEY ddckCKDestBlt;
	DDCOLORKEY ddckCKSrcOverlay;
	DDCOLORKEY ddckCKSrcBlt;
	union {
		DDPIXELFORMAT ddpfPixelFormat;
		u32         dwFVF;
	};
	DDSCAPS2   ddsCaps;
	u32      dwTextureStage;
} DDSHeader;

DDSCompression DDS::Load_Image(const char *filename, u32 *width, u32 *height, u8* dst, u32 dstsize, u32 *requiredsize, u32* mipmapcount)
{
	DDSCompression Result = DDSC_NONE;
	if (NULL == dst)
	{
		return Result;
	}
	DDSHeader ddsd;
	FILE *pFile;
	u32 factor = 1, bufferSize = 0, block_size = 8;

	// Open the file
	pFile = fopen(filename, "rb");

	if (pFile == NULL)
	{
		return Result;
	}

	// Get the surface descriptor
	fread(&ddsd, sizeof(ddsd), 1, pFile);
	if (ddsd.dwSignature != DDSSignature || ddsd.dwSize != 124)
	{
		fclose(pFile);
		return Result;
	}
	// Check for a valid Header
	u32 flag = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	if ((ddsd.dwFlags & flag) != flag)
	{
		fclose(pFile);
		return Result;
	}
	flag = DDPF_FOURCC | DDPF_RGB;
	if ((ddsd.ddpfPixelFormat.dwFlags & flag) == 0)
	{
		fclose(pFile);
		return Result;
	}
	if (ddsd.ddpfPixelFormat.dwSize != 32)
	{
		fclose(pFile);
		return Result;
	}

	//
	// Suport only Basic DDS compresion Formats
	//

	switch (ddsd.ddpfPixelFormat.dwFourCC)
	{
	case FOURCC_DXT1:
		// DXT1's compression ratio is 8:1
		Result = DDSC_DXT1;
		factor = 2;
		block_size = 8;
		break;
	case FOURCC_DXT3:
		// DXT3's compression ratio is 4:1
		Result = DDSC_DXT3;
		factor = 4;
		block_size = 16;
		break;
	case FOURCC_DXT5:
		// DXT5's compression ratio is 4:1
		Result = DDSC_DXT5;
		factor = 4;
		block_size = 16;
		break;
	default:
		// the format is not supported so return inmediatelly
		fclose(pFile);
		return Result;
	}

	//
	// How big will the buffer need to be to load all of the pixel data 
	// including mip-maps?
	//

	if (ddsd.dwLinearSize == 0)
	{
		// Buffer size is not preset so calculate it
		ddsd.dwLinearSize = ((ddsd.dwWidth + 3) >> 2)*((ddsd.dwHeight + 3) >> 2)*block_size;
	}

	if (ddsd.dwMipMapCount > 1)
		bufferSize = ddsd.dwLinearSize * factor;
	else
		bufferSize = ddsd.dwLinearSize;

	// Check available buffer size
	*requiredsize = bufferSize;
	if (bufferSize > dstsize)
	{
		fclose(pFile);
		return Result;
	}

	fread(dst, 1, bufferSize, pFile);

	// Close the file
	fclose(pFile);

	*width = ddsd.dwWidth;
	*height = ddsd.dwHeight;
	*mipmapcount = ddsd.dwMipMapCount;
	return Result;
}
};