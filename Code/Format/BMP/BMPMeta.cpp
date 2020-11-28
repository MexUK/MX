#include "BMPMeta.h"

using namespace mx;

BMPMeta::BMPMeta() :
	m_uiFileType(UNKNOWN_BMP_FILE_TYPE),
	m_uiDIBHeader(UNKNOWN_BMP_DIB_HEADER),

	m_uiColourPlaneCount(0),
	m_uiBitPerPixelCount(0),

	m_uiCompressionType(UNKNOWN_BMP_COMPRESSION_TYPE),
	m_uiDataSize(0),
	m_uiXResolution(0),
	m_uiYResolution(0),
	m_uiTableEntryCount(0),
	m_uiTableImportantEntryCount(0),
	m_uiUnitType(0),
	m_uiReserved1(0),
	m_uiRecordingAlgorithm(0),
	m_uiHalftoningAlgorithm(0),
	m_uiHalftoningAlgorithmSize1(0),
	m_uiHalftoningAlgorithmSize2(0),
	m_uiColourEncoding(0),
	m_uiApplicationIdentifier(0),

	m_iXPelsPerMeter(0),
	m_iYPelsPerMeter(0),
	m_uiRedMask(0),
	m_uiGreenMask(0),
	m_uiBlueMask(0),

	m_uiAlphaMask(0),

	m_uiCSType(0),
	m_endpoints(mx::g_blankCEXYZTRIPLE),
	m_uiGammaRed(0),
	m_uiGammaGreen(0),
	m_uiGammaBlue(0),

	m_uiIntent(0),
	m_uiProfileDataOffset(0),
	m_uiProfileDataSize(0),
	m_uiReserved2(0)
{
}

EBMPDIBHeaderType				BMPMeta::getDIBHeaderType()
{
	switch (m_uiDIBHeader)
	{
	case BMP_DIB_HEADER_BITMAPCOREHEADER:		return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_DIB_HEADER_OS21XBITMAPHEADER:		return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_64:	return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_16:	return BMP_DIB_HEADER_TYPE_OS2;
	case BMP_DIB_HEADER_BITMAPINFOHEADER:		return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_DIB_HEADER_BITMAPV2INFOHEADER:		return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_DIB_HEADER_BITMAPV3INFOHEADER:		return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_DIB_HEADER_BITMAPV4HEADER:			return BMP_DIB_HEADER_TYPE_WINDOWS;
	case BMP_DIB_HEADER_BITMAPV5HEADER:			return BMP_DIB_HEADER_TYPE_WINDOWS;
	default:									return UNKNOWN_BMP_DIB_HEADER_TYPE;
	}
}

uint32							BMPMeta::getDIBHeaderSize()
{
	switch (m_uiDIBHeader)
	{
	case BMP_DIB_HEADER_BITMAPCOREHEADER:		return 12;
	case BMP_DIB_HEADER_OS21XBITMAPHEADER:		return 12;
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_64:	return 64;
	case BMP_DIB_HEADER_OS22XBITMAPHEADER_16:	return 16;
	case BMP_DIB_HEADER_BITMAPINFOHEADER:		return 40;
	case BMP_DIB_HEADER_BITMAPV2INFOHEADER:		return 52;
	case BMP_DIB_HEADER_BITMAPV3INFOHEADER:		return 56;
	case BMP_DIB_HEADER_BITMAPV4HEADER:			return 108;
	case BMP_DIB_HEADER_BITMAPV5HEADER:			return 124;
	default:									return 0;
	}
}

uint32							BMPMeta::getWinBMPVersion()
{
	switch (m_uiDIBHeader)
	{
	case BMP_DIB_HEADER_BITMAPCOREHEADER:		return 1;
	case BMP_DIB_HEADER_BITMAPINFOHEADER:		return 1;
	case BMP_DIB_HEADER_BITMAPV2INFOHEADER:		return 2;
	case BMP_DIB_HEADER_BITMAPV3INFOHEADER:		return 3;
	case BMP_DIB_HEADER_BITMAPV4HEADER:			return 4;
	case BMP_DIB_HEADER_BITMAPV5HEADER:			return 5;
	default:									return 0;
	}
}