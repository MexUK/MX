#pragma once

#include "mx.h"
#include "Format/JSON/JSONContainer.h"
#include "Format/JSON/JSONString.h"
#include "Format/JSON/JSONValue.h"

class mx::JSONObject : public mx::JSONContainer
{
public:
	JSONObject(void) { m_uiType = JSON_OBJECT; }

	void											addValue(mx::JSONString *pKey, mx::JSONValue *pValue)
	{
		m_umapValues[pKey] = pValue;

		if (pValue->m_uiType == JSON_ARRAY || pValue->m_uiType == JSON_OBJECT)
		{
			static_cast<mx::JSONContainer*>(pValue)->m_pParentContainer = this;
		}
	}

	void											removeValues(void)
	{
		for (auto& it : m_umapValues)
		{
			delete it.first;
			if (it.second->m_uiType == JSON_ARRAY || it.second->m_uiType == JSON_OBJECT)
			{
				it.second->removeValues();
			}
			delete it.second;
		}
	}

	std::unordered_map<mx::JSONString*, mx::JSONValue*>	m_umapValues;
};