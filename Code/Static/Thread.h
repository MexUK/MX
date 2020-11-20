#pragma once

#include "mx.h"
#include "Types.h"
#include <cstdarg>
#include <vector>
//#include <Windows.h>

class mx::Thread
{
public:
	static void								suspend();
	static void								suspend(HANDLE hThread);
	static void								suspend(uint32_t uiThreadId);

	static void								resume();
	static void								resume(HANDLE hThread);
	static void								resume(uint32_t uiThreadId);

	static bool								isSuspended();
	static bool								isSuspended(HANDLE hThread);
	static bool								isSuspended(uint32_t uiThreadId);
};