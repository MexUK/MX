#pragma once

#include "mx.h"
#include <vector>
#include <unordered_map>

class mx::JSONObject;
class mx::JSONContainer;
class mx::JSONString;

class mx::JSONManager
{
public:
	JSONManager(void);

	mx::JSONObject*				unserializeString(std::string& strJSON);

	//void						loadObjectIntoLua(JSONObject *pObject);

private:
	std::string					parseString(std::string& strJSON);
	float64						parseNumber(std::string& strJSON);

	void						setLatestKey(mx::JSONString *pString) { m_pLatestKey = pString; }
	mx::JSONString*				getLatestKey(void) { return m_pLatestKey; }

	//void						loadArrayIntoLua(JSONArray *pArray);
	//void						loadValueIntoLua(JSONValue *pValue);

	mx::JSONObject*				m_pEntryContainer;
	mx::JSONContainer*			m_pLatestContainer;
	bool						m_bExpectingKeyOrObjectEnd;
	mx::JSONString*				m_pLatestKey;
	uint32						m_uiSeek;
};