#pragma once

#include "mx.h"
#include "Format/JSON/EJSONType.h"

class mx::JSONString;
class mx::JSONValue;

class mx::JSONValue
{
public:
	virtual void									addValue(mx::JSONString *pKey, mx::JSONValue *pValue) {}
	virtual void									removeValues(void) {}

	mx::EJSONType									m_uiType;
};