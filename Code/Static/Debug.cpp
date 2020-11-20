#include "Debug.h"
#include "Static/File.h"
#include "Static/DataPath.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

uint32_t		Debug::m_uiDebugLevel		= DEBUG_NONE;
std::string		Debug::m_strDebugFilePath	= "Debug.txt";

// debug settings
void		Debug::setDebugLevel(uint32_t uiDebugLevel)
{
	m_uiDebugLevel = uiDebugLevel;
}

void		Debug::setDebugFilePath(string strDebugFilePath)
{
	m_strDebugFilePath = strDebugFilePath;
}

// log type
string		Debug::getLogTypeName(uint32_t uiLogType)
{
	switch (uiLogType)
	{
	case DEBUG_LAUNCH:		return "LAUNCH";
	case DEBUG_MAIN:		return "MAIN";
	case DEBUG_NET_SCRIPT:	return "NET-SCRIPT";
	case DEBUG_INPUT:		return "INPUT";
	case DEBUG_RENDER:		return "RENDER";
	case DEBUG_HOOK:		return "HOOK";
	case DEBUG_NETWORK:		return "NETWORK";
	case DEBUG_GAME_LOG:	return "GAME-LOG";
	case DEBUG_CLOSE:		return "CLOSE";
	default:				return "UNKNOWN-LOG-TYPE("+String::toString(uiLogType)+")";
	}
}

// clear file
void		Debug::clearLog()
{
	File::remove(m_strDebugFilePath);
}

// log to file - internal
void		Debug::_log(uint32_t uiLogType, char* pText)
{
	if ((m_uiDebugLevel & uiLogType) != uiLogType)
		return;

	string strData = "[" + getLogTypeName(uiLogType) + "] [THREAD " + String::toString(GetCurrentThreadId()) + "] [" + String::getTimestampText() + "] " + pText + "\n";
	File::appendText(m_strDebugFilePath, strData);
}

// log to file - debug types
void		Debug::logLaunch(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_LAUNCH, szBuffer);
}

void		Debug::logMain(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_MAIN, szBuffer);
}

void		Debug::logNetScript(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_NET_SCRIPT, szBuffer);
}

void		Debug::logInput(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_INPUT, szBuffer);
}

void		Debug::logRender(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_RENDER, szBuffer);
}

void		Debug::logHook(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_HOOK, szBuffer);
}

void		Debug::logNetwork(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_NETWORK, szBuffer);
}

void		Debug::logGameLog(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_GAME_LOG, szBuffer);
}

void		Debug::logClose(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_CLOSE, szBuffer);
}

// log to file - useful
void		Debug::log(char* pFormat, ...)
{
	DEBUG_LOG_CALLS
	_log(DEBUG_MAIN, szBuffer);
}

void		Debug::log(std::string str)
{
	_log(DEBUG_MAIN, (char*)str.c_str());
}

void		Debug::log(glm::vec2 vec)
{
	string str = String::toString(vec.x) + " " + String::toString(vec.y);
	_log(DEBUG_MAIN, (char*)str.c_str());
}

void		Debug::log(glm::vec3 vec)
{
	string str = String::toString(vec.x) + " " + String::toString(vec.y) + " " + String::toString(vec.z);
	_log(DEBUG_MAIN, (char*)str.c_str());
}