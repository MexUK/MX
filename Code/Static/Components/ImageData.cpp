#include "ImageData.h"

using namespace mx;

ImageData::ImageData() :
	m_uiFormat(UNKNOWN_IMAGE_FORMAT),
	m_pData(nullptr),
	m_pTable(nullptr)
{
}

ImageData::ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pData) :
	m_uiFormat(uiFormat),
	m_vecSize(vecSize),
	m_pData(pData),
	m_pTable(nullptr)
{
}

// allocation for image data
void			ImageData::allocateData()
{
	m_pData = (uint8*)malloc(getDataSize());
}

void			ImageData::deallocateData()
{
	free(m_pData);
	m_pData = nullptr;
}

void			ImageData::checkToAllocateData()
{
	if (m_pData == nullptr)
		allocateData();
}

// allocation for table
void			ImageData::allocateTable()
{
	m_pTable = (uint8*)malloc(getTableSize());
}

void			ImageData::deallocateTable()
{
	free(m_pTable);
	m_pTable = nullptr;
}

void			ImageData::checkToAllocateTable()
{
	if (m_pTable == nullptr)
		allocateTable();
}

bool			ImageData::doesHaveTable()
{
	return m_pTable != nullptr;
}

// computable properties
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
		case TABLE_1:				return 1;
		case TABLE_2:				return 2;
		case TABLE_4:				return 4;
		case TABLE_8:				return 8;
		case SAMPLE_16:				return 16;
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
		case TABLE_1:				return 3;
		case TABLE_2:				return 3;
		case TABLE_4:				return 3;
		case TABLE_8:				return 3;
		case SAMPLE_16:				return 3;
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
	case TABLE_1:				return (m_vecSize.x * m_vecSize.y) / 8;
	case TABLE_2:				return (m_vecSize.x * m_vecSize.y) / 4;
	case TABLE_4:				return (m_vecSize.x * m_vecSize.y) / 2;
	case TABLE_8:				return m_vecSize.x * m_vecSize.y;
	case SAMPLE_16:				return m_vecSize.x * m_vecSize.y * 2;
	default:					return 0;
	}
}

/*
bool			ImageData::doesHaveTable()
{
	// todo
	return false;
}
*/

uint32			ImageData::getTableColourCount()
{
	switch (m_uiFormat)
	{
	case UNCOMPRESSED_RGB:		return 0;
	case UNCOMPRESSED_RGBA:		return 0;
	case COMPRESSED_RGB_DXT1:	return 0;
	case COMPRESSED_RGBA_DXT1:	return 0;
	case COMPRESSED_RGBA_DXT3:	return 0;
	case COMPRESSED_RGBA_DXT5:	return 0;
	case TABLE_1:				return 2;
	case TABLE_2:				return 4;
	case TABLE_4:				return 16;
	case TABLE_8:				return 256;
	case SAMPLE_16:				return 0;
	default:					return 0;
	}
}

uint32			ImageData::getTableSize()
{
	// todo - https://en.wikipedia.org/wiki/BMP_file_format#Color_table
	return getTableColourCount() * 4;
}