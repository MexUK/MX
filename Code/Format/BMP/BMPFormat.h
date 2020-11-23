#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Format/Format.h"
#include "Static/Components/ImageData.h"

#define MX_BMP_HEADER1_SIZE				14
#define MX_BMP_HEADER2_VERSION2_SIZE	16
#define MX_BMP_HEADER2_VERSION3_SIZE	40
#define MX_BMP_HEADER2_VERSION4_SIZE	108

class mx::BMPFormat : public mx::Format
{
public:
	mx::ImageData					m_image;
	uint8							m_bSerializeHeader1	: 1;
	uint8							m_uiBMPVersion;
	uint16							m_usColourPlaneCount;

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

	void							setRasterData(uint8* pRasterData) { m_image.m_pData = pRasterData; }
	uint8*							getRasterData(void) { return m_image.m_pData; }

	uint16							getBPP(void) { return m_image.getBitsPerPixel(); }

	void							setShouldSerializeHeader1(bool bSerializeHeader1) { m_bSerializeHeader1 = bSerializeHeader1; }
	bool							shouldSerializeHeader1(void) { return m_bSerializeHeader1; }

	void							setBMPVersion(uint8 uiBMPVersion) { m_uiBMPVersion = uiBMPVersion; }
	uint8							getBMPVersion(void) { return m_uiBMPVersion; }

	void							setColourPlaneCount(uint16 usColourPlaneCount) { m_usColourPlaneCount = usColourPlaneCount; }
	uint16							getColourPlaneCount(void) { return m_usColourPlaneCount; }

private:
	void							unserializeVersion1(void);
	void							unserializeVersion2(void);
	void							unserializeVersion3(void);
	void							unserializeVersion4(void);

	void							serializeVersion1(void);
	void							serializeVersion2(void);
	void							serializeVersion3(void);
	void							serializeVersion4(void);

	uint32							detectBMPVersion(void);
	uint32							getBMPHeader2Size(uint32 uiBMPVersion);

	void							serializeBMPHeader1(uint32 uiBMPVersion);
	void							serializeTableAndImageData();

	void							populateImageData(uint32 uiWidth, uint32 uiHeight, uint32 uiBitsPerPixel);

	mx::EImageFormat				computeImageFormat(uint16 uiBitsPerPixel);
	bool							computeTablePresence(uint16 uiBitsPerPixel);
};