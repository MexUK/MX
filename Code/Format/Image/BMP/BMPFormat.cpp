#include "BMPFormat.h"
#include "Image/ImageManager.h"
#include "Static/Debug.h"
#include "Exception/EExceptionCode.h"
#include "Stream/Stream.h"
#include "Static/Memory.h"
#include "Format/Image/BMP/BMPFormat_Header1.h"
#include "Format/Image/BMP/BMPFormat_Header2_Version2.h"
#include "Format/Image/BMP/BMPFormat_Header2_Version3.h"
#include "Format/Image/BMP/BMPFormat_Header2_Version4.h"

using namespace std;
using namespace mx;


BMPFormat::BMPFormat(mx::Stream& stream) :
	Format(stream, true, LITTLE_ENDIAN),
	m_bSkipBMPFileHeaderForSerialize(false),
	m_bHasPalette(false),
	m_uiBMPVersion(0),
	m_usFileType(0),
	m_usFileSize(0),
	m_usColourPlaneCount(0),
	m_uiWidth(0),
	m_uiHeight(0),
	m_usBPP(0)
{
}

// serialization
void			BMPFormat::_unserialize(void)
{
	m_uiBMPVersion = detectBMPVersion();
	switch (m_uiBMPVersion)
	{
	case 1:		return unserializeVersion1();
	case 2:		return unserializeVersion2();
	case 3:		return unserializeVersion3();
	case 4:		return unserializeVersion4();
	default:	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
}

void			BMPFormat::_serialize(void)
{
	switch (m_uiBMPVersion)
	{
	case 1:		return serializeVersion1();
	case 2:		return serializeVersion2();
	case 3:		return serializeVersion3();
	case 4:		return serializeVersion4();
	default:	throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
}

// BMP version
uint8			BMPFormat::detectBMPVersion(void)
{
	Stream stream;
	Reader reader(stream);

	uint8 uiBMPVersion = 0;
	string strHeader = reader.str(2);
	if (strHeader.c_str()[0] == '\0' && strHeader.c_str()[1] == '\0')
	{
		// BMP version 1.x
		uiBMPVersion = 1;
	}
	else if (strHeader.c_str()[0] == 'B' && strHeader.c_str()[1] == 'M')
	{
		// BMP version 2.x, 3.x or 4.x
		reader.seek(14);
		switch (reader.ui32())
		{
		case 12: // BMP version 2.x
			uiBMPVersion = 2;
			break;
		case 40: // BMP version 3.x
			uiBMPVersion = 3;
			break;
		case 108: // BMP version 4.x
			uiBMPVersion = 4;
		}
	}
	reader.seek(2);
	return uiBMPVersion;
}

// rows
void			BMPFormat::swapRows(void)
{
	ImageManager::swapRows(m_strRasterData, m_uiWidth, m_uiHeight);
}

// raster data format
ERasterDataFormat	BMPFormat::getRasterDataFormat(void)
{
	if (m_usBPP == 24)
	{
		return RASTERDATAFORMAT_BGR24;
	}
	else if (m_usBPP == 32)
	{
		return RASTERDATAFORMAT_BGRA32;
	}
	else
	{
		return RASTERDATAFORMAT_UNKNOWN;
	}
}

// raster data
void			BMPFormat::setRasterDataBGRA32(string& strRasterDataBGRA32)
{
	m_strRasterData = strRasterDataBGRA32;
}
string			BMPFormat::getRasterDataBGRA32(void)
{
	if (m_usBPP == 24)
	{
		return ImageManager::convertBGR24ToBGRA32(m_strRasterData);
	}
	else if (m_usBPP == 32)
	{
		return m_strRasterData;
	}
	else
	{
		return "";
	}
}

// version serialization
void			BMPFormat::unserializeVersion1(void)
{
	uint16 uiFileType = m_reader.ui16();
	uint16 uiBitmapType = m_reader.ui16();
	m_uiWidth = m_reader.ui16();
	m_uiHeight = m_reader.ui16();
	uint16 uiBitmapLineWidthBytes = m_reader.ui16();
	m_usColourPlaneCount = m_reader.ui8();
	m_usBPP = m_reader.ui8();
	uint32 uiBitmapBitsZero = m_reader.ui8();
	
	uint32 uiByteCount = m_uiWidth * m_uiHeight * (uint32)((float32)m_usBPP / 8.0f);
	m_strRasterData = m_reader.str(uiByteCount);
}

void			BMPFormat::unserializeVersion2(void)
{
	uint32 uiByteCount;

	// read headers 1 & 2
	BMPFormat_Header1 header1;
	BMPFormat_Header2_Version2 header2;
	m_reader.structure<BMPFormat_Header1>(header1);
	m_reader.structure<BMPFormat_Header2_Version2>(header2);

	// palette
	m_bHasPalette = header2.m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 3 * (1 << header2.m_usBPP);
		m_strPaletteData = m_reader.str(uiByteCount);
	}

	// raster data
	uiByteCount = header2.m_uiWidth * header2.m_uiHeight * (uint32)((float32)header2.m_usBPP / 8.0f);
	m_strRasterData = m_reader.str(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = header2.m_uiWidth;
	m_uiHeight = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
	m_usBPP = header2.m_usBPP;
}

void			BMPFormat::unserializeVersion3(void)
{
	uint32 uiByteCount;

	// read headers 1 & 2
	BMPFormat_Header1 header1;
	BMPFormat_Header2_Version3 header2;
	m_reader.structure<BMPFormat_Header1>(header1);
	m_reader.structure<BMPFormat_Header2_Version3>(header2);

	// palette
	m_bHasPalette = header2.m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 4 * header2.m_uiColoursUsed;
		m_strPaletteData = m_reader.str(uiByteCount);
	}

	// raster data
	uiByteCount = header2.m_uiWidth * header2.m_uiHeight * (uint32)((float32)header2.m_usBPP / 8.0f);
	m_strRasterData = m_reader.str(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = header2.m_uiWidth;
	m_uiHeight = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
	m_usBPP = header2.m_usBPP;
}

void			BMPFormat::unserializeVersion4(void)
{
	uint32 uiByteCount;

	// read headers 1 & 2
	BMPFormat_Header1 header1;
	m_reader.structure<BMPFormat_Header1>(header1);
	BMPFormat_Header2_Version4 header2;
	m_reader.structure<BMPFormat_Header2_Version4>(header2);

	// palette
	m_bHasPalette = header2.m_usBPP < 16;
	if (m_bHasPalette)
	{
		uiByteCount = 4 * header2.m_uiColoursUsed;
		m_strPaletteData = m_reader.str(uiByteCount);
	}

	// raster data
	uiByteCount = header2.m_uiWidth * header2.m_uiHeight * (uint32)((float32)header2.m_usBPP / 8.0f);
	m_strRasterData = m_reader.str(uiByteCount);

	// copy from raw structs to wrapper structs
	m_uiWidth = header2.m_uiWidth;
	m_uiHeight = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
	m_usBPP = header2.m_usBPP;
}

void			BMPFormat::serializeVersion1(void)
{
	uint32 uiBitmapLineWidthBytes = m_uiWidth * (uint32)(((float32)m_usBPP) / 8.0f);
	
	m_writer.ui16(0x0002); // RT_BITMAP
	m_writer.ui16(0x0000); // bitmap type
	m_writer.ui16((uint16)m_uiWidth);
	m_writer.ui16((uint16)m_uiHeight);
	m_writer.ui16((uint16)uiBitmapLineWidthBytes);
	m_writer.ui8((uint8)m_usColourPlaneCount);
	m_writer.ui8((uint8)m_usBPP);
	m_writer.ui32(0);
	
	m_writer.str(m_strRasterData);
}

void			BMPFormat::serializeVersion2(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 16 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(12);
	m_writer.ui32(m_uiWidth);
	m_writer.ui32(m_uiHeight);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_usBPP);

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.str(m_strRasterData);
}

void			BMPFormat::serializeVersion3(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 40 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(40);
	m_writer.ui32(m_uiWidth);
	m_writer.ui32(m_uiHeight);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_usBPP);
	m_writer.ui32(0); // uiCompressionMethods
	m_writer.ui32(m_uiWidth * m_uiHeight * (uint32)((float32)m_usBPP / 8.0f)); // uiBitmapSize
	m_writer.ui32(0); // uiHorizontalResolution
	m_writer.ui32(0); // uiVerticalResolution
	m_writer.ui32(0); // uiColoursUsed
	m_writer.ui32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.str(m_strRasterData);
}

void			BMPFormat::serializeVersion4(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 108 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_strRasterData.length();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(108);
	m_writer.ui32(m_uiWidth);
	m_writer.ui32(m_uiHeight);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_usBPP);
	m_writer.ui32(0); // uiCompressionMethods
	m_writer.ui32(m_uiWidth * m_uiHeight * (uint32)((float32)m_usBPP / 8.0f)); // uiBitmapSize
	m_writer.ui32(0); // uiHorizontalResolution
	m_writer.ui32(0); // uiVerticalResolution
	m_writer.ui32(0); // uiColoursUsed
	m_writer.ui32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.str(m_strRasterData);
}
