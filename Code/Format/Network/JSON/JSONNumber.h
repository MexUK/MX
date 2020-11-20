#pragma once

#include "mx.h"
#include "Format/Network/JSON/JSONValue.h"

class mx::JSONNumber : public mx::JSONValue
{
public:
	JSONNumber(void) { m_uiType = JSON_NUMBER; }
	double											m_dValue;
};