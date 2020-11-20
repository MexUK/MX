#include "Process.h"
#include "Static/String.h"
#include "TlHelp32.h"
#include <Shellapi.h>

using namespace std;
using namespace mx;

// process
bool							Process::startProcess(string& strEXEFilePath)
{
	return (int) ShellExecute(NULL, L"open", String::convertStdStringToStdWString(strEXEFilePath).c_str(), NULL, NULL, SW_SHOWNORMAL) > 32;
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
		if (wstring(pe32.szExeFile) == String::convertStdStringToStdWString(strExecutableName))
		{
			bFound = true;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return bFound;
}

HANDLE							Process::getProcessByName(string& strExecutableName, uint32_t& uiProcessIdOut)
{
	bool bFound = false;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hProcessSnap, &pe32);
	do
	{
		if (wstring(pe32.szExeFile) == String::convertStdStringToStdWString(strExecutableName))
		{
			bFound = true;
			uiProcessIdOut = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return nullptr;
}

// process code
void							Process::injectLibrary(string& strLibraryPath)
{
	LoadLibrary(String::convertStdStringToStdWString(strLibraryPath).c_str());
}

// EXE file
string							Process::getEXEFilePath(void)
{
	TCHAR wszRunningProgramPath[MAX_PATH];
	wmemset(wszRunningProgramPath, 0, MAX_PATH);
	GetModuleFileName(NULL, wszRunningProgramPath, MAX_PATH);
	wstring wstr = wszRunningProgramPath;
	return String::convertStdWStringToStdString(wstr);
}

// threads
HANDLE							Process::getPrimaryThread(uint32_t uiProcessId, uint32_t& uiThreadIdOut)
{
	if(uiProcessId == 0)
		uiProcessId = GetCurrentProcessId();

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		//CUtility::debug("0");
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
		//CUtility::debug("1");
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
					//CUtility::debugva("thread id %u creationTime %u", GetThreadId(hThread), creationTime.dwLowDateTime);

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
	{
		//CUtility::debug("failed to duplicate thread handle in CUtility::getCurrentThreadNonPseudo()");
	}
	return hNewThread;
}

// explorer
void							Process::openFolder(string& strFolderPath)
{
	ShellExecute(NULL, NULL, String::convertStdStringToStdWString(strFolderPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void							Process::openTextFile(string& strFilePath)
{
	ShellExecute(NULL, NULL, String::convertStdStringToStdWString(strFilePath).c_str(), NULL, NULL, SW_SHOWNORMAL);
}