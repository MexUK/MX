#pragma once

#include "mx.h"
#include "Format/Network/JSON/JSONValue.h"

class mx::JSONNull : public mx::JSONValue
{
public:
	JSONNull(void) { m_uiType = JSON_NULL; }
};