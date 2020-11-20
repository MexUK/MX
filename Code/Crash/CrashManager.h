#pragma once

#include "mx.h"
#include "Object/Singleton.h"
//#include <Windows.h>

class mx::CrashManager : public mx::Singleton<mx::CrashManager>
{
public:
	void					init(void);
	void					uninit(void);

	void					setFilePath(std::string& strFilePath) { m_strFilePath = strFilePath; }
	std::string&			getFilePath(void) { return m_strFilePath; }

	void					setExtraLogEntryText(std::string& strExtraLogEntryText) { m_strExtraLogEntryText = strExtraLogEntryText; }
	std::string&			getExtraLogEntryText(void) { return m_strExtraLogEntryText; }

private:
	static LONG WINAPI		onCrashStatic(_EXCEPTION_POINTERS *pExceptionInfo);
	void					onCrash(_EXCEPTION_POINTERS *pExceptionInfo);

private:
	std::string				m_strFilePath;
	std::string				m_strExtraLogEntryText;
};