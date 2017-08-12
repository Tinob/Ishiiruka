// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
// Added for Ishiiruka By Tino

// Copyright 2014 Rodolfo Bogado
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the owner nor the names of its contributors may
//       be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "Common/Common.h"

#include "VideoCommon/ImageLoader.h"
#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/VideoConfig.h"

#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((u32)(u8)(ch0) | ((u32)(u8)(ch1) << 8) | ((u32)(u8)(ch2) << 16) | ((u32)(u8)(ch3) << 24 ))
    /*
    * FOURCC codes for DX compressed-texture pixel formats
    */
#define DDS_SIGNARURE (MAKEFOURCC('D','D','S',' '))
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
#define DDPF_FOURCC 0x00000004      // DDPF_FOURCC
#define DDPF_RGB 0x00000040         // DDPF_RGB
#define DDPF_RGBA 0x00000041        // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDPF_LUMINANCE 0x00020000   // DDPF_LUMINANCE
#define DDPF_LUMINANCEA 0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDPF_ALPHA 0x00000002       // DDPF_ALPHA
#define DDPF_PAL8 0x00000020        // DDPF_PALETTEINDEXED8
#define DDPF_PAL8A 0x00000021       // DDPF_PALETTEINDEXED8 | DDPF_ALPHAPIXELS
#define DDPF_BUMPDUDV 0x00080000    // DDPF_BUMPDUDV

    // Subset here matches D3D10_RESOURCE_DIMENSION and D3D11_RESOURCE_DIMENSION
enum DDS_RESOURCE_DIMENSION
{
  DDS_DIMENSION_TEXTURE1D = 2,
  DDS_DIMENSION_TEXTURE2D = 3,
  DDS_DIMENSION_TEXTURE3D = 4,
};
#pragma pack(push, 1)
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

typedef struct _DDSHeader
{
  u32      dwSignature;
  u32      dwSize;
  u32      dwFlags;
  u32      dwHeight;
  u32      dwWidth;
  union
  {
    s32  lPitch;
    u32 dwLinearSize;
  };
  union
  {
    u32 dwBackBufferCount;
    u32 dwDepth;
  };
  union
  {
    u32 dwMipMapCount;
    u32 dwRefreshRate;
    u32 dwSrcVBHandle;
  };
  u32      dwAlphaBitDepth;
  u32      dwReserved;
  u32     lpSurface;
  union
  {
    DDCOLORKEY ddckCKDestOverlay;
    u32      dwEmptyFaceColor;
  };
  DDCOLORKEY ddckCKDestBlt;
  DDCOLORKEY ddckCKSrcOverlay;
  DDCOLORKEY ddckCKSrcBlt;
  union
  {
    DDPIXELFORMAT ddpfPixelFormat;
    u32         dwFVF;
  };
  DDSCAPS2   ddsCaps;
  u32      dwTextureStage;
} DDSHeader;

struct _DDSHeader_DXT10
{
  uint32_t dxgiFormat;
  uint32_t resourceDimension;
  uint32_t miscFlag;  // see DDS_RESOURCE_MISC_FLAG
  uint32_t arraySize;
  uint32_t miscFlags2;  // see DDS_MISC_FLAGS2
};

#pragma pack(pop)

static_assert(sizeof(_DDSHeader) == 128, "DDS Header size mismatch");
static_assert(sizeof(_DDSHeader_DXT10) == 20, "DDS DX10 Extended Header size mismatch");

bool ImageLoader::ReadDDS(ImageLoaderParams& loader_params)
{
  DDSHeader ddsd;
  size_t header_size = 0;
  File::IOFile file;
  file.Open(loader_params.Path, "rb");
  if (!file.IsOpen())
    return false;
  u32 block_size = 8;

  // Get the surface descriptor
  if (!file.ReadBytes(&ddsd, sizeof(ddsd)) || ddsd.dwSignature != DDS_SIGNARURE || ddsd.dwSize != 124)
  {
    return false;
  }
  header_size += sizeof(ddsd);
  // Check for a valid Header
  u32 flag = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
  if ((ddsd.dwFlags & flag) != flag)
  {
    return false;
  }

  // Image should be 2D.
  if (ddsd.dwFlags & DDSD_DEPTH)
    return false;

  flag = DDPF_FOURCC | DDPF_RGB;
  if ((ddsd.ddpfPixelFormat.dwFlags & flag) == 0)
  {
    return false;
  }
  if (ddsd.ddpfPixelFormat.dwSize != 32)
  {
    return false;
  }
  // Handle DX10 extension header.
  u32 dxt10_format = 0;
  if (ddsd.ddpfPixelFormat.dwFourCC == MAKEFOURCC('D', 'X', '1', '0'))
  {
    _DDSHeader_DXT10 dxt10_header;
    if (!file.ReadBytes(&dxt10_header, sizeof(dxt10_header)))
      return false;
    // Can't handle array textures here. Doesn't make sense to use them, anyway.
    if (dxt10_header.resourceDimension != DDS_DIMENSION_TEXTURE2D || dxt10_header.arraySize != 1)
      return false;

    header_size += sizeof(dxt10_header);
    dxt10_format = dxt10_header.dxgiFormat;
  }

  //
  // Suport only Basic DDS compresion Formats
  //
  u32 FourCC = ddsd.ddpfPixelFormat.dwFourCC;
  if ((FourCC == FOURCC_DXT1 || dxt10_format == 71)
    && g_ActiveConfig.backend_info.bSupportedFormats[HostTextureFormat::PC_TEX_FMT_DXT1])
  {
    block_size = 8;
  }
  else if ((FourCC == FOURCC_DXT3 || dxt10_format == 74)
    && g_ActiveConfig.backend_info.bSupportedFormats[HostTextureFormat::PC_TEX_FMT_DXT3])
  {
    block_size = 16;
  }
  else if ((FourCC == FOURCC_DXT5 || dxt10_format == 77)
    && g_ActiveConfig.backend_info.bSupportedFormats[HostTextureFormat::PC_TEX_FMT_DXT5])
  {
    block_size = 16;
  }
  else if (dxt10_format == 98
    && g_ActiveConfig.backend_info.bSupportedFormats[HostTextureFormat::PC_TEX_FMT_BPTC])
  {
    block_size = 16;
  }
  else
  {
    // unsupported format
    return false;
  }

  //
  // How big will the buffer need to be to load all of the pixel data 
  // including mip-maps?	
  ddsd.dwLinearSize = ((ddsd.dwWidth + 3) >> 2)*((ddsd.dwHeight + 3) >> 2)*block_size;
  ddsd.dwMipMapCount = std::min<u32>(std::max<u32>(IntLog2(std::max(ddsd.dwWidth, ddsd.dwHeight)) - 2, 0), ddsd.dwMipMapCount);
  bool mipmapspresent = ddsd.dwMipMapCount > 1 && ddsd.dwFlags & DDSD_MIPMAPCOUNT;
  loader_params.data_size = ddsd.dwLinearSize;
  if (mipmapspresent)
  {
    // calculate mipmaps size
    loader_params.data_size += block_size;
    u32 w = ddsd.dwWidth;
    u32 h = ddsd.dwHeight;
    u32 level = 0;
    while ((w > 1 || h > 1) && level < ddsd.dwMipMapCount)
    {
      level++;
      w = std::max(w >> 1, 1u);
      h = std::max(h >> 1, 1u);
      loader_params.data_size += ((w + 3) >> 2)*((h + 3) >> 2) * block_size;
    }
  }
  else
  {
    ddsd.dwMipMapCount = 0;
  }
  size_t remaining = file.GetSize() - header_size;
  if (remaining < loader_params.data_size)
  {
    loader_params.data_size = ddsd.dwLinearSize;
    ddsd.dwMipMapCount = 0;
    mipmapspresent = false;
    if (remaining < loader_params.data_size)
    {
      // Invalid File size
      return false;
    }
  }
  // Check available buffer size
  loader_params.dst = loader_params.request_buffer_delegate(loader_params.data_size, mipmapspresent);
  if (loader_params.dst == nullptr)
  {
    return false;
  }

  if (!file.ReadBytes(loader_params.dst, loader_params.data_size))
  {
    //Unable to read file
    return false;
  }

  if (FourCC == FOURCC_DXT1 || dxt10_format == 71)
  {
    loader_params.resultTex = HostTextureFormat::PC_TEX_FMT_DXT1;
  }
  else if (FourCC == FOURCC_DXT3 || dxt10_format == 74)
  {
    loader_params.resultTex = HostTextureFormat::PC_TEX_FMT_DXT3;
  }
  else if (FourCC == FOURCC_DXT5 || dxt10_format == 77)
  {
    loader_params.resultTex = HostTextureFormat::PC_TEX_FMT_DXT5;
  }
  else
  {
    loader_params.resultTex = HostTextureFormat::PC_TEX_FMT_BPTC;
  }
  loader_params.Width = ddsd.dwWidth;
  loader_params.Height = ddsd.dwHeight;
  loader_params.nummipmaps = ddsd.dwMipMapCount;
  return true;
}

bool TextureToDDS(const u8* data, int row_stride, const std::string& filename, int width, int height, DDSCompression format)
{
  DDSHeader header = { 0 };
  header.dwSignature = DDS_SIGNARURE;
  header.dwSize = 124;
  header.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
  header.ddpfPixelFormat.dwFlags = DDPF_FOURCC | DDPF_RGB;
  header.ddpfPixelFormat.dwSize = 32;
  header.dwWidth = width;
  header.dwHeight = height;
  switch (format)
  {
  case DDSC_DXT1:
    header.ddpfPixelFormat.dwFourCC = FOURCC_DXT1;
    break;
  case DDSC_DXT3:
    header.ddpfPixelFormat.dwFourCC = FOURCC_DXT3;
    break;
  case DDSC_DXT5:
    header.ddpfPixelFormat.dwFourCC = FOURCC_DXT5;
    break;
  default:
    break;
  }
  header.dwLinearSize = ((header.dwWidth + 3) >> 2)*((header.dwHeight + 3) >> 2) * 16;
  header.dwMipMapCount = 1;
  File::IOFile fp(filename, "wb");
  if (!fp.IsOpen())
  {
    PanicAlertT("Screenshot failed: Could not open file %s %d", filename.c_str(), errno);
    return false;
  }
  fp.WriteBytes(&header, sizeof(DDSHeader));
  u32 ddstride = ((header.dwWidth + 3) >> 2) * 16;
  u32 lines = ((header.dwHeight + 3) >> 2);
  for (size_t i = 0; i < lines; i++)
  {
    fp.WriteBytes(data, ddstride);
    data += row_stride;
  }
  return true;
}
