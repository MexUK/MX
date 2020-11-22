#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Format/Format.h"
#include "Static/Components/ImageData.h"

class mx::BMPFormat : public mx::Format
{
public:
	mx::ImageData					m_image;
	uint8							m_bSkipBMPFileHeaderForSerialize	: 1;
	uint8							m_bHasPalette						: 1;
	uint8							m_uiBMPVersion;
	uint16							m_usColourPlaneCount;
	std::string						m_strPaletteData;

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

	void							setSkipBMPFileHeaderForSerialize(bool bSkipBMPFileHeaderForSerialize) { m_bSkipBMPFileHeaderForSerialize = bSkipBMPFileHeaderForSerialize; }
	bool							getSkipBMPFileHeaderForSerialize(void) { return m_bSkipBMPFileHeaderForSerialize; }

	void							setBMPVersion(uint8 uiBMPVersion) { m_uiBMPVersion = uiBMPVersion; }
	uint8							getBMPVersion(void) { return m_uiBMPVersion; }

	void							setColourPlaneCount(uint16 usColourPlaneCount) { m_usColourPlaneCount = usColourPlaneCount; }
	uint16							getColourPlaneCount(void) { return m_usColourPlaneCount; }

	void							setHasPalette(bool bHasPalette) { m_bHasPalette = bHasPalette; }
	bool							doesHavePalette(void) { return m_bHasPalette; }

	void							setPaletteData(std::string& strPaletteData) { m_strPaletteData = strPaletteData; }
	std::string&					getPaletteData(void) { return m_strPaletteData; }

private:
	void							unserializeVersion1(void);
	void							unserializeVersion2(void);
	void							unserializeVersion3(void);
	void							unserializeVersion4(void);

	void							serializeVersion1(void);
	void							serializeVersion2(void);
	void							serializeVersion3(void);
	void							serializeVersion4(void);

	uint8							detectBMPVersion(void);
};