#pragma once

#include "mx.h"
#include "Types.h"
#include "Object/Singleton.h"
#include <string>
#include <map>
#include <vector>

class mx::Timing : public mx::Singleton<Timing>
{
public:
	void										start(std::string& strTimingName);
	void										stop(void);

	std::map<std::string, std::vector<uint32>>&	getTimings(void) { return m_umapTimings; }

	std::string									joinTimings(std::vector<uint32>& vecTimings);



	void										log(std::string& strTimingName);

private:
	uint32										pollTime(void);

private:
	uint32										m_uiTimeStart;
	std::map<std::string, std::vector<uint32>>	m_umapTimings;
	std::string									m_strTimingName;
};