#pragma once

#include "mx.h"
#include "Format/Network/JSON/JSONValue.h"

class mx::JSONBoolean : public mx::JSONValue
{
public:
	JSONBoolean(void) { m_uiType = JSON_BOOLEAN; }
	bool											m_bValue;
};