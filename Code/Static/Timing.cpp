#include "Timing.h"
#include "Static/String.h"
#include <chrono>

using namespace std;
using namespace chrono;
using namespace mx;

void							Timing::start(string& strTimingName)
{
	m_uiTimeStart = pollTime();
	m_strTimingName = strTimingName;
}

void							Timing::stop(void)
{
	uint32 uiTimeEnd = pollTime();
	uint32 uiTimeLength = uiTimeEnd - m_uiTimeStart;
	
	m_umapTimings[m_strTimingName].push_back(uiTimeLength);

	m_uiTimeStart = 0;
	m_strTimingName = "";
}

uint32							Timing::pollTime(void)
{
	return (int32) duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

string							Timing::joinTimings(vector<uint32>& vecTimings)
{
	string strTimings = "";
	for (uint32 i = 0, j = vecTimings.size(); i < j; i++)
	{
		strTimings += String::toString(vecTimings[i]);
		if (i != (j - 1))
		{
			strTimings += " ";
		}
	}
	return strTimings;
}




void							Timing::log(string& strTimingName)
{
	uint32 uiTimeEnd = pollTime();
	uint32 uiTimeLength = uiTimeEnd - m_uiTimeStart;

	m_umapTimings[m_strTimingName].push_back(uiTimeLength);
}