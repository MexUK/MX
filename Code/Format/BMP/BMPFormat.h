#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Static/Components/ImageData.h"
#include "Format/Format.h"
#include "Format/BMP/BMPMeta.h"

#define MX_BMP_HEADER1_SIZE				14

class mx::BMPFormat : public mx::Format
{
public:
	mx::ImageData					m_image;
	mx::BMPMeta						m_meta;
	uint8							m_bSerializeHeader1	: 1;
	uint8							m_uiSerializeBMPVersion;

public:
	BMPFormat(mx::Stream& stream);
	BMPFormat(mx::Stream& stream, mx::ImageData& image);

	void							_unserialize(void);
	void							_serialize(void);

	void							setImage(mx::ImageData& image) { m_image = image; }
	mx::ImageData&					getImage(void) { return m_image; }

	EImageFormat					getRasterDataFormat(void) { return m_image.m_uiFormat; }

	void							setWidth(uint32 uiWidth) { m_image.m_vecSize.x = uiWidth; }
	uint32							getWidth(void) { return m_image.m_vecSize.x; }

	void							setHeight(uint32 uiHeight) { m_image.m_vecSize.y = uiHeight; }
	uint32							getHeight(void) { return m_image.m_vecSize.y; }

	void							setRasterData(uint8* pRaster) { m_image.m_pRaster = pRaster; }
	uint8*							getRasterData(void) { return m_image.m_pRaster; }

	uint16							getBPP(void) { return m_image.getBitsPerPixel(); }

	void							setShouldSerializeHeader1(bool bSerializeHeader1) { m_bSerializeHeader1 = bSerializeHeader1; }
	bool							shouldSerializeHeader1(void) { return m_bSerializeHeader1; }

	void							setSerializeBMPVersion(uint8 uiSerializeBMPVersion) { m_uiSerializeBMPVersion = uiSerializeBMPVersion; }
	uint8							getSerializeBMPVersion(void) { return m_uiSerializeBMPVersion; }

private:
	static EBMPFileType				getFileTypeByTwoCC(uint16 uiTwoCC);
	static uint16					getTwoCCByFileType(EBMPFileType uiBMPFileType);
	static EBMPDIBHeaderType		getDIBHeaderTypeByFileType(EBMPFileType uiBMPFileType);

	EBMPDIBHeaderType				detectFileHeaderTwoCC();
	EBMPDIBHeader					detectDIBHeader(EBMPDIBHeaderType uiDIBHeaderType);
	EBMPCompressionType				detectCompressionType(uint32 uiCompressionType, EBMPDIBHeaderType uiDIBHeaderType);
	bool							detectTablePresence();

	uint32							computeFileSize();
	uint32							computeTableSize();
	uint32							computeTableEntryByteCount();
	mx::EImageFormat				computeRasterFormat();
	uint32							computeRasterOffset();

	void							parseFileHeader();
	void							parseDIBHeader();
	void							parseTable();
	void							parseRaster();

	void							parseDIBHeader_BITMAPCOREHEADER();
	void							parseDIBHeader_OS21XBITMAPHEADER();
	void							parseDIBHeader_OS22XBITMAPHEADER_64();
	void							parseDIBHeader_OS22XBITMAPHEADER_16();
	void							parseDIBHeader_BITMAPV2INFOHEADER();
	void							parseDIBHeader_BITMAPV3INFOHEADER();
	void							parseDIBHeader_BITMAPV4HEADER();
	void							parseDIBHeader_BITMAPV5HEADER();

	void							storeFileHeader();
	void							storeDIBHeader();
	void							storeTable();
	void							storeRaster();

	void							storeDIBHeader_BITMAPCOREHEADER();
	void							storeDIBHeader_OS21XBITMAPHEADER();
	void							storeDIBHeader_OS22XBITMAPHEADER_64();
	void							storeDIBHeader_OS22XBITMAPHEADER_16();
	void							storeDIBHeader_BITMAPV2INFOHEADER();
	void							storeDIBHeader_BITMAPV3INFOHEADER();
	void							storeDIBHeader_BITMAPV4HEADER();
	void							storeDIBHeader_BITMAPV5HEADER();
};