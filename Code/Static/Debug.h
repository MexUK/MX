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

#define MX_DEBUG_LAUNCH			1
#define MX_DEBUG_MAIN			2
#define MX_DEBUG_NET_SCRIPT		4
#define MX_DEBUG_INPUT			8
#define MX_DEBUG_RENDER			16
#define MX_DEBUG_HOOK			32
#define MX_DEBUG_NETWORK		64
#define MX_DEBUG_GAME_LOG		128
#define MX_DEBUG_CLOSE			256

#define MX_DEBUG_NONE			0
#define MX_DEBUG_MIN			MX_DEBUG_MAIN
#define MX_DEBUG_ALL			MX_DEBUG_LAUNCH | MX_DEBUG_MAIN | MX_DEBUG_NET_SCRIPT | MX_DEBUG_INPUT | MX_DEBUG_RENDER | MX_DEBUG_HOOK | MX_DEBUG_NETWORK | MX_DEBUG_GAME_LOG | MX_DEBUG_CLOSE

#define MX_DEBUG_BUFFER_SIZE	65536

#define MX_DEBUG_LOG_CALLS		va_list args;\
								char szBuffer[MX_DEBUG_BUFFER_SIZE];\
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