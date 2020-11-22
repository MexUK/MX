#pragma once

#include "mx.h"
#include "Static/Components/EImageFormat.h"

class mx::ImageData
{
public:
	mx::EImageFormat	m_uiFormat;
	glm::uvec2			m_vecSize;
	uint8*				m_pData;

public:
	ImageData();
	ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pData);
	~ImageData();
	
	uint32				getBitsPerPixel();
	uint32				getChannelCount();
	uint32				getDataSize();
};