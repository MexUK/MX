#include "BMPFormat.h"
#include "Static/Debug.h"
#include "Static/Image.h"
#include "Stream/Stream.h"
#include "Static/Memory.h"
#include "Format/BMP/BMPFormat_Header1.h"
#include "Format/BMP/BMPFormat_Header2_Version2.h"
#include "Format/BMP/BMPFormat_Header2_Version3.h"
#include "Format/BMP/BMPFormat_Header2_Version4.h"

using namespace std;
using namespace mx;

BMPFormat::BMPFormat(mx::Stream& stream) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(ImageData(
		UNKNOWN_IMAGE_FORMAT,
		g_vecDefaultUvec2,
		nullptr
	)),
	m_bSkipBMPFileHeaderForSerialize(false),
	m_bHasPalette(false),
	m_uiBMPVersion(4),
	m_usColourPlaneCount(0)
{
}

BMPFormat::BMPFormat(Stream& stream, ImageData& image) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(image),
	m_bSkipBMPFileHeaderForSerialize(false),
	m_bHasPalette(false),
	m_uiBMPVersion(4),
	m_usColourPlaneCount(0)
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

// version serialization
void			BMPFormat::unserializeVersion1(void)
{
	uint16 uiFileType = m_reader.ui16();
	uint16 uiBitmapType = m_reader.ui16();
	m_image.m_vecSize.x = m_reader.ui16();
	m_image.m_vecSize.y = m_reader.ui16();
	uint16 uiBitmapLineWidthBytes = m_reader.ui16();
	m_usColourPlaneCount = m_reader.ui8();
	uint8 uiBPP = m_reader.ui8();
	uint32 uiBitmapBitsZero = m_reader.ui8();
	
	uint32 uiImageDataSize = m_image.m_vecSize.x * m_image.m_vecSize.y * (uint32)(((float32)uiBPP) / 8.0f);
	m_reader.cstr((char*)m_image.m_pData, uiImageDataSize);
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
	m_reader.cstr((char*)m_image.m_pData, uiByteCount);

	// copy from raw structs to wrapper structs
	m_image.m_vecSize.x = header2.m_uiWidth;
	m_image.m_vecSize.y = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
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
	uiByteCount = header2.m_uiWidth * header2.m_uiHeight * (uint32)(((float32)header2.m_usBPP) / 8.0f);
	m_reader.cstr((char*)m_image.m_pData, uiByteCount);

	// copy from raw structs to wrapper structs
	m_image.m_vecSize.x = header2.m_uiWidth;
	m_image.m_vecSize.y = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
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
	m_reader.cstr((char*)m_image.m_pData, uiByteCount);

	// copy from raw structs to wrapper structs
	m_image.m_vecSize.x = header2.m_uiWidth;
	m_image.m_vecSize.y = header2.m_uiHeight;
	m_usColourPlaneCount = header2.m_usPlaneCount;
}

void			BMPFormat::serializeVersion1(void)
{
	uint32 uiBitmapLineWidthBytes = m_image.m_vecSize.x * (uint32)(((float32)m_image.getBitsPerPixel()) / 8.0f);
	
	m_writer.ui16(0x0002); // RT_BITMAP
	m_writer.ui16(0x0000); // bitmap type
	m_writer.ui16((uint16)m_image.m_vecSize.x);
	m_writer.ui16((uint16)m_image.m_vecSize.y);
	m_writer.ui16((uint16)uiBitmapLineWidthBytes);
	m_writer.ui8((uint8)m_usColourPlaneCount);
	m_writer.ui8((uint8)m_image.getBitsPerPixel());
	m_writer.ui32(0);
	
	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}

void			BMPFormat::serializeVersion2(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 16 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_image.getDataSize();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(12);
	m_writer.ui32(m_image.m_vecSize.x);
	m_writer.ui32(m_image.m_vecSize.y);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_image.getBitsPerPixel());

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}

void			BMPFormat::serializeVersion3(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 40 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_image.getDataSize();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(40);
	m_writer.ui32(m_image.m_vecSize.x);
	m_writer.ui32(m_image.m_vecSize.y);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_image.getBitsPerPixel());
	m_writer.ui32(0); // uiCompressionMethods
	m_writer.ui32(m_image.m_vecSize.x * m_image.m_vecSize.y * (uint32)(((float32)m_image.getBitsPerPixel()) / 8.0f)); // uiBitmapSize
	m_writer.ui32(0); // uiHorizontalResolution
	m_writer.ui32(0); // uiVerticalResolution
	m_writer.ui32(0); // uiColoursUsed
	m_writer.ui32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}

void			BMPFormat::serializeVersion4(void)
{
	if (!getSkipBMPFileHeaderForSerialize())
	{
		uint32 uiFileSize = 14 + 108 + (m_bHasPalette ? m_strPaletteData.length() : 0) + m_image.getDataSize();

		string str = "BM";
		m_writer.str(str);
		m_writer.ui32(uiFileSize); // file size
		m_writer.ui16(0); // reserved 1
		m_writer.ui16(0); // reserved 2
		m_writer.ui32(54); // bitmap offset
	}

	m_writer.ui32(108);
	m_writer.ui32(m_image.m_vecSize.x);
	m_writer.ui32(m_image.m_vecSize.y);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_image.getBitsPerPixel());
	m_writer.ui32(0); // uiCompressionMethods
	m_writer.ui32(m_image.m_vecSize.x * m_image.m_vecSize.y * (uint32)((float32)m_image.getBitsPerPixel() / 8.0f)); // uiBitmapSize
	m_writer.ui32(0); // uiHorizontalResolution
	m_writer.ui32(0); // uiVerticalResolution
	m_writer.ui32(0); // uiColoursUsed
	m_writer.ui32(0); // uiColoursImportant

	if (m_bHasPalette)
	{
		m_writer.str(m_strPaletteData);
	}

	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}
