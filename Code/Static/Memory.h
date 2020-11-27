#pragma once

#include "mx.h"
#include "Types.h"
#include <cstdarg>
#include <vector>

class MemoryRange;

class mx::Memory
{
public:
	static void								callCdecl(size_t uiFunctionAddress, unsigned short usArgumentCount, ...);
	static void								callThiscall(size_t uiFunctionAddress, size_t uiThis, unsigned short usArgumentCount, ...);
	static void								call(size_t uiFunctionAddress, unsigned short usArgumentCount, ...);

	static void								setMemoryNopped(size_t uiAddress, unsigned short usByteCount = 1);
	
	static void								removeFunctionCall(size_t uiCallAddress, unsigned short usArgumentCount = 0, bool bCallerCleanStack = false);

	static bool								setPageFullAccess(size_t lpAddress, unsigned long dwSize);
	
	static void*							realloc(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiArrayAddr, uint32_t uiArrayEntrySize, uint32_t uiNewArrayCount);

	template <size_t uiInnerDimensionSize>
	static void								updateAddresses(void *pNewDataStart, size_t uiAddresses[][uiInnerDimensionSize], size_t uiMaxAddrMatch = 0xFFFFFF);
	
	static std::vector<size_t>				getAddressReferences(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiFindAddr);
	static void								getAddressReferencesForRange(
												size_t uiAddressStart,
												size_t uiAddressEnd,
												std::vector<size_t>& vecAddresses,
												std::vector<std::vector<size_t>>& vecAddressReferences);
	
	static size_t							getInstructionAddress(HANDLE hThread);
	static bool								setInstructionAddress(HANDLE hThread, size_t uiAddr);
	static bool								resumeAt(HANDLE hThread, size_t uiAddr);
	static bool								addCode(HANDLE hProcess, uint8_t* pCodeData, uint32_t uiCodeDataSize, size_t& uiCodeAddrOut);
	static bool								loadDLL(HANDLE hProcess, HANDLE hThread, std::string& strDllFilePath);

private:
	__forceinline void						pushVL(unsigned short usStackEntryCount, va_list vlArguments);
	__forceinline void						push(unsigned long uiArgument);
	__forceinline void						removeStackArguments(unsigned short usStackEntryCount);
	__forceinline static void				call(size_t uiFunctionAddress);
};

#ifdef MX_ARCHITECTURE_32
// assembly instruction wrappers
void			mx::Memory::push(unsigned long uiArgument)
{
	__asm
	{
		push	uiArgument
	}
}
void			mx::Memory::pushVL(unsigned short usStackEntryCount, va_list vlArguments)
{
	for (int i = 0; i < usStackEntryCount; i++)
	{
		push(va_arg(vlArguments, unsigned long));
	}
}
void			mx::Memory::removeStackArguments(unsigned short usStackEntryCount)
{
	unsigned long uiStackByteCount = usStackEntryCount * 4;
	__asm
	{
		add		esp, uiStackByteCount
	}
}
__declspec(naked) void			mx::Memory::call(size_t uiFunctionAddress)
{
	__asm
	{
		call	uiFunctionAddress
	}
}

template <size_t uiInnerDimensionSize>
void							mx::Memory::updateAddresses(void *pNewDataStart, size_t pAddresses[][uiInnerDimensionSize], size_t uiMaxAddrMatch)
{
	size_t uiNewAddr = (size_t)pNewDataStart;
	for (uint32 i = 0; i < sizeof(pAddresses) / uiInnerDimensionSize; i++)
	{
		uint32 i2 = 0;
		size_t uiAddrMatch;
		size_t uiNewAddr2 = uiNewAddr + i;
		while ((uiAddrMatch = pAddresses[i][i2]) != 0)
		{
			if (uiAddrMatch <= uiMaxAddrMatch)
				*(size_t*)uiAddrMatch = uiNewAddr2;

			i2++;
		}
	}
}
#endif