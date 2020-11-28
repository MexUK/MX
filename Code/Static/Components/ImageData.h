#pragma once

#include "mx.h"
#include "Static/Components/EImageFormat.h"

class mx::ImageData
{
public:
	mx::EImageFormat	m_uiFormat;
	glm::uvec2			m_vecSize;
	uint8*				m_pRaster;
	uint8*				m_pTable;

public:
	ImageData();
	ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pRaster, uint8* pTable = nullptr);
	
	void				allocateRaster();
	void				deallocateRaster();
	void				checkToAllocateRaster();

	void				allocateTable();
	void				deallocateTable();
	void				checkToAllocateTable();
	bool				doesHaveTable();

	uint32				getBitsPerPixel();
	uint32				getChannelCount();
	uint32				getRasterSize(); // in bytes
	uint32				getTableColourCount();
	uint32				getTableSize(); // in bytes

	void				convertToFormat(mx::EImageFormat uiImageFormat);

	void				convertUncompressedRGBToUncompressedRGBA();
};