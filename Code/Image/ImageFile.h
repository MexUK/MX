#pragma once

#include "mx.h"
#include "Types.h"
#include "ERasterDataFormat.h"
#include <string>

class mx::ImageFile
{
public:
	uint32					m_uiImageWidth;
	uint32					m_uiImageHeight;
	uint32					m_uiBPP;
	ERasterDataFormat		m_uiRasterFormat;
	std::string				m_strRasterData;
};