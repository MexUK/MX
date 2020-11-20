#pragma once

#include "mx.h"
#include "Types.h"

#pragma pack(push, 1)
struct mx::DDSFormat_Header_DDS_PIXELFORMAT_Part1
{
	uint32					m_uiSize2;
	uint32					m_uiFlags2;
	uint8					m_ucFourCC[4];
	uint32					m_uiRGBBitCount;
	uint32					m_uiRBitMask;
	uint32					m_uiGBitMask;
	uint32					m_uiBBitMask;
	uint32					m_uiABitMask;
};
#pragma pack(pop)