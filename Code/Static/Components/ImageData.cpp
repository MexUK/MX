#include "ImageData.h"

using namespace mx;

ImageData::ImageData() :
	m_uiFormat(UNKNOWN_IMAGE_FORMAT),
	m_pData(nullptr)
{
}

ImageData::ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pData) :
	m_uiFormat(uiFormat),
	m_vecSize(vecSize),
	m_pData(pData)
{
}

ImageData::~ImageData()
{
	if(m_pData)
		free(m_pData);
}

uint32			ImageData::getBitsPerPixel()
{
	switch(m_uiFormat)
	{
		case UNCOMPRESSED_RGB:		return 24;
		case UNCOMPRESSED_RGBA:		return 32;
		case COMPRESSED_RGB_DXT1:	return 4;
		case COMPRESSED_RGBA_DXT1:	return 4;
		case COMPRESSED_RGBA_DXT3:	return 8;
		case COMPRESSED_RGBA_DXT5:	return 8;
		default:					return 0;
	}
}

uint32			ImageData::getChannelCount()
{
	switch(m_uiFormat)
	{
		case UNCOMPRESSED_RGB:		return 3;
		case UNCOMPRESSED_RGBA:		return 4;
		case COMPRESSED_RGB_DXT1:	return 3;
		case COMPRESSED_RGBA_DXT1:	return 4;
		case COMPRESSED_RGBA_DXT3:	return 4;
		case COMPRESSED_RGBA_DXT5:	return 4;
		default:					return 0;
	}
}

uint32			ImageData::getDataSize()
{
	switch (m_uiFormat)
	{
	case UNCOMPRESSED_RGB:		return m_vecSize.x * m_vecSize.y * 3;
	case UNCOMPRESSED_RGBA:		return m_vecSize.x * m_vecSize.y * 4;
	case COMPRESSED_RGB_DXT1:	return (m_vecSize.x * m_vecSize.y) / 2;
	case COMPRESSED_RGBA_DXT1:	return (m_vecSize.x * m_vecSize.y) / 2;
	case COMPRESSED_RGBA_DXT3:	return m_vecSize.x * m_vecSize.y;
	case COMPRESSED_RGBA_DXT5:	return m_vecSize.x * m_vecSize.y;
	default:					return 0;
	}
}