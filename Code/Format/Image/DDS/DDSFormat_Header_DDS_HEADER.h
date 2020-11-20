#pragma once

#include "mx.h"
#include "Types.h"

#pragma pack(push, 1)
struct mx::DDSFormat_Header_DDS_HEADER
{
	uint32					m_uiSize;
	uint32					m_uiFlags;
	uint32					m_uiHeight;
	uint32					m_uiWidth;
	uint32					m_uiPitchOrLinearSize;
	uint32					m_uiDepth;
	uint32					m_uiMipMapCount;
	uint32					m_uiReserved1[11];
};
#pragma pack(pop)