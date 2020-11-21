#pragma once

#include "mx.h"
#include "Format/JSON/JSONValue.h"

class mx::JSONString : public mx::JSONValue
{
public:
	JSONString(void) { m_uiType = JSON_STRING; }
	std::string										m_strValue;
};