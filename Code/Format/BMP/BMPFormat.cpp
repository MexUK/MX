#include "BMPFormat.h"
#include "Static/Debug.h"
#include "Static/Image.h"
#include "Stream/Stream.h"
#include "Static/Memory.h"
#include "Format/FormatException.h"

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
	m_uiSerializeBMPVersion(4)
{
}

BMPFormat::BMPFormat(Stream& stream, ImageData& image) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(image),
	m_bSerializeHeader1(true),
	m_uiSerializeBMPVersion(4)
{
}

// serialization
void			BMPFormat::_unserialize(void)
{
	parseFileHeader();
	parseDIBHeader();
	parseTable();
	parseRaster();
}

void			BMPFormat::_serialize(void)
{
	storeFileHeader();
	storeDIBHeader();
	storeTable();
	storeRaster();
}

// component utility
EBMPFileType		BMPFormat::getFileTypeByTwoCC(uint16 uiTwoCC)
{
	switch (uiTwoCC)
	{
	case 'BM':		return BMP_FILE_TYPE_WINDOWS;
	case 'BA':		return BMP_FILE_TYPE_OS2;
	case 'CI':		return BMP_FILE_TYPE_COLOUR_ICON;
	case 'CP':		return BMP_FILE_TYPE_COLOUR_POINTER;
	case 'IC':		return BMP_FILE_TYPE_STRUCT_ICON;
	case 'PT':		return BMP_FILE_TYPE_POINTER;
	default:		return UNKNOWN_BMP_FILE_TYPE;
	}
}

uint16				BMPFormat::getTwoCCByFileType(EBMPFileType uiBMPFileType)
{
	switch (uiBMPFileType)
	{
	case BMP_FILE_TYPE_WINDOWS:			return 'BM';
	case BMP_FILE_TYPE_OS2:				return 'BA';
	case BMP_FILE_TYPE_COLOUR_ICON:		return 'CI';
	case BMP_FILE_TYPE_COLOUR_POINTER:	return 'CP';
	case BMP_FILE_TYPE_STRUCT_ICON:		return 'IC';
	case BMP_FILE_TYPE_POINTER:			return 'PT';
	default:							return '\0\0';
	}
}

EBMPDIBHeaderType	BMPFormat::getDIBHeaderTypeByFileType(EBMPFileType uiBMPFileType)
{
	switch (uiBMPFileType)
	{
	case BMP_FILE_TYPE_WINDOWS:			return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_FILE_TYPE_OS2:				return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_FILE_TYPE_COLOUR_ICON:		return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_FILE_TYPE_COLOUR_POINTER:	return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_FILE_TYPE_STRUCT_ICON:		return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_FILE_TYPE_POINTER:			return BMP_DIB_HEADER_TYPE_OS2;
	default:							return UNKNOWN_BMP_DIB_HEADER_TYPE;
	}
}

// header detection
EBMPDIBHeaderType	BMPFormat::detectFileHeaderTwoCC()
{
	m_meta.m_uiFileType = getFileTypeByTwoCC(m_reader.ui16());
	return getDIBHeaderTypeByFileType(m_meta.m_uiFileType);
}

EBMPDIBHeader	BMPFormat::detectDIBHeader(EBMPDIBHeaderType uiDIBHeaderType)
{
	m_reader.seek(MX_BMP_HEADER1_SIZE);
	switch (m_reader.ui32())
	{
	case 12:		return uiDIBHeaderType == BMP_DIB_HEADER_TYPE_WINDOWS ? BMP_DIB_HEADER_BITMAPCOREHEADER : BMP_DIB_HEADER_OS21XBITMAPHEADER;
	case 64:		return BMP_DIB_HEADER_OS22XBITMAPHEADER_64;
	case 16:		return BMP_DIB_HEADER_OS22XBITMAPHEADER_16;
	case 40:		return BMP_DIB_HEADER_BITMAPINFOHEADER;
	case 52:		return BMP_DIB_HEADER_BITMAPV2INFOHEADER;
	case 56:		return BMP_DIB_HEADER_BITMAPV3INFOHEADER;
	case 108:		return BMP_DIB_HEADER_BITMAPV4HEADER;
	case 124:		return BMP_DIB_HEADER_BITMAPV5HEADER;
	default:		return UNKNOWN_BMP_DIB_HEADER;
	}
}

// component detection
EBMPCompressionType	BMPFormat::detectCompressionType(uint32 uiCompressionType, EBMPDIBHeaderType uiDIBHeaderType)
{
	if (uiDIBHeaderType == BMP_DIB_HEADER_TYPE_OS2 && uiCompressionType > 4)
		return UNKNOWN_BMP_COMPRESSION_TYPE;

	if (m_meta.getDIBHeaderType() == BMP_DIB_HEADER_TYPE_WINDOWS && uiCompressionType > 5)
		return UNKNOWN_BMP_COMPRESSION_TYPE;

	switch (uiCompressionType)
	{
	case 0:		return BMP_COMPRESSION_RGB;
	case 1:		return BMP_COMPRESSION_RLE8;
	case 2:		return BMP_COMPRESSION_RLE4;
	case 3:		return BMP_COMPRESSION_BITFIELDS;
	case 4:		return BMP_COMPRESSION_JPEG;
	case 5:		return BMP_COMPRESSION_PNG;
	case 6:		return BMP_COMPRESSION_ALPHABITFIELDS;
	case 11:	return BMP_COMPRESSION_CMYK;
	case 12:	return BMP_COMPRESSION_CMYKRLE8;
	case 13:	return BMP_COMPRESSION_CMYKRLE4;
	default:	return UNKNOWN_BMP_COMPRESSION_TYPE;
	}
}

bool			BMPFormat::detectTablePresence()
{
	return m_meta.m_uiBitPerPixelCount < 16; // todo
}

// component computation
uint32			BMPFormat::computeFileSize()
{
	return computeRasterOffset() + m_image.getRasterSize();
}

uint32			BMPFormat::computeTableSize()
{
	return computeTableEntryByteCount() * m_meta.m_uiTableEntryCount;
}

uint32			BMPFormat::computeTableEntryByteCount()
{
	// todo: "the color table can optionally consist of 2-byte entries – these entries constitute indexes to the currently realized palette[8] instead of explicit RGB color definitions."
	if (m_meta.getDIBHeaderType() == BMP_DIB_HEADER_TYPE_WINDOWS)
		return 4;
	else if (m_meta.getDIBHeaderType() == BMP_DIB_HEADER_TYPE_OS2)
		return 3;
	else
		return 0;
}

EImageFormat	BMPFormat::computeRasterFormat()
{
	switch (m_meta.m_uiBitPerPixelCount)
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

uint32			BMPFormat::computeRasterOffset()
{
	return MX_BMP_HEADER1_SIZE
		+ m_meta.getDIBHeaderSize()
		+ (m_meta.getDIBHeaderType() == BMP_DIB_HEADER_TYPE_WINDOWS && m_meta.getWinBMPVersion() == 5 ? m_meta.m_uiProfileDataSize : 0)
		+ m_image.getTableSize();
}

// component parsing
void			BMPFormat::parseFileHeader()
{
	EBMPDIBHeaderType uiDIBHeaderType = detectFileHeaderTwoCC();
	if(uiDIBHeaderType == UNKNOWN_BMP_DIB_HEADER)
		throw new FormatException(INVALID_FORMAT);

	m_meta.m_uiDIBHeader = detectDIBHeader(uiDIBHeaderType);
	if(m_meta.m_uiDIBHeader == UNKNOWN_BMP_DIB_HEADER)
		throw new FormatException(INVALID_FORMAT);
}

void			BMPFormat::parseDIBHeader()
{
	switch (m_meta.m_uiDIBHeader)
	{
	case BMP_DIB_HEADER_BITMAPCOREHEADER:		return parseDIBHeader_BITMAPCOREHEADER();
	case BMP_DIB_HEADER_OS21XBITMAPHEADER:		return parseDIBHeader_OS21XBITMAPHEADER();
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_64:	return parseDIBHeader_OS22XBITMAPHEADER_64();
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_16:	return parseDIBHeader_OS22XBITMAPHEADER_16();
	case BMP_DIB_HEADER_BITMAPV2INFOHEADER:		return parseDIBHeader_BITMAPV2INFOHEADER();
	case BMP_DIB_HEADER_BITMAPV3INFOHEADER:		return parseDIBHeader_BITMAPV3INFOHEADER();
	case BMP_DIB_HEADER_BITMAPV4HEADER:			return parseDIBHeader_BITMAPV4HEADER();
	case BMP_DIB_HEADER_BITMAPV5HEADER:			return parseDIBHeader_BITMAPV5HEADER();
	}
}

void			BMPFormat::parseTable()
{
	if (detectTablePresence())
	{
		m_image.checkToAllocateTable();
		m_reader.cstr((char*)m_image.m_pTable, computeTableSize());
	}
}

void			BMPFormat::parseRaster()
{
	m_image.checkToAllocateRaster();
	m_reader.cstr((char*)m_image.m_pRaster, m_image.getRasterSize());
}

// header parsing
void			BMPFormat::parseDIBHeader_BITMAPCOREHEADER()
{
	m_image.m_vecSize.x = m_reader.ui16(); // todo - use m_reader.i16() and then type cast to ui16. // todo - in fact maybe store as ivec2 not uvec2, for native reading of bottom-to-top row Win BMPs.
	m_image.m_vecSize.y = m_reader.ui16(); // todo - use m_reader.i16() and then type cast to ui16. // todo - in fact maybe store as ivec2 not uvec2, for native reading of bottom-to-top row Win BMPs.
	m_meta.m_uiColourPlaneCount = m_reader.ui16();
	m_meta.m_uiBitPerPixelCount = m_reader.ui16();

	if (m_meta.m_uiColourPlaneCount != 1)
		throw new FormatException(INVALID_FORMAT);

	switch (m_meta.m_uiBitPerPixelCount)
	{
	case 0:
		// Win V2+: "The number of bits-per-pixel is specified or is implied by the JPEG or PNG format."
		if(m_meta.getWinBMPVersion() <= 1)
			throw new FormatException(INVALID_FORMAT);
		break;
	case 1:
	case 4:
	case 8:
	case 16:
		if (m_meta.getWinBMPVersion() <= 1)
			throw new FormatException(INVALID_FORMAT);
		break;
	case 24:
		break;
	case 32:
		if (m_meta.getWinBMPVersion() <= 1)
			throw new FormatException(INVALID_FORMAT);
		break;
	default:
		throw new FormatException(INVALID_FORMAT);
	}
}

void			BMPFormat::parseDIBHeader_OS21XBITMAPHEADER()
{
	m_image.m_vecSize.x = m_reader.ui16();
	m_image.m_vecSize.y = m_reader.ui16();
	m_meta.m_uiColourPlaneCount = m_reader.ui16();
	m_meta.m_uiBitPerPixelCount = m_reader.ui16();

	if (m_meta.m_uiColourPlaneCount != 1)
		throw new FormatException(INVALID_FORMAT);

	switch (m_meta.m_uiBitPerPixelCount)
	{
	case 1:
	case 4:
	case 8:
	case 24:
		break;
	default:
		throw new FormatException(INVALID_FORMAT);
	}
}

void			BMPFormat::parseDIBHeader_OS22XBITMAPHEADER_64()
{
	parseDIBHeader_OS21XBITMAPHEADER();

	m_meta.m_uiCompressionType = detectCompressionType(m_reader.ui32(), BMP_DIB_HEADER_TYPE_OS2);
	m_meta.m_uiDataSize = m_reader.ui32();
	m_meta.m_uiXResolution = m_reader.ui32();
	m_meta.m_uiYResolution = m_reader.ui32();
	m_meta.m_uiTableEntryCount = m_reader.ui32();
	m_meta.m_uiTableImportantEntryCount = m_reader.ui32();
	m_meta.m_uiUnitType = m_reader.ui16();
	m_meta.m_uiReserved1 = m_reader.ui16();
	m_meta.m_uiRecordingAlgorithm = m_reader.ui16();
	m_meta.m_uiHalftoningAlgorithm = m_reader.ui16();
	m_meta.m_uiHalftoningAlgorithmSize1 = m_reader.ui32();
	m_meta.m_uiHalftoningAlgorithmSize2 = m_reader.ui32();
	m_meta.m_uiColourEncoding = m_reader.ui32();
	m_meta.m_uiApplicationIdentifier = m_reader.ui32();

	if(m_meta.m_uiCompressionType == UNKNOWN_BMP_COMPRESSION_TYPE)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiDataSize == 0 && m_meta.m_uiCompressionType != BMP_COMPRESSION_RGB)
		throw new FormatException(INVALID_FORMAT);
	if(m_meta.m_uiDataSize != m_image.getRasterSize())
		throw new FormatException(INVALID_FORMAT);
	if((MX_BMP_HEADER1_SIZE + m_meta.getDIBHeaderSize() + m_meta.m_uiDataSize) != m_reader.m_stream.size())
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiTableEntryCount != 0 && m_meta.m_uiBitPerPixelCount >= 16)
		throw new FormatException(INVALID_FORMAT);
	if(m_meta.m_uiTableEntryCount == 0 && m_meta.m_uiBitPerPixelCount < 16)
		m_meta.m_uiTableEntryCount = 1 << m_meta.m_uiBitPerPixelCount;
	if(m_meta.m_uiTableEntryCount == 0 && m_meta.m_uiTableImportantEntryCount > 0)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiBitPerPixelCount >= 16 && m_meta.m_uiTableImportantEntryCount != 0)
		throw new FormatException(INVALID_FORMAT);
	
	if(m_meta.m_uiUnitType != 0)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiReserved1 != 0)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiRecordingAlgorithm != 0)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiHalftoningAlgorithm > 3)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiRecordingAlgorithm == 0 && m_meta.m_uiHalftoningAlgorithmSize1 != 0
	|| (m_meta.m_uiRecordingAlgorithm == 1 && m_meta.m_uiHalftoningAlgorithmSize1 > 100))
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiRecordingAlgorithm == 0 && m_meta.m_uiHalftoningAlgorithmSize2 != 0
	|| (m_meta.m_uiRecordingAlgorithm == 1 && m_meta.m_uiHalftoningAlgorithmSize2 != 0))
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiColourEncoding != 0)
		throw new FormatException(INVALID_FORMAT);
}

void			BMPFormat::parseDIBHeader_OS22XBITMAPHEADER_16()
{
	parseDIBHeader_OS21XBITMAPHEADER();

	// todo - verify that this header is correctly parsed.
}

void			BMPFormat::parseDIBHeader_BITMAPV2INFOHEADER()
{
	parseDIBHeader_BITMAPCOREHEADER();

	m_meta.m_uiCompressionType = detectCompressionType(m_reader.ui32(), BMP_DIB_HEADER_TYPE_WINDOWS);
	m_meta.m_uiDataSize = m_reader.ui32();
	m_meta.m_iXPelsPerMeter = m_reader.ui32(); // todo - use i32() not unsigned
	m_meta.m_iYPelsPerMeter = m_reader.ui32(); // todo - use i32() not unsigned
	m_meta.m_uiTableEntryCount = m_reader.ui32();
	m_meta.m_uiTableImportantEntryCount = m_reader.ui32();
	m_meta.m_uiRedMask = m_reader.ui32();
	m_meta.m_uiGreenMask = m_reader.ui32();
	m_meta.m_uiBlueMask = m_reader.ui32();

	if(m_meta.m_uiCompressionType == UNKNOWN_BMP_COMPRESSION_TYPE)
		throw new FormatException(INVALID_FORMAT);

	if(m_meta.m_uiDataSize == 0 && m_meta.m_uiCompressionType != BMP_COMPRESSION_RGB)
		throw new FormatException(INVALID_FORMAT);
	if(m_meta.m_uiDataSize != m_image.getRasterSize())
		throw new FormatException(INVALID_FORMAT);
	if((MX_BMP_HEADER1_SIZE + m_meta.getDIBHeaderSize() + m_meta.m_uiDataSize) != m_reader.m_stream.size())
		throw new FormatException(INVALID_FORMAT);

	//if(m_meta.m_uiTableEntryCount != 0 && m_meta.m_uiBitPerPixelCount >= 16)
		//"If biClrUsed is nonzero and the biBitCount member is less than 16, the biClrUsed member specifies the actual number of colors the graphics engine or device driver accesses"
	if(m_meta.m_uiTableEntryCount == 0 && m_meta.m_uiBitPerPixelCount < 16)
		m_meta.m_uiTableEntryCount = 1 << m_meta.m_uiBitPerPixelCount;
	if(m_meta.m_uiTableEntryCount == 0 && m_meta.m_uiTableImportantEntryCount > 0)
		throw new FormatException(INVALID_FORMAT);
	if(m_meta.m_uiTableEntryCount > (1 << m_meta.m_uiBitPerPixelCount)) // todo - use this in OS/2 format too?
		throw new FormatException(INVALID_FORMAT);

	if (m_meta.getWinBMPVersion() >= 4 && m_meta.m_uiCompressionType == BMP_COMPRESSION_BITFIELDS)
	{
		if(m_meta.m_uiRedMask != 0 || m_meta.m_uiGreenMask != 0 || m_meta.m_uiBlueMask != 0)
			throw new FormatException(INVALID_FORMAT);
	}
}

void			BMPFormat::parseDIBHeader_BITMAPV3INFOHEADER()
{
	parseDIBHeader_BITMAPV2INFOHEADER();

	m_meta.m_uiAlphaMask = m_reader.ui32();
}

void			BMPFormat::parseDIBHeader_BITMAPV4HEADER()
{
	parseDIBHeader_BITMAPV3INFOHEADER();

	m_meta.m_uiCSType = m_reader.ui32();
	m_reader.structure(m_meta.m_endpoints); // Note: e.g. endpoints.ciexyzRed.ciexyzX, which uses type FXPT2DOT30. (Which is fixed point, 2 bit integer, 30 bit fractional).
	m_meta.m_uiGammaRed = m_reader.ui32();
	m_meta.m_uiGammaGreen = m_reader.ui32();
	m_meta.m_uiGammaBlue = m_reader.ui32();

	uint32 uiWinBMPVersion = m_meta.getWinBMPVersion();
	if (uiWinBMPVersion == 4)
	{
		if (m_meta.m_uiCSType != LCS_CALIBRATED_RGB)
			throw new FormatException(INVALID_FORMAT);
	}
	else if (uiWinBMPVersion == 5)
	{
		switch (m_meta.m_uiCSType)
		{
		case LCS_CALIBRATED_RGB:
		case LCS_sRGB:
		case LCS_WINDOWS_COLOR_SPACE:
		case PROFILE_LINKED:
		case PROFILE_EMBEDDED:
			break;
		default:
			throw new FormatException(INVALID_FORMAT);
		}
	}

	if (m_meta.m_uiCSType != LCS_CALIBRATED_RGB)
	{
		m_meta.m_uiGammaRed = 0;
		m_meta.m_uiGammaGreen = 0;
		m_meta.m_uiGammaBlue = 0;
	}
}

void			BMPFormat::parseDIBHeader_BITMAPV5HEADER()
{
	parseDIBHeader_BITMAPV4HEADER();

	m_meta.m_uiIntent = m_reader.ui32();
	m_meta.m_uiProfileDataOffset = m_reader.ui32();
	m_meta.m_uiProfileDataSize = m_reader.ui32();
	m_meta.m_uiReserved2 = m_reader.ui32();

	switch (m_meta.m_uiIntent)
	{
	case LCS_GM_ABS_COLORIMETRIC:
	case LCS_GM_BUSINESS:
	case LCS_GM_GRAPHICS:
	case LCS_GM_IMAGES:
		break;
	default:
		throw new FormatException(INVALID_FORMAT);
	}

	if (m_meta.m_uiCSType != PROFILE_LINKED && m_meta.m_uiCSType != PROFILE_EMBEDDED)
	{
		m_meta.m_uiProfileDataOffset = 0;
		m_meta.m_uiProfileDataSize = 0;
	}
	else
	{
		if (m_meta.m_uiProfileDataSize > MAX_PATH)
			throw new FormatException(INVALID_FORMAT);

		if ((MX_BMP_HEADER1_SIZE + m_meta.m_uiProfileDataOffset + m_meta.m_uiProfileDataSize) >= m_reader.m_stream.size())
			throw new FormatException(INVALID_FORMAT);

		char szProfileData[MAX_PATH];
		memset(szProfileData, '\0', MAX_PATH);
		m_reader.seek(MX_BMP_HEADER1_SIZE + m_meta.m_uiProfileDataOffset);
		m_reader.cstr(szProfileData, m_meta.m_uiProfileDataSize);
		string strProfileData(szProfileData, m_meta.m_uiProfileDataSize);
		if (!String::isCodePage1252(strProfileData))
			throw new FormatException(INVALID_FORMAT);
	}

	if (m_meta.m_uiReserved2 != 0)
		throw new FormatException(INVALID_FORMAT);
}

// storing
void							BMPFormat::storeFileHeader()
{
	m_writer.ui16(getTwoCCByFileType(m_meta.m_uiFileType));
	m_writer.ui32(computeFileSize());
	m_writer.ui32(0); // reserved, "if created manually can be 0"
	m_writer.ui32(0); // reserved, "if created manually can be 0"
	m_writer.ui32(computeRasterOffset());
}

void							BMPFormat::storeDIBHeader()
{
	switch (m_meta.m_uiDIBHeader)
	{
	case BMP_DIB_HEADER_BITMAPCOREHEADER:		return storeDIBHeader_BITMAPCOREHEADER();
	case BMP_DIB_HEADER_OS21XBITMAPHEADER:		return storeDIBHeader_OS21XBITMAPHEADER();
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_64:	return storeDIBHeader_OS22XBITMAPHEADER_64();
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_16:	return storeDIBHeader_OS22XBITMAPHEADER_16();
	case BMP_DIB_HEADER_BITMAPV2INFOHEADER:		return storeDIBHeader_BITMAPV2INFOHEADER();
	case BMP_DIB_HEADER_BITMAPV3INFOHEADER:		return storeDIBHeader_BITMAPV3INFOHEADER();
	case BMP_DIB_HEADER_BITMAPV4HEADER:			return storeDIBHeader_BITMAPV4HEADER();
	case BMP_DIB_HEADER_BITMAPV5HEADER:			return storeDIBHeader_BITMAPV5HEADER();
	}
}

void							BMPFormat::storeTable()
{
	if (!m_image.m_pTable)
		return;

	m_writer.cstr((char*)m_image.m_pTable, m_image.getTableSize());
}

void							BMPFormat::storeRaster()
{
	m_writer.cstr((char*)m_image.m_pRaster, m_image.getRasterSize());
}

// header storing
void							BMPFormat::storeDIBHeader_BITMAPCOREHEADER()
{
	m_writer.ui16(m_image.m_vecSize.x); // todo - use m_writer.i16() and then type cast from ui16. // todo - in fact maybe store as ivec2 not uvec2, for native reading of bottom-to-top row Win BMPs.
	m_writer.ui16(m_image.m_vecSize.y); // todo - use m_writer.i16() and then type cast from ui16. // todo - in fact maybe store as ivec2 not uvec2, for native reading of bottom-to-top row Win BMPs.
	m_writer.ui16(m_meta.m_uiColourPlaneCount);
	m_writer.ui16(m_meta.m_uiBitPerPixelCount);
}

void							BMPFormat::storeDIBHeader_OS21XBITMAPHEADER()
{
	m_writer.ui16(m_image.m_vecSize.x);
	m_writer.ui16(m_image.m_vecSize.y);
	m_writer.ui16(m_meta.m_uiColourPlaneCount);
	m_writer.ui16(m_meta.m_uiBitPerPixelCount);
}

void							BMPFormat::storeDIBHeader_OS22XBITMAPHEADER_64()
{
	storeDIBHeader_OS21XBITMAPHEADER();

	m_writer.ui32(m_meta.m_uiCompressionType);
	m_writer.ui32(m_meta.m_uiDataSize); // todo - compute instead ?
	m_writer.ui32(m_meta.m_uiXResolution);
	m_writer.ui32(m_meta.m_uiYResolution);
	m_writer.ui32(m_meta.m_uiTableEntryCount);
	m_writer.ui32(m_meta.m_uiTableImportantEntryCount);
	m_writer.ui16(m_meta.m_uiUnitType);
	m_writer.ui16(m_meta.m_uiReserved1);
	m_writer.ui16(m_meta.m_uiRecordingAlgorithm);
	m_writer.ui16(m_meta.m_uiHalftoningAlgorithm);
	m_writer.ui32(m_meta.m_uiHalftoningAlgorithmSize1);
	m_writer.ui32(m_meta.m_uiHalftoningAlgorithmSize2);
	m_writer.ui32(m_meta.m_uiColourEncoding);
	m_writer.ui32(m_meta.m_uiApplicationIdentifier);
}

void							BMPFormat::storeDIBHeader_OS22XBITMAPHEADER_16()
{
	storeDIBHeader_OS21XBITMAPHEADER();

	// todo - verify that this header is correctly stored.
}

void							BMPFormat::storeDIBHeader_BITMAPV2INFOHEADER()
{
	storeDIBHeader_BITMAPCOREHEADER();

	m_writer.ui32(m_meta.m_uiCompressionType);
	m_writer.ui32(m_meta.m_uiDataSize); // todo - compute instead ?
	m_writer.ui32(m_meta.m_iXPelsPerMeter); // todo - use i32() not unsigned
	m_writer.ui32(m_meta.m_iYPelsPerMeter); // todo - use i32() not unsigned
	m_writer.ui32(m_meta.m_uiTableEntryCount);
	m_writer.ui32(m_meta.m_uiTableImportantEntryCount);
	m_writer.ui32(m_meta.m_uiRedMask);
	m_writer.ui32(m_meta.m_uiGreenMask);
	m_writer.ui32(m_meta.m_uiBlueMask);
}

void							BMPFormat::storeDIBHeader_BITMAPV3INFOHEADER()
{
	storeDIBHeader_BITMAPV2INFOHEADER();

	m_writer.ui32(m_meta.m_uiAlphaMask);
}

void							BMPFormat::storeDIBHeader_BITMAPV4HEADER()
{
	storeDIBHeader_BITMAPV3INFOHEADER();

	m_writer.ui32(m_meta.m_uiCSType);
	m_writer.structure(m_meta.m_endpoints); // Note: e.g. endpoints.ciexyzRed.ciexyzX, which uses type FXPT2DOT30. (Which is fixed point, 2 bit integer, 30 bit fractional).
	m_writer.ui32(m_meta.m_uiGammaRed);
	m_writer.ui32(m_meta.m_uiGammaGreen);
	m_writer.ui32(m_meta.m_uiGammaBlue);
}

void							BMPFormat::storeDIBHeader_BITMAPV5HEADER()
{
	storeDIBHeader_BITMAPV4HEADER();

	m_writer.ui32(m_meta.m_uiIntent);
	m_writer.ui32(m_meta.m_uiProfileDataOffset); // todo - compute the offset.
	m_writer.ui32(m_meta.m_uiProfileDataSize);
	m_writer.ui32(m_meta.m_uiReserved2);

	// todo - also store the profile data into the stream.
}