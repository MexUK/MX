#pragma once

#include "mx.h"
#include "Types.h"

#pragma pack(push, 1)
struct mx::BMPFormat_Header1
{
	uint32						m_uiFileSize;
	uint32						m_usReserved1;
	uint16						m_usReserved2;
	uint16						m_uiBitmapOffset;
	uint32						m_uiBitmpapHeaderSize;
};
#pragma pack(pop)