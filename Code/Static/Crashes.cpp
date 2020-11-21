#include "Crashes.h"
#include "Static/String.h"
#include "Static/File.h"
#include <time.h>

using namespace std;
using namespace mx;

// binding
void					Crashes::bind(void)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)onCrash);
}

void					Crashes::unbind(void)
{
	SetUnhandledExceptionFilter(NULL);
}

// crash
LONG WINAPI				Crashes::onCrash(_EXCEPTION_POINTERS *pExceptionInfo)
{
	string strFilePath = m_strFilePath.empty() ? DEFAULT_CRASH_LOG_FILE_PATH : m_strFilePath;
	string strLogText = getLogText(pExceptionInfo);
	File::appendText(strFilePath, strLogText);
	return EXCEPTION_CONTINUE_SEARCH;
}

// log entry
string					Crashes::getLogText(_EXCEPTION_POINTERS* pExceptionInfo)
{
	CONTEXT* ctx = pExceptionInfo->ContextRecord;
	char szLogText[4096];
	sprintf_s(
		szLogText,

		"Date: %s\n"
		"Extra: %s\n"
		"EIP: 0x%x\n"
		"EAX: 0x%x\n"
		"EBX: 0x%x\n"
		"ECX: 0x%x\n"
		"EDX: 0x%x\n"
		"ESP: 0x%x\n"
		"EBP: 0x%x\n"
		"ESI: 0x%x\n"
		"EDI: 0x%x\n"
		"EFlags: 0x%x\n"
		"------------------------------------------------------------------\n",

		String::getTimestampText().c_str(),
		m_strExtraText.c_str(),
		ctx->Eip,
		ctx->Eax,
		ctx->Ebx,
		ctx->Ecx,
		ctx->Edx,
		ctx->Esp,
		ctx->Ebp,
		ctx->Esi,
		ctx->Edi,
		ctx->EFlags
	);
	return szLogText;
}