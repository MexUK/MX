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
	m_bSerializeHeader1(true),
	m_uiBMPVersion(4),
	m_usColourPlaneCount(0)
{
}

BMPFormat::BMPFormat(Stream& stream, ImageData& image) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(image),
	m_bSerializeHeader1(true),
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
uint32			BMPFormat::detectBMPVersion(void)
{
	Stream stream;
	Reader reader(stream);

	uint32 uiBMPVersion = 0;
	string strHeader = reader.str(2);
	if (strHeader.c_str()[0] == '\0' && strHeader.c_str()[1] == '\0')
	{
		// BMP version 1.x
		uiBMPVersion = 1;
	}
	else if (strHeader.c_str()[0] == 'B' && strHeader.c_str()[1] == 'M')
	{
		// BMP version 2.x, 3.x or 4.x
		reader.seek(MX_BMP_HEADER1_SIZE);
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
	uint16 uiWidth = m_reader.ui16();
	uint16 uiHeight = m_reader.ui16();
	uint16 uiBitmapLineWidthBytes = m_reader.ui16();
	m_usColourPlaneCount = m_reader.ui8();
	uint8 uiBPP = m_reader.ui8();
	uint32 uiBitmapBitsZero = m_reader.ui8();
	
	populateImageData(uiWidth, uiHeight, uiBPP);
}

void			BMPFormat::unserializeVersion2(void)
{
	BMPFormat_Header1 header1;
	BMPFormat_Header2_Version2 header2;
	m_reader.structure<BMPFormat_Header1>(header1);
	m_reader.structure<BMPFormat_Header2_Version2>(header2);

	m_usColourPlaneCount = header2.m_usPlaneCount;

	populateImageData(header2.m_uiWidth, header2.m_uiHeight, header2.m_usBPP);
}

void			BMPFormat::unserializeVersion3(void)
{
	BMPFormat_Header1 header1;
	BMPFormat_Header2_Version3 header2;
	m_reader.structure<BMPFormat_Header1>(header1);
	m_reader.structure<BMPFormat_Header2_Version3>(header2);

	m_usColourPlaneCount = header2.m_usPlaneCount;

	populateImageData(header2.m_uiWidth, header2.m_uiHeight, header2.m_usBPP);
}

void			BMPFormat::unserializeVersion4(void)
{
	BMPFormat_Header1 header1;
	BMPFormat_Header2_Version4 header2;
	m_reader.structure<BMPFormat_Header1>(header1);
	m_reader.structure<BMPFormat_Header2_Version4>(header2);

	m_usColourPlaneCount = header2.m_usPlaneCount;

	populateImageData(header2.m_uiWidth, header2.m_uiHeight, header2.m_usBPP);
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
	
	serializeTableAndImageData(); // no table in BMP version 1
}

void			BMPFormat::serializeVersion2(void)
{
	if (shouldSerializeHeader1())
		serializeBMPHeader1(2);

	m_writer.ui32(12);
	m_writer.ui32(m_image.m_vecSize.x);
	m_writer.ui32(m_image.m_vecSize.y);
	m_writer.ui16(1); // plane count
	m_writer.ui16(m_image.getBitsPerPixel());

	serializeTableAndImageData();
}

void			BMPFormat::serializeVersion3(void)
{
	if (shouldSerializeHeader1())
		serializeBMPHeader1(3);

	m_writer.ui32(MX_BMP_HEADER2_VERSION3_SIZE);
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

	serializeTableAndImageData();
}

void			BMPFormat::serializeVersion4(void)
{
	if (shouldSerializeHeader1())
		serializeBMPHeader1(4);

	m_writer.ui32(MX_BMP_HEADER2_VERSION4_SIZE);
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

	serializeTableAndImageData();
}

// serialize utility
void			BMPFormat::serializeBMPHeader1(uint32 uiBMPVersion)
{
	m_writer.cstr((char*)"BM", 2);
	m_writer.ui32(MX_BMP_HEADER1_SIZE + getBMPHeader2Size(uiBMPVersion) + m_image.getTableSize() + m_image.getDataSize()); // file size
	m_writer.ui16(0); // reserved 1
	m_writer.ui16(0); // reserved 2
	m_writer.ui32(54); // bitmap offset
}

void			BMPFormat::serializeTableAndImageData()
{
	if (m_image.doesHaveTable())
		m_writer.cstr((char*)m_image.m_pTable, m_image.getTableSize());

	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}

// BMP header size
uint32			BMPFormat::getBMPHeader2Size(uint32 uiBMPVersion)
{
	switch (uiBMPVersion)
	{
	case 1:		return 0;
	case 2:		return MX_BMP_HEADER2_VERSION2_SIZE;
	case 3:		return MX_BMP_HEADER2_VERSION3_SIZE;
	case 4:		return MX_BMP_HEADER2_VERSION4_SIZE;
	default:	return 0;
	}
}

// populate image data
void			BMPFormat::populateImageData(uint32 uiWidth, uint32 uiHeight, uint32 uiBitsPerPixel)
{
	m_image.m_uiFormat = computeImageFormat(uiBitsPerPixel);
	m_image.m_vecSize.x = uiWidth;
	m_image.m_vecSize.y = uiHeight;

	if (computeTablePresence(uiBitsPerPixel))
	{
		m_image.checkToAllocateTable();
		m_reader.cstr((char*)m_image.m_pTable, m_image.getTableSize());
	}

	m_image.checkToAllocateData();
	m_reader.cstr((char*)m_image.m_pData, m_image.getDataSize());
}

// compute properties
EImageFormat	BMPFormat::computeImageFormat(uint16 uiBitsPerPixel)
{
	switch (uiBitsPerPixel)
	{
	case 1:		return TABLE_1;
	case 2:		return TABLE_2;
	case 4:		return TABLE_4;
	case 8:		return TABLE_8;
	case 16:	return SAMPLE_16;
	case 24:	return UNCOMPRESSED_RGB;
	case 32:	return UNCOMPRESSED_RGBA;
	default:	return UNKNOWN_IMAGE_FORMAT;
	}
}

bool			BMPFormat::computeTablePresence(uint16 uiBitsPerPixel)
{
	return uiBitsPerPixel < 16;
}