// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
#include "Core/HW/Memmap.h"
#include "Common/Common.h"
#include "VideoCommon/CPMemory.h"
#include "VideoCommon/VertexShaderManager.h"

// CP state
u8 *cached_arraybases[16];

// STATE_TO_SAVE
u32 arraybases[16];
u32 arraystrides[16];
TMatrixIndexA MatrixIndexA;
TMatrixIndexB MatrixIndexB;
TVtxDesc g_VtxDesc;
// Most games only use the first VtxAttr and simply reconfigure it all the time as needed.
VAT g_VtxAttr[8];
int g_attr_dirty;  // bitfield 

void MarkAllAttrDirty()
{
	g_attr_dirty = 0xff;
}

void LoadCPReg(u32 sub_cmd, u32 value)
{
	switch (sub_cmd & 0xF0)
	{
	case 0x30:
		VertexShaderManager::SetTexMatrixChangedA(value);
		break;

	case 0x40:
		VertexShaderManager::SetTexMatrixChangedB(value);
		break;

	case 0x50:
		g_VtxDesc.Hex &= ~0x1FFFF;  // keep the Upper bits
		g_VtxDesc.Hex |= value;
		g_attr_dirty = 0xFF;
		break;

	case 0x60:
		g_VtxDesc.Hex &= 0x1FFFF;  // keep the lower 17Bits
		g_VtxDesc.Hex |= (u64)value << 17;
		g_attr_dirty = 0xFF;
		break;

	case 0x70:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g0.Hex = value;
		g_attr_dirty |= 1 << (sub_cmd & 7);
		break;

	case 0x80:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g1.Hex = value;
		g_attr_dirty |= 1 << (sub_cmd & 7);
		break;

	case 0x90:
		_assert_((sub_cmd & 0x0F) < 8);
		g_VtxAttr[sub_cmd & 7].g2.Hex = value;
		g_attr_dirty |= 1 << (sub_cmd & 7);
		break;

		// Pointers to vertex arrays in GC RAM
	case 0xA0:
		arraybases[sub_cmd & 0xF] = value;
		cached_arraybases[sub_cmd & 0xF] = Memory::GetPointer(value);
		break;

	case 0xB0:
		arraystrides[sub_cmd & 0xF] = value & 0xFF;
		break;
	}
}

void FillCPMemoryArray(u32 *memory)
{
	memory[0x30] = MatrixIndexA.Hex;
	memory[0x40] = MatrixIndexB.Hex;
	memory[0x50] = (u32)g_VtxDesc.Hex;
	memory[0x60] = (u32)(g_VtxDesc.Hex >> 17);

	for (int i = 0; i < 8; ++i)
	{
		memory[0x70 + i] = g_VtxAttr[i].g0.Hex;
		memory[0x80 + i] = g_VtxAttr[i].g1.Hex;
		memory[0x90 + i] = g_VtxAttr[i].g2.Hex;
	}

	for (int i = 0; i < 16; ++i)
	{
		memory[0xA0 + i] = arraybases[i];
		memory[0xB0 + i] = arraystrides[i];
	}
}

void RecomputeCachedArraybases()
{
	for (int i = 0; i < 16; i++)
	{
		cached_arraybases[i] = Memory::GetPointer(arraybases[i]);
	}
}

