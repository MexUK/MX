#pragma once

#include "mx.h"
#include "Image/ERasterDataFormat.h"
#include <string>

class mx::RasterDataFormat
{
public:
	void						unload(void) {}

	void						setRasterDataFormatId(ERasterDataFormat ERasterDataFormatValue) { m_uiRasterDataFormatId = ERasterDataFormatValue; }
	ERasterDataFormat			getRasterDataFormatId(void) { return m_uiRasterDataFormatId; }

	void						setLocalizationKey(std::string& strLocalizationKey) { m_strLocalizationKey = strLocalizationKey; }
	std::string					getLocalizationKey(void) { return m_strLocalizationKey; }

	void						setText(std::string& strText) { m_strText = strText; }
	std::string					getText(void) { return m_strText; }
	std::string					getTextForMenu(void);

private:
	ERasterDataFormat			m_uiRasterDataFormatId;
	std::string					m_strText;
	std::string					m_strLocalizationKey;		// e.g. TextureFormat_1
};