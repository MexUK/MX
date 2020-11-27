#include "Memory.h"

using namespace std;
using namespace mx;

// function invoking
void			Memory::callCdecl(unsigned long uiFunctionAddress, unsigned short usArgumentCount, ...)
{
#ifdef MX_ARCHITECTURE_32
	va_list vlArgs;
	va_start(vlArgs, usArgumentCount);
	unsigned long uiArgument;
	for (int i = 0; i < usArgumentCount; i++)
	{
		uiArgument = va_arg(vlArgs, unsigned long);
		__asm
		{
			push	uiArgument
		}
	}
	va_end(vlArgs);

	unsigned long uiStackByteCount = usArgumentCount * 4;
	__asm
	{
		call	uiFunctionAddress
		add		esp, uiStackByteCount
	}
#else
#ifdef MX_ARCHITECTURE_64
	// todo
#endif
#endif
	/*
	va_list vlArgs;
	va_start(vlArgs, usArgumentCount);
	pushVL(usArgumentCount, vlArgs);
	va_end(vlArgs);

	call(uiFunctionAddress);
	removeStackArguments(usArgumentCount);
	*/
}

void			Memory::callThiscall(unsigned long uiFunctionAddress, unsigned long uiThis, unsigned short usArgumentCount, ...)
{
#ifdef MX_ARCHITECTURE_32
	va_list vlArgs;
	va_start(vlArgs, usArgumentCount);
	unsigned long uiArgument;
	for (int i = 0; i < usArgumentCount; i++)
	{
		uiArgument = va_arg(vlArgs, unsigned long);
		__asm
		{
			push	uiArgument
		}
	}
	va_end(vlArgs);

	__asm
	{
		mov		ecx, uiThis
		call	uiFunctionAddress
	}
#else
#ifdef MX_ARCHITECTURE_64
	// todo
#endif
#endif
}

void			Memory::call(unsigned long uiFunctionAddress, unsigned short usArgumentCount, ...)
{
#ifdef MX_ARCHITECTURE_32
	va_list vlArgs;
	va_start(vlArgs, usArgumentCount);
	unsigned long uiArgument;
	for (int i = 0; i < usArgumentCount; i++)
	{
		uiArgument = va_arg(vlArgs, unsigned long);
		__asm
		{
			push	uiArgument
		}
	}
	va_end(vlArgs);

	__asm
	{
		call	uiFunctionAddress
	}
#else
#ifdef MX_ARCHITECTURE_64
	// todo
#endif
#endif

	/*
	va_list vlArgs;
	va_start(vlArgs, usArgumentCount);
	pushVL(usArgumentCount, vlArgs);
	va_end(vlArgs);

	call(uiFunctionAddress);
	removeStackArguments(usArgumentCount);
	*/
}

// machine code modifying
void			Memory::setMemoryNopped(unsigned long uiAddress, unsigned short usByteCount)
{
#ifdef MX_ARCHITECTURE_32
	memset((void*)uiAddress, 0x90, usByteCount);
#else
#ifdef MX_ARCHITECTURE_64
	// todo
#endif
#endif
}

void			Memory::removeFunctionCall(unsigned long uiCallAddress, unsigned short usArgumentCount, bool bCallerCleanStack)
{
	//removeCallArguments(uiCallAddress, usArgumentCount);
	//removeInstruction_call(uiCallAddress);
	//removeCallerStackCleanUp(uiCallAddress + 5);
#ifdef MX_ARCHITECTURE_32
	uint8 iNOP = (uint8) 0x90;
	for (int i = 1; i <= usArgumentCount; i++)
	{
		*(char*) (uiCallAddress - (i*2)) = iNOP;
		*(char*) (uiCallAddress - ((i*2)+1)) = iNOP;
	}
	for (int i = 0; i < 5; i++)
	{
		*(char*)(uiCallAddress + i) = iNOP;
	}
	if (bCallerCleanStack)
	{
		for (int i = 0; i < 3; i++)
		{
			*(char*)(uiCallAddress + 5 + i) = iNOP;
		}
	}
#else
#ifdef MX_ARCHITECTURE_64
	// todo
#endif
#endif
}

// page access
bool			Memory::setPageFullAccess(unsigned long lpAddress, unsigned long dwSize)
{
	unsigned long lpflOldProtect;
	return VirtualProtect((LPVOID) lpAddress, dwSize, PAGE_EXECUTE_READWRITE, &lpflOldProtect) != 0;
}

// address rewriting
void* Memory::realloc(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiArrayAddr, uint32_t uiArrayEntrySize, uint32_t uiNewArrayCount)
{
	void* pNewArray = malloc(uiNewArrayCount * uiArrayEntrySize);
	memset(pNewArray, 0, uiNewArrayCount * uiArrayEntrySize);
	uint32_t uiNewArrayAddr = (uint32_t)pNewArray;

	uint32_t uiNewAddr = uiNewArrayAddr;
	for (uint32_t uiAddr = uiArrayAddr; uiAddr < (uiArrayAddr + uiArrayEntrySize); uiAddr++)
	{
		vector<unsigned long> vecAddresses = getAddressReferences(uiAddrRangeStart, uiAddrRangeEnd, uiAddr);
		for (uint32_t uiAddr2 : vecAddresses)
		{
			*(uint32_t*)uiAddr2 = uiNewAddr;
		}
		uiNewAddr++;
	}

	return pNewArray;
}

vector<unsigned long>	Memory::getAddressReferences(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiFindAddr)
{
	vector<unsigned long> vecAddresses;
	unsigned long uiLong = 0;
	unsigned char
		ucByte0,
		ucByte1,
		ucByte2,
		ucByte3;
	for (unsigned char
			*pMemory = (unsigned char*)uiAddrRangeStart,
			*pMemoryEnd = (unsigned char*) (uiAddrRangeEnd - (sizeof(unsigned long) - 1));
		pMemory != pMemoryEnd;
		pMemory++)
	{
		ucByte0 = *(unsigned char*) pMemory;
		ucByte1 = *(unsigned char*) (pMemory + 1);
		ucByte2 = *(unsigned char*) (pMemory + 2);
		ucByte3 = *(unsigned char*) (pMemory + 3);


		//if (convertByesToLong() == uiAddress)
		//uiLong = ucByte3 + (ucByte2 * 256) + (ucByte1 * 65536) + (ucByte0 * 16777216);
		uiLong = ucByte0 + (ucByte1 * 256) + (ucByte2 * 65536) + (ucByte3 * 16777216);
		if (uiLong == uiFindAddr)
		{
			vecAddresses.push_back((unsigned long) pMemory);
		}
	}
	return vecAddresses;
}

void						Memory::getAddressReferencesForRange(
	unsigned long uiAddressStart,
	unsigned long uiAddressEnd,
	vector<unsigned long>& vecAddresses,
	vector<vector<unsigned long>>& vecAddressReferences)
{
	vector<unsigned long> vecAddressReferences2;
	for (unsigned long uiAddress = uiAddressStart; uiAddress <= uiAddressEnd; uiAddress++)
	{
		vecAddressReferences2 = getAddressReferences(uiAddress, uiAddressStart, uiAddressEnd);
		if (vecAddressReferences2.size())
		{
			vecAddresses.push_back(uiAddress);
			vecAddressReferences.push_back(vecAddressReferences2);
		}
	}
}

// CPU registers
uint32_t							Memory::getEIP(HANDLE hThread)
{
	CONTEXT context;
	ZeroMemory(&context, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(hThread, &context) == 0)
	{
		return 0;
	}

#ifdef MX_ARCHITECTURE_32
	return context.Eip;
#else
#ifdef MX_ARCHITECTURE_64
	return context.Rip;
#endif
#endif
}

bool								Memory::setEIP(HANDLE hThread, uint32_t uiAddr)
{
	CONTEXT context;
	ZeroMemory(&context, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	
	if (GetThreadContext(hThread, &context) == 0)
	{
		return false;
	}

#ifdef MX_ARCHITECTURE_32
	context.Eip = uiAddr;
#else
#ifdef MX_ARCHITECTURE_64
	context.Rip = uiAddr;
#endif
#endif
	context.ContextFlags = CONTEXT_FULL;
	
	if (0 == SetThreadContext(hThread, &context))
	{
		return false;
	}
	
	return true;
}

bool								Memory::resumeAt(HANDLE hThread, uint32_t uiAddr)
{
	if (!setEIP(hThread, uiAddr))
	{
		return false;
	}

	if (ResumeThread(hThread) == -1)
	{
		return false;
	}

	return true;
}

// machine code insertion
bool								Memory::addCode(HANDLE hProcess, uint8_t *pCodeData, uint32_t uiCodeDataSize, uint32_t& uiCodeAddrOut)
{
	// allocate memory in process
	void *pCode = VirtualAllocEx(hProcess, NULL, uiCodeDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (pCode == nullptr)
	{
		return false;
	}

	// write the data to memory in remote process
	SIZE_T cb = 0;
	if (WriteProcessMemory(hProcess, pCode, pCodeData, uiCodeDataSize, &cb) == FALSE)
	{
		return false;
	}

	// ensure all code data was written
	if (cb != uiCodeDataSize)
	{
		return false;
	}

	// pass back address of code
	uiCodeAddrOut = (uint32_t)pCode;

	return true;
}

bool								Memory::loadDLL(HANDLE hProcess, HANDLE hThread, string& strDllFilePath)
{
#ifdef MX_ARCHITECTURE_32
	// Note: Process hProcess must be suspended when calling this function.

	// prepare snippet to inject into remote process
	char aSnippetCode[] =
	{
		0x60, 0xE8, 0x00, 0x00, 0x00, 0x00, 0x5B, 0x81, 0xEB, 0x06, 0x00, 0x00,
		0x00, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0x8D, 0x93, 0x22, 0x00, 0x00, 0x00,
		0x52, 0xFF, 0xD0, 0x61, 0x68, 0xCC, 0xCC, 0xCC, 0xCC, 0xC3
	};

	uint32_t uiSnippetCodeSize = sizeof(aSnippetCode);
	uint32_t uiInjectedDataSize = uiSnippetCodeSize + strDllFilePath.length() + 1;

	unsigned char* pInjectedData = new unsigned char[uiInjectedDataSize];
	memcpy(pInjectedData, aSnippetCode, uiSnippetCodeSize);

	*(uint32_t*)(pInjectedData + 14) = (uint32_t)LoadLibraryA;
	*(uint32_t*)(pInjectedData + 29) = Memory::getEIP(hThread);
	memcpy(pInjectedData + uiSnippetCodeSize, strDllFilePath.c_str(), strDllFilePath.length() + 1);

	// add snippet to remote process
	uint32_t uiCodeAddr;
	if (!Memory::addCode(hProcess, pInjectedData, uiInjectedDataSize, uiCodeAddr))
		return false;

	// run snippet in remote process
	if (!Memory::resumeAt(hThread, uiCodeAddr))
		return false;

	return true;
#else
#ifdef MX_ARCHITECTURE_64
// todo
return true;
#endif
#endif
}