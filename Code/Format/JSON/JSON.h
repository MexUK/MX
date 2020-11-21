#pragma once

#include "mx.h"
#include <vector>
#include <unordered_map>

class mx::JSON
{
public:
	JSON(void);

	mx::JSONObject*				parse(std::string& strJSON);

private:
	std::string					parseString(std::string& strJSON);
	float64						parseNumber(std::string& strJSON);

	void						setLatestKey(mx::JSONString *pString) { m_pLatestKey = pString; }
	mx::JSONString*				getLatestKey(void) { return m_pLatestKey; }

	mx::JSONObject*				m_pEntryContainer;
	mx::JSONContainer*			m_pLatestContainer;
	bool						m_bExpectingKeyOrObjectEnd;
	mx::JSONString*				m_pLatestKey;
	uint32						m_uiSeek;
};