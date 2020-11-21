#pragma once

#include "mx.h"
#include "Format/Format.h"
#include <string>

class mx::DDSFormat : public mx::Format
{
public:
	DDSFormat(mx::Stream& stream);

private:
	void								_unserialize(void);
	void								_serialize(void);
	
public:
	uint32								m_uiWidth;
	uint32								m_uiHeight;
	uint32								m_uiBPP;
	std::string							m_strRasterData;
};