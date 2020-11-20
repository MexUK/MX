#include "Thread.h"
//#include <Windows.h>

using namespace std;
using namespace mx;

// suspend
void			Thread::suspend()
{
	SuspendThread(GetCurrentThread());
}

void			Thread::suspend(HANDLE hThread)
{
	SuspendThread(hThread);
}

void			Thread::suspend(uint32_t uiThreadId)
{
}

// resume
void			Thread::resume()
{
	ResumeThread(GetCurrentThread());
}

void			Thread::resume(HANDLE hThread)
{
	ResumeThread(hThread);
}

void			Thread::resume(uint32_t uiThreadId)
{
}

// is suspended
bool			Thread::isSuspended()
{
	return false;
}

bool			Thread::isSuspended(HANDLE hThread)
{
	return false;
}

bool			Thread::isSuspended(uint32_t uiThreadId)
{
	return false;
}

