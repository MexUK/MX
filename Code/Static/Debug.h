#pragma once

#include "mx.h"
#include <string>

/*
* TODO
* Rename log type DEBUG_LAUNCH to DEBUG_LOAD
* Rename log type DEBUG_CLOSE to DEBUG_UNLOAD
* Add log type DEBUG_SCRIPTING
* Add log type DEBUG_LAUNCHER
*/

#define DEBUG_LAUNCH		1
#define DEBUG_MAIN			2
#define DEBUG_NET_SCRIPT	4
#define DEBUG_INPUT			8
#define DEBUG_RENDER		16
#define DEBUG_HOOK			32
#define DEBUG_NETWORK		64
#define DEBUG_GAME_LOG		128
#define DEBUG_CLOSE			256

#define DEBUG_NONE			0
#define DEBUG_MIN			DEBUG_MAIN
#define DEBUG_ALL			DEBUG_LAUNCH | DEBUG_MAIN | DEBUG_NET_SCRIPT | DEBUG_INPUT | DEBUG_RENDER | DEBUG_HOOK | DEBUG_NETWORK | DEBUG_GAME_LOG | DEBUG_CLOSE

#define DEBUG_BUFFER_SIZE	65536

#define DEBUG_LOG_CALLS		va_list args;\
							char szBuffer[DEBUG_BUFFER_SIZE];\
							va_start(args, pFormat);\
							vsprintf_s(szBuffer, sizeof(szBuffer), pFormat, args);\
							va_end(args);

class mx::Debug
{
public:
	static void			setDebugLevel(uint32_t uiDebugLevel);
	static void			setDebugFilePath(std::string strDebugFilePath);

	static std::string	getLogTypeName(uint32_t uiLogType);

	static void			clearLog(void);

	static void			logLaunch(char* pFormat, ...);
	static void			logMain(char* pFormat, ...);
	static void			logNetScript(char* pFormat, ...);
	static void			logInput(char* pFormat, ...);
	static void			logRender(char* pFormat, ...);
	static void			logHook(char* pFormat, ...);
	static void			logNetwork(char* pFormat, ...);
	static void			logGameLog(char* pFormat, ...);
	static void			logClose(char* pFormat, ...);

	static void			log(char* pFormat, ...);	// uses DEBUG_MAIN
	static void			log(std::string str);		// uses DEBUG_MAIN
	static void			log(glm::vec2 vec);			// uses DEBUG_MAIN
	static void			log(glm::vec3 vec);			// uses DEBUG_MAIN

private:
	static void			_log(uint32_t uiLogType, char *pText);

private:
	static uint32_t		m_uiDebugLevel;
	static std::string	m_strDebugFilePath;
};