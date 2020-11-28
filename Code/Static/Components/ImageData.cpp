#include "ImageData.h"

using namespace mx;

ImageData::ImageData() :
	m_uiFormat(UNKNOWN_IMAGE_FORMAT),
	m_pRaster(nullptr),
	m_pTable(nullptr)
{
}

ImageData::ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pRaster, uint8* pTable) :
	m_uiFormat(uiFormat),
	m_vecSize(vecSize),
	m_pRaster(pRaster),
	m_pTable(pTable)
{
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

// allocation for raster
void			ImageData::allocateRaster()
{
	m_pRaster = (uint8*)malloc(getRasterSize());
}

void			ImageData::deallocateRaster()
{
	free(m_pRaster);
	m_pRaster = nullptr;
}

void			ImageData::checkToAllocateRaster()
{
	if (m_pRaster == nullptr)
		allocateRaster();
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

uint32			ImageData::getRasterSize()
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

// format conversion
void			ImageData::convertToFormat(EImageFormat uiImageFormat)
{
	if (uiImageFormat == m_uiFormat)
		return;

	switch (m_uiFormat)
	{
	case UNCOMPRESSED_RGB:
		switch (uiImageFormat)
		{
		case UNCOMPRESSED_RGBA:		return convertUncompressedRGBToUncompressedRGBA();
		case COMPRESSED_RGB_DXT1:	return;
		case COMPRESSED_RGBA_DXT1:	return;
		case COMPRESSED_RGBA_DXT3:	return;
		case COMPRESSED_RGBA_DXT5:	return;
		case TABLE_1:				return;
		case TABLE_2:				return;
		case TABLE_4:				return;
		case TABLE_8:				return;
		case SAMPLE_16:				return;
		default:					return;
		}
		break;
	case UNCOMPRESSED_RGBA:		return;
	case COMPRESSED_RGB_DXT1:	return;
	case COMPRESSED_RGBA_DXT1:	return;
	case COMPRESSED_RGBA_DXT3:	return;
	case COMPRESSED_RGBA_DXT5:	return;
	case TABLE_1:				return;
	case TABLE_2:				return;
	case TABLE_4:				return;
	case TABLE_8:				return;
	case SAMPLE_16:				return;
	default:					return;
	}
}

void			ImageData::convertUncompressedRGBToUncompressedRGBA()
{
	/*
	uint8* pNewRaster = allocateRasterForImageFormat(UNCOMPRESSED_RGBA);
	uint32 uiRasterSize = getRasterSize();
	uint32 uiNewIndex = 0;
	for (uint32 uiIndex = 0; uiIndex < uiRasterSize; uiIndex += 3)
	{
		pNewRaster[uiIndex] = m_pRaster[uiIndex];
		uiIndex++;
		pNewRaster[uiIndex] = m_pRaster[uiIndex];
		uiIndex++;
		pNewRaster[uiIndex] = m_pRaster[uiIndex];
		uiIndex++;
		pNewRaster[uiIndex++] = 0;
	}
	*/
}