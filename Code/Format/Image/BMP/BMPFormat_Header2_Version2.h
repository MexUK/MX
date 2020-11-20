#pragma once

#include "mx.h"
#include "Types.h"

#pragma pack(push, 1)
struct mx::BMPFormat_Header2_Version2
{
	uint32						m_uiWidth;
	uint32						m_uiHeight;
	uint16						m_usPlaneCount;
	uint16						m_usBPP; // if < 16 then has palette
};
#pragma pack(pop)