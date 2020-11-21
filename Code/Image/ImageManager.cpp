#include "ImageManager.h"
#include "RasterDataFormatManager.h"
#include "ImageFile.h"
#include "Static/String.h"
#include "Static/File.h"
#include "Static/Image.h"
#include "Static/Path.h"
#include "Stream/FileStream.h"
#include "Format/BMP/BMPFormat.h"
#include "Format/DDS/DDSFormat.h"
#include "Image/ImageManager.h"
#include "ThirdParty/STB/stb_image.h"

using namespace std;
using namespace glm;
using namespace mx;

ImageManager::ImageManager(void)
{
	m_pRasterDataFormatManager = new RasterDataFormatManager;
}
ImageManager::~ImageManager(void)
{
	delete m_pRasterDataFormatManager;
}

void					ImageManager::init(void)
{
	m_pRasterDataFormatManager->init();
}
void					ImageManager::uninit(void)
{
	m_pRasterDataFormatManager->uninit();
}

ImageFile*				ImageManager::loadImageFromFile(string& strFilePath)
{
	string str = Path::ext(strFilePath);
	string strFileExtensionUpper = String::upper(str);
	ImageFile *pImageFile = new ImageFile;

	if (strFileExtensionUpper == "BMP")
	{
		BMPFormat* pBMPFormat = Format::unserializeFileHeap<BMPFormat>(strFilePath);
		pImageFile->m_uiImageWidth = pBMPFormat->getWidth();
		pImageFile->m_uiImageHeight = pBMPFormat->getHeight();
		pImageFile->m_uiBPP = pBMPFormat->getBPP();
		pImageFile->m_uiRasterFormat = pBMPFormat->getRasterDataFormat();
		pImageFile->m_strRasterData = pBMPFormat->getRasterData();
		delete pBMPFormat;
	}
	else if (strFileExtensionUpper == "PNG"
		  || strFileExtensionUpper == "GIF"
		  || strFileExtensionUpper == "JPG"
		  || strFileExtensionUpper == "JPEG"
		  || strFileExtensionUpper == "TGA"
		  || strFileExtensionUpper == "PGM"
		  || strFileExtensionUpper == "PPM"
		  || strFileExtensionUpper == "HDR")
	{
		int w, h, n;
		uint8 *pBGRAData = stbi_load(strFilePath.c_str(), &w, &h, &n, 4);
		if (pBGRAData == NULL)
		{
			stbi_image_free(pBGRAData);
			delete pImageFile;
			return nullptr;
		}
		pImageFile->m_uiImageWidth = w;
		pImageFile->m_uiImageHeight = h;
		pImageFile->m_uiBPP = 32;
		switch (n)
		{
		case 3:
			//pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_RGB24;
			pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_RGBA32;
			break;
		case 4:
		default:
			pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_RGBA32;
			break;
		}
		pImageFile->m_strRasterData = string((char*)pBGRAData, w * h * 4); // * 4
		stbi_image_free(pBGRAData);
	}
	else if (strFileExtensionUpper == "DDS")
	{
		Stream stream(strFilePath);
		DDSFormat ddsFormat(stream);
		ddsFormat.unserialize();

		pImageFile->m_uiImageWidth = ddsFormat.m_uiWidth;
		pImageFile->m_uiImageHeight = ddsFormat.m_uiHeight;
		pImageFile->m_uiBPP = 4;
		pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_DXT1;
		pImageFile->m_strRasterData = ddsFormat.m_strRasterData;

		/*
		int width, height, channels;
		int forceChannels = SOIL_LOAD_AUTO;
		unsigned char *pRasterData = SOIL_load_image(strFilePath.c_str(), &width, &height, &channels, forceChannels);
		if (!pRasterData)
			return nullptr;
		*/

		//int reuseTextureId = 0;
		//SOIL_create_OGL_texture(pRasterData, width, height, channels, reuseTextureId, 0);

		/*
		pImageFile->m_uiImageWidth = width;
		pImageFile->m_uiImageHeight = height;
		pImageFile->m_uiBPP = 4;// channels * 8;
		pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_DXT1;
		pImageFile->m_strRasterData = string((char*)pRasterData, (width * height) / 2);
		*/

		/*
		pImageFile->m_uiImageWidth = width;
		pImageFile->m_uiImageHeight = height;
		pImageFile->m_uiBPP = 24;
		pImageFile->m_uiRasterFormat = RASTERDATAFORMAT_RGB24;
		pImageFile->m_strRasterData = string((char*)pRasterData, width * height * 3);
		*/
	}
	return pImageFile;
}

string					ImageManager::convertBGRA32ToRasterDataFormat(string& strRasterData, ERasterDataFormat ERasterDataFormatValue, string* strPaletteDataOut, uint32 uiWidth, uint32 uiHeight)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_BGRA32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGBA32:	return convertBGRA32ToRGBA32(strRasterData);
	case RASTERDATAFORMAT_BGR32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGB32:	return convertBGRA32ToRGBA32(strRasterData);
	case RASTERDATAFORMAT_PAL4:		return convertBGRA32ToPAL4(strRasterData);
	case RASTERDATAFORMAT_PAL8:		return convertBGRA32ToPAL8(strRasterData);
	case RASTERDATAFORMAT_BGR24:	return convertBGRA32ToBGR24(strRasterData);
	case RASTERDATAFORMAT_RGB24:
	{
		string str = convertBGRA32ToBGR24(strRasterData);
		return convertBGR24ToRGB24(str);
	}
	}
	return "";
}
string					ImageManager::convertRasterDataFormatToBGRA32(string& strRasterData, ERasterDataFormat ERasterDataFormatValue, string& strPaletteDataIn, uint32 uiWidth, uint32 uiHeight)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_BGRA32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGBA32:	return convertRGBA32ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_BGR32:	return strRasterData; // no conversion needed for this raster data format
	case RASTERDATAFORMAT_RGB32:	return convertRGBA32ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_PAL4:		return convertPAL4ToBGRA32(strRasterData, strPaletteDataIn, uiWidth, uiHeight);
	case RASTERDATAFORMAT_PAL8:		return convertPAL8ToBGRA32(strRasterData, strPaletteDataIn, uiWidth, uiHeight);
	case RASTERDATAFORMAT_BGR24:	return convertBGR24ToBGRA32(strRasterData);
	case RASTERDATAFORMAT_RGB24:
	{
		string str = convertRGB24ToBGR24(strRasterData);
		return convertBGR24ToBGRA32(str);
	}
	}
	return "";
}

string					ImageManager::convertBGRA32ToRGBA32(string& strRasterData)
{
	for (uint32 uiIndex32 = 0, j = strRasterData.length(); uiIndex32 < j; uiIndex32 += 4)
	{
		uint8 ucValue = strRasterData.c_str()[uiIndex32];
		strRasterData[uiIndex32] = strRasterData.c_str()[uiIndex32 + 2];
		strRasterData[uiIndex32 + 2] = ucValue;
	}
	return strRasterData;
}
string					ImageManager::convertRGBA32ToBGRA32(string& strRasterData)
{
	return convertBGRA32ToRGBA32(strRasterData);
}

string					ImageManager::convertBGRA32ToBGR24(string& strRasterData)
{
	uint32 uiIndex24 = 0;
	string strNewRasterData = "";
	strNewRasterData.resize((strRasterData.length() / 4) * 3);
	for (uint32 uiIndex32 = 0, j = strRasterData.length(); uiIndex32 < j; uiIndex32 += 4)
	{
		strNewRasterData[uiIndex24] = strRasterData.c_str()[uiIndex32];
		strNewRasterData[uiIndex24 + 1] = strRasterData.c_str()[uiIndex32 + 1];
		strNewRasterData[uiIndex24 + 2] = strRasterData.c_str()[uiIndex32 + 2];
		uiIndex24 += 3;
	}
	return strNewRasterData;
}
string					ImageManager::convertBGR24ToBGRA32(string& strRasterData)
{
	uint32 uiIndex32 = 0;
	string strNewRasterData = "";
	strNewRasterData.resize((strRasterData.length() / 3) * 4);
	for (uint32 uiIndex24 = 0, j = strRasterData.length(); uiIndex24 < j; uiIndex24 += 3)
	{
		strNewRasterData[uiIndex32] = strRasterData.c_str()[uiIndex24];
		strNewRasterData[uiIndex32 + 1] = strRasterData.c_str()[uiIndex24 + 1];
		strNewRasterData[uiIndex32 + 2] = strRasterData.c_str()[uiIndex24 + 2];
		strNewRasterData[uiIndex32 + 3] = (uint8) 255;
		uiIndex32 += 4;
	}
	return strNewRasterData;
}

string					ImageManager::convertBGR24ToRGB24(string& strRasterData)
{
	for (uint32 uiIndex24 = 0, j = strRasterData.length(); uiIndex24 < j; uiIndex24 += 3)
	{
		uint8 ucValue = strRasterData.c_str()[uiIndex24];
		strRasterData[uiIndex24] = strRasterData.c_str()[uiIndex24 + 2];
		strRasterData[uiIndex24 + 2] = ucValue;
	}
	return strRasterData;
}
string					ImageManager::convertRGB24ToBGR24(string& strRasterData)
{
	return convertBGR24ToRGB24(strRasterData);
}

string					ImageManager::convertBGRA32ToPAL4(string& strRasterData)
{
	return "";
}
string					ImageManager::convertPAL4ToBGRA32(string& strRasterData, string& strPaletteData, uint32 uiWidth, uint32 uiHeight)
{
	string strPixels;
	strPixels.resize(uiWidth * uiHeight * 4);

	uint32 uiPixelIndex = 0;
	for (uint32 i = 0, j = (uint32)((uiWidth * uiHeight) / 2); i < j; i++)
	{
		uint8 ucPaletteIndexes = strRasterData.c_str()[i] & 0xFF;
		uint8 ucPaletteIndex1 = (ucPaletteIndexes >> 4) & 0x0F;
		uint8 ucPaletteIndex2 = ucPaletteIndexes & 0x0F;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex1 * 4) + 3] & 0xFF);
		uiPixelIndex += 4;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex2 * 4) + 3] & 0xFF);
		uiPixelIndex += 4;
	}

	return strPixels;
}

string					ImageManager::convertBGRA32ToPAL8(string& strRasterData)
{
	return "";
}
string					ImageManager::convertPAL8ToBGRA32(string& strRasterData, string& strPaletteData, uint32 uiWidth, uint32 uiHeight)
{
	string strPixels;
	strPixels.resize(uiWidth * uiHeight * 4);

	uint32 uiPixelIndex = 0;
	for (uint32 i = 0, j = (uint32)(uiWidth * uiHeight); i < j; i++)
	{
		uint8 ucPaletteIndex = strRasterData.c_str()[i] & 0xFF;

		strPixels[uiPixelIndex] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 2] & 0xFF;
		strPixels[uiPixelIndex + 1] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 1] & 0xFF;
		strPixels[uiPixelIndex + 2] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 0] & 0xFF;
		strPixels[uiPixelIndex + 3] = strPaletteData.c_str()[(ucPaletteIndex * 4) + 3] & 0xFF;
		//strPixels[uiPixelIndex + 3] = ((getTexture()->getRasterFormat() & FORMAT_888) == FORMAT_888) ? 0xFF : (strPaletteData.c_str()[(ucPaletteIndex * 4) + 3] & 0xFF);

		uiPixelIndex += 4;
	}

	return strPixels;
}

string					ImageManager::convertBGRA32ToAlphaBGRA32(string& strRasterData)
{
	uint32 uiIndex24 = 0;
	string strNewRasterData = "";
	strNewRasterData.resize((strRasterData.length() / 4) * 4);
	for (uint32 uiIndex32 = 0, j = strRasterData.length(); uiIndex32 < j; uiIndex32 += 4)
	{
		uint8 uiAlpha = strRasterData.c_str()[uiIndex32 + 3];
		strNewRasterData[uiIndex32] = uiAlpha;
		strNewRasterData[uiIndex32 + 1] = uiAlpha;
		strNewRasterData[uiIndex32 + 2] = uiAlpha;
		strNewRasterData[uiIndex32 + 3] = uiAlpha;
		uiIndex24 += 3;
	}
	return strNewRasterData;
}

void					ImageManager::swapRows(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	const char *pRasterData = strRasterData.data();

	uint32 uiRasterDataSize = uiWidth * uiHeight * 4;
	char *pNewRasterData = new char[uiRasterDataSize];

	const unsigned int rows = uiHeight / 2; // Iterate only half the buffer to get a full flip
	const unsigned int row_stride = uiWidth * 4;
	unsigned char* temp_row = (unsigned char*)malloc(row_stride);

	int source_offset, target_offset;

	for (unsigned int rowIndex = 0; rowIndex < rows; rowIndex++)
	{
		source_offset = rowIndex * row_stride;
		target_offset = (uiHeight - rowIndex - 1) * row_stride;

		memcpy(temp_row, pRasterData + source_offset, row_stride);
		memcpy(pNewRasterData + source_offset, pRasterData + target_offset, row_stride);
		memcpy(pNewRasterData + target_offset, temp_row, row_stride);
	}

	free(temp_row);
	temp_row = NULL;

	strRasterData = string(pNewRasterData, uiRasterDataSize);

	/*
	uint32 uiHalfHeight = uiHeight / 2;
	for (uint32 x = 0; x < uiWidth; x++)
	{
		for (uint32 y = 0; y < uiHalfHeight; y++)
		{
			const char ucPixelValue = pRasterData[(x * uiWidth) + y];
			strRasterData[(x * uiWidth) + y] = pRasterData[(x * uiWidth) + (uiHeight - y)];
			strRasterData[(x * uiWidth) + (uiHeight - y)] = ucPixelValue;
		}
	}
	*/

	/*
	const char *pRasterData = strRasterData.data();

	uint8 ucBytes1[4], ucBytes2[4];

	for (uint32 y = 0, j = (uint32)floor(uiHeight / 2); y < j; y++)
	{
		for (uint32 x = 0; x < uiWidth; x++)
		{
			int iPixelKey1 = ((y * uiWidth) + x) * 4;
			int iPixelKey2 = ((((uiHeight - 1) - y) * uiWidth) + x) * 4;

			// read
			ucBytes1[0] = pRasterData[iPixelKey1];
			ucBytes1[1] = pRasterData[iPixelKey1 + 1];
			ucBytes1[2] = pRasterData[iPixelKey1 + 2];
			ucBytes1[3] = pRasterData[iPixelKey1 + 3];
			ucBytes2[0] = pRasterData[iPixelKey2];
			ucBytes2[1] = pRasterData[iPixelKey2 + 1];
			ucBytes2[2] = pRasterData[iPixelKey2 + 2];
			ucBytes2[3] = pRasterData[iPixelKey2 + 3];

			// write
			strRasterData[iPixelKey1] = ucBytes2[0];
			strRasterData[iPixelKey1 + 1] = ucBytes2[1];
			strRasterData[iPixelKey1 + 2] = ucBytes2[2];
			strRasterData[iPixelKey1 + 3] = ucBytes2[3];
			strRasterData[iPixelKey2] = ucBytes1[0];
			strRasterData[iPixelKey2 + 1] = ucBytes1[1];
			strRasterData[iPixelKey2 + 2] = ucBytes1[2];
			strRasterData[iPixelKey2 + 3] = ucBytes1[3];
		}
	}
	*/
}

string					ImageManager::swapColumns(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	for (uint32 x = 0, j = (uint32)floor(uiWidth / 2); x < j; x++)
	{
		for (uint32 y = 0; y < uiHeight; y++)
		{
			int iPixelKey1 = ((y * uiWidth) + x) * 4;
			int iPixelKey2 = ((y * uiWidth) + ((uiWidth - 1) - x)) * 4;

			uint8 ucBytes1[4], ucBytes2[4];

			// read
			ucBytes1[0] = strRasterData.c_str()[iPixelKey1];
			ucBytes1[1] = strRasterData.c_str()[iPixelKey1 + 1];
			ucBytes1[2] = strRasterData.c_str()[iPixelKey1 + 2];
			ucBytes1[3] = strRasterData.c_str()[iPixelKey1 + 3];
			ucBytes2[0] = strRasterData.c_str()[iPixelKey2];
			ucBytes2[1] = strRasterData.c_str()[iPixelKey2 + 1];
			ucBytes2[2] = strRasterData.c_str()[iPixelKey2 + 2];
			ucBytes2[3] = strRasterData.c_str()[iPixelKey2 + 3];

			// write
			strRasterData[iPixelKey1] = ucBytes2[0];
			strRasterData[iPixelKey1 + 1] = ucBytes2[1];
			strRasterData[iPixelKey1 + 2] = ucBytes2[2];
			strRasterData[iPixelKey1 + 3] = ucBytes2[3];
			strRasterData[iPixelKey2] = ucBytes1[0];
			strRasterData[iPixelKey2 + 1] = ucBytes1[1];
			strRasterData[iPixelKey2 + 2] = ucBytes1[2];
			strRasterData[iPixelKey2 + 3] = ucBytes1[3];
		}
	}
	return strRasterData;
}
string					ImageManager::swapRowsAndColumns(string& strRasterData, uint32 uiWidth, uint32 uiHeight)
{
	swapRows(strRasterData, uiWidth, uiHeight);
	return swapColumns(strRasterData, uiWidth, uiHeight);
}

D3DFORMAT				ImageManager::getD3DFormatFromRasterDataFormat(ERasterDataFormat ERasterDataFormatValue)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return D3DFMT_DXT1;
	case RASTERDATAFORMAT_DXT2:	return D3DFMT_DXT2;
	case RASTERDATAFORMAT_DXT3:	return D3DFMT_DXT3;
	case RASTERDATAFORMAT_DXT4:	return D3DFMT_DXT4;
	case RASTERDATAFORMAT_DXT5:	return D3DFMT_DXT5;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
	case RASTERDATAFORMAT_PAL8:
	case RASTERDATAFORMAT_PAL4:
		return D3DFMT_A8B8G8R8;
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return D3DFMT_R8G8B8;
	}
	return D3DFMT_UNKNOWN;
}
ERasterDataFormat		ImageManager::getRasterDataFormatFromD3DFormat(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return RASTERDATAFORMAT_DXT1;
	case D3DFMT_DXT2:	return RASTERDATAFORMAT_DXT2;
	case D3DFMT_DXT3:	return RASTERDATAFORMAT_DXT3;
	case D3DFMT_DXT4:	return RASTERDATAFORMAT_DXT4;
	case D3DFMT_DXT5:	return RASTERDATAFORMAT_DXT5;
	case D3DFMT_A8B8G8R8:
		return RASTERDATAFORMAT_BGRA32;
	case D3DFMT_R8G8B8:
		return RASTERDATAFORMAT_BGR24;
	}
	return RASTERDATAFORMAT_UNKNOWN;
}

EDXTCompressionType		ImageManager::getDXTCompressionTypeFromRasterDataFormat(ERasterDataFormat ERasterDataFormatValue)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return DXT_1;
	case RASTERDATAFORMAT_DXT2:	return DXT_2;
	case RASTERDATAFORMAT_DXT3:	return DXT_3;
	case RASTERDATAFORMAT_DXT4:	return DXT_4;
	case RASTERDATAFORMAT_DXT5:	return DXT_5;
	}
	return DXT_NOT_COMPRESSED;
}

uint8			ImageManager::getBPPFromRasterDataFormat(ERasterDataFormat ERasterDataFormatValue)
{
	switch (ERasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:	return 16;
	case RASTERDATAFORMAT_DXT2:	return 16;
	case RASTERDATAFORMAT_DXT3:	return 32;
	case RASTERDATAFORMAT_DXT4:	return 32;
	case RASTERDATAFORMAT_DXT5:	return 32;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
		return 32;
	case RASTERDATAFORMAT_PAL4:
		return 4;
	case RASTERDATAFORMAT_PAL8:
		return 8;
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return 24;
	}
	return 0;
}

string					ImageManager::getD3DFormatToPack(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT2:	return "DXT2";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT4:	return "DXT4";
	case D3DFMT_DXT5:	return "DXT5";
	case D3DFMT_A8B8G8R8:
		return "8888";
	case D3DFMT_R8G8B8:
		return "8880";
	}
	return "0000";
}

string					ImageManager::getD3DFormatText(D3DFORMAT d3dFormat)
{
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:	return "DXT1";
	case D3DFMT_DXT2:	return "DXT2";
	case D3DFMT_DXT3:	return "DXT3";
	case D3DFMT_DXT4:	return "DXT4";
	case D3DFMT_DXT5:	return "DXT5";
	case D3DFMT_A8B8G8R8:
		return "8888";
	case D3DFMT_R8G8B8:
		return "8880";
	}
	return "UnknownD3DFormat";
}

string					ImageManager::getFourCCFromDXTCompressionType(EDXTCompressionType EDXTCompressionTypeValue)
{
	switch (EDXTCompressionTypeValue)
	{
	case DXT_1:		return "DXT1";
	case DXT_2:		return "DXT2";
	case DXT_3:		return "DXT3";
	case DXT_4:		return "DXT4";
	case DXT_5:		return "DXT5";
	}
	return String::packUint32(0);
}
EDXTCompressionType		ImageManager::getDXTCompressionTypeFromFourCC(string& strFourCC)
{
	if (strFourCC == "DXT1")
	{
		return DXT_1;
	}
	else if (strFourCC == "DXT2")
	{
		return DXT_2;
	}
	else if (strFourCC == "DXT3")
	{
		return DXT_3;
	}
	else if (strFourCC == "DXT4")
	{
		return DXT_4;
	}
	else if (strFourCC == "DXT5")
	{
		return DXT_5;
	}
	return DXT_NOT_COMPRESSED;
}

uint8			ImageManager::getTexelByteCountFromDXTCompressionType(EDXTCompressionType EDXTCompressionTypeValue)
{
	switch (EDXTCompressionTypeValue)
	{
	case DXT_1:		return 8;
	case DXT_2:		return 16;
	case DXT_3:		return 16;
	case DXT_4:		return 16;
	case DXT_5:		return 16;
	}
	return 0;
}

void			ImageManager::copyBMPtoJPEG(string& strBMPFilePath)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	string strFileExtension = "JPEG";
	string strImageFilePath2 = Path::setExtWithCase(strBMPFilePath, strFileExtension);
	Gdiplus::Image *pImage = new Gdiplus::Image(String::convertStdStringToStdWString(strBMPFilePath).c_str());
	CLSID encoderClsid;
	GetEncoderClsid(L"image/jpeg", &encoderClsid);
	pImage->Save(String::convertStdStringToStdWString(strImageFilePath2).c_str(), &encoderClsid, NULL);
	delete pImage;

	Gdiplus::GdiplusShutdown(gdiplusToken);
}

void			ImageManager::copyBMPtoPNG(string& strBMPFilePath)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	string strFileExtension = "PNG";
	string strImageFilePath2 = Path::setExtWithCase(strBMPFilePath, strFileExtension);
	Gdiplus::Image* pImage = new Gdiplus::Image(String::convertStdStringToStdWString(strBMPFilePath).c_str());
	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	pImage->Save(String::convertStdStringToStdWString(strImageFilePath2).c_str(), &encoderClsid, NULL);
	delete pImage;

	Gdiplus::GdiplusShutdown(gdiplusToken);
}

void			ImageManager::saveBMPFile(string& strPixelsRGBA32, uvec2& vecSize, string& strBMPFilePath, string& strBMPFilePathOut)
{
	/*
	Stream stream(strBMPFilePathOut);
	BMPFormat bmp(stream);
	bmp.setBMPVersion(3);
	bmp.setBPP(32);
	bmp.setColourPlaneCount(1);
	bmp.setHasPalette(false);
	bmp.setSkipBMPFileHeaderForSerialize(false);
	bmp.setWidth(vecSize.x);
	bmp.setHeight(vecSize.y);
	bmp.setRasterData(strPixelsRGBA32);

	strBMPFilePathOut = File::getNextIncrementingFileName(strBMPFilePath);
	bmp.serialize();
	*/
}