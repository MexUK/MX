#pragma once

#include "mx.h"
#include "Static/Components/EImageFormat.h"

class mx::ImageData
{
public:
	mx::EImageFormat	m_uiFormat;
	glm::uvec2			m_vecSize;
	unsigned char*		m_pData;

public:
	ImageData();
	~ImageData();
	
	uint32				getBitsPerPixel();
	uint32				getChannelCount();
	uint32				getDataSize();
};