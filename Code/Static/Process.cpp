#include "Process.h"
#include "Static/String.h"
#include "TlHelp32.h"
#include <Shellapi.h>

using namespace std;
using namespace mx;

// start existence
bool							Process::startProcess(string& strEXEFilePath)
{
	return (int) ShellExecute(NULL, L"open", String::atow(strEXEFilePath).c_str(), NULL, NULL, SW_SHOWNORMAL) > 32;
}

bool							Process::isProcessActive(string& strExecutableName)
{
	bool bFound = false;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hProcessSnap, &pe32);
	do
	{
		if (wstring(pe32.szExeFile) == String::atow(strExecutableName))
		{
			bFound = true;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bFound;
}

// process fetching
HANDLE							Process::getProcessByName(string& strExecutableName, uint32_t& uiProcessIdOut)
{
	bool bFound = false;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hProcessSnap, &pe32);
	do
	{
		if (wstring(pe32.szExeFile) == String::atow(strExecutableName))
		{
			bFound = true;
			uiProcessIdOut = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return nullptr;
}

// EXE file
string							Process::getEXEFilePath(void)
{
	TCHAR wszRunningProgramPath[MAX_PATH];
	wmemset(wszRunningProgramPath, 0, MAX_PATH);
	GetModuleFileName(NULL, wszRunningProgramPath, MAX_PATH);
	wstring wstr = wszRunningProgramPath;
	return String::wtoa(wstr);
}

// threads
HANDLE							Process::getPrimaryThread(uint32_t uiProcessId, uint32_t& uiThreadIdOut)
{
	if(uiProcessId == 0)
		uiProcessId = GetCurrentProcessId();

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSnapshot);
		return nullptr;
	}

	THREADENTRY32 te32;
	HANDLE
		hThread,
		hThreadRet = nullptr;
	FILETIME
		creationTime,
		exitTime,
		kernelTime,
		userTime;
	unsigned long dwLowestCreationTime = -1;
	te32.dwSize = sizeof(THREADENTRY32);

	if (Thread32First(hSnapshot, &te32) == FALSE)
	{
		CloseHandle(hSnapshot);
		return nullptr;
	}

	do {
		if (te32.th32OwnerProcessID == uiProcessId)
		{
			hThread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME, false, te32.th32ThreadID);
			if (hThread != NULL)
			{
				if (GetThreadTimes(hThread, &creationTime, &exitTime, &kernelTime, &userTime) != 0)
				{
					if (creationTime.dwLowDateTime < dwLowestCreationTime)
					{
						if (hThreadRet != nullptr)
						{
							CloseHandle(hThreadRet);
						}

						dwLowestCreationTime = creationTime.dwLowDateTime;
						hThreadRet = hThread;
						uiThreadIdOut = te32.th32ThreadID;
					}
					else
					{
						CloseHandle(hThread);
					}
				}
				else
				{
					CloseHandle(hThread);
				}
			}
		}
	}
	while (Thread32Next(hSnapshot, &te32) != FALSE);

	CloseHandle(hSnapshot);
	return hThreadRet;
}

bool							Process::toggleThreadSuspended(HANDLE hThread, bool bSuspended)
{
	unsigned long dwResult;
	if (bSuspended)
	{
		dwResult = SuspendThread(hThread);
	}
	else
	{
		dwResult = ResumeThread(hThread);
	}
	return dwResult != (unsigned long)-1;
}

HANDLE							Process::getCurrentThreadNonPseudo(void)
{
	HANDLE
		hThread = GetCurrentThread(),
		hNewThread;
	if (DuplicateHandle(GetCurrentProcess(), hThread, GetCurrentProcess(), &hNewThread, 0, false, DUPLICATE_SAME_ACCESS) == 0)
		return nullptr;
	return hNewThread;
}

// explorer
void							Process::openFolder(string& strFolderPath)
{
	shellCommand(strFolderPath);
}

void							Process::openTextFile(string& strFilePath)
{
	shellCommand(strFilePath);
}

// shell
void							Process::shellCommand(string& strText)
{
	ShellExecute(NULL, NULL, String::atow(strText).c_str(), NULL, NULL, SW_SHOWNORMAL);
}