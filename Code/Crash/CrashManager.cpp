#include "CrashManager.h"
#include "Static/File.h"
#include <time.h>

using namespace std;
using namespace mx;

// initialization
void					CrashManager::init(void)
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashManager::onCrashStatic);
}

void					CrashManager::uninit(void)
{
}

// crash
LONG WINAPI				CrashManager::onCrashStatic(_EXCEPTION_POINTERS *pExceptionInfo)
{
	get()->onCrash(pExceptionInfo);
	return EXCEPTION_CONTINUE_SEARCH;
}

void					CrashManager::onCrash(_EXCEPTION_POINTERS *pExceptionInfo)
{
	CONTEXT *ctx = pExceptionInfo->ContextRecord;

	// fetch date as string
	time_t rawtime = time(NULL);
	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);
	char szDate[100];
	strftime(szDate, sizeof(szDate), "%d %B %Y %X [%Z]", &timeinfo);

	// create text for crash log
	char szLogText[1000];
	sprintf_s(
		szLogText,

		"Date: %s\n"
		"Extra Info - %s\n"
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

		szDate,
		m_strExtraLogEntryText.c_str(),
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

	string strLogText = string(szLogText);
	File::appendText(getFilePath(), strLogText);
}