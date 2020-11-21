#pragma once

#include "mx.h"
#include <string>

#define DEFAULT_CRASH_LOG_FILE_PATH "CrashLog.txt"

class mx::Crashes
{
public:
	static void					bind(void);
	static void					unbind(void);

	static void					setFilePath(std::string& strFilePath) { m_strFilePath = strFilePath; }
	static std::string&			getFilePath(void) { return m_strFilePath; }

	static void					setExtraText(std::string& strExtraText) { m_strExtraText = strExtraText; }
	static std::string&			getExtraText(void) { return m_strExtraText; }

private:
	static LONG WINAPI			onCrash(_EXCEPTION_POINTERS *pExceptionInfo);

	static std::string			getLogText(_EXCEPTION_POINTERS* pExceptionInfo);

private:
	static std::string			m_strFilePath;
	static std::string			m_strExtraText;
};