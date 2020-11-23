#pragma once

#include "mx.h"
#include "Static/Components/EImageFormat.h"

class mx::ImageData
{
public:
	mx::EImageFormat	m_uiFormat;
	glm::uvec2			m_vecSize;
	uint8*				m_pData;
	uint8*				m_pTable;

public:
	ImageData();
	ImageData(mx::EImageFormat uiFormat, glm::uvec2& vecSize, uint8* pData);
	
	void				allocateData();
	void				deallocateData();
	void				checkToAllocateData();

	void				allocateTable();
	void				deallocateTable();
	void				checkToAllocateTable();
	bool				doesHaveTable();

	uint32				getBitsPerPixel();
	uint32				getChannelCount();
	uint32				getDataSize(); // in bytes
	//bool				doesHaveTable();
	uint32				getTableColourCount();
	uint32				getTableSize(); // in bytes
};