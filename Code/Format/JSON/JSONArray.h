#pragma once

#include "mx.h"
#include "Format/JSON/JSONContainer.h"
#include "Format/JSON/JSONString.h"
#include "Format/JSON/JSONValue.h"

class mx::JSONArray : public mx::JSONContainer
{
public:
	JSONArray(void) { m_uiType = JSON_ARRAY; }

	void											addValue(mx::JSONString *pNull, mx::JSONValue *pValue)
	{
		m_vecValues.push_back(pValue);

		if (pValue->m_uiType == JSON_ARRAY || pValue->m_uiType == JSON_OBJECT)
		{
			static_cast<mx::JSONContainer*>(pValue)->m_pParentContainer = this;
		}
	}

	void											removeValues(void)
	{
		for (mx::JSONValue *pValue : m_vecValues)
		{
			if (pValue->m_uiType == JSON_ARRAY || pValue->m_uiType == JSON_OBJECT)
			{
				pValue->removeValues();
			}
			delete pValue;
		}
	}

	std::vector<mx::JSONValue*>						m_vecValues;
};