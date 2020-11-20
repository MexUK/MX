#pragma once

#include "mx.h"
#include <string>
//#include <Windows.h>

class mx::Process
{
public:
	// process
	static bool							startProcess(std::string& strEXEFilePath);
	static bool							isProcessActive(std::string& strExecutableName);
	static HANDLE						getProcessByName(std::string& strExecutableName, uint32_t& uiProcessIdOut);

	// process code
	static void							injectLibrary(std::string& strLibraryPath);
	
	// EXE file
	static std::string					getEXEFilePath(void);

	// threads
	static HANDLE						getPrimaryThread(uint32_t uiProcessId, uint32_t& uiThreadIdOut);
	static HANDLE						getCurrentThreadNonPseudo(void);
	static bool							toggleThreadSuspended(HANDLE hThread, bool bSuspended);

	// explorer
	static void							openFolder(std::string& strFolderPath);

	// programs
	static void							openTextFile(std::string& strFilePath);
};