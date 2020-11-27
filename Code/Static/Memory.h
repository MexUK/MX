#pragma once

#include "mx.h"
#include "Types.h"
#include <cstdarg>
#include <vector>

class MemoryRange;

class mx::Memory
{
public:
	static void								callCdecl(unsigned long uiFunctionAddress, unsigned short usArgumentCount, ...);
	static void								callThiscall(unsigned long uiFunctionAddress, unsigned long uiThis, unsigned short usArgumentCount, ...);
	static void								call(unsigned long uiFunctionAddress, unsigned short usArgumentCount, ...);

	static void								setMemoryNopped(unsigned long uiAddress, unsigned short usByteCount = 1);
	
	static void								removeFunctionCall(unsigned long uiCallAddress, unsigned short usArgumentCount = 0, bool bCallerCleanStack = false);

	static bool								setPageFullAccess(unsigned long lpAddress, unsigned long dwSize);
	
	static void*							realloc(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiArrayAddr, uint32_t uiArrayEntrySize, uint32_t uiNewArrayCount);

	template <size_t uiInnerDimensionSize>
	static void								updateAddresses(void *pNewDataStart, uint32 uiAddresses[][uiInnerDimensionSize], uint32 uiMaxAddrMatch = 0xFFFFFF);
	
	static std::vector<unsigned long>		getAddressReferences(size_t uiAddrRangeStart, size_t uiAddrRangeEnd, size_t uiFindAddr);
	static void								getAddressReferencesForRange(
												unsigned long uiAddressStart,
												unsigned long uiAddressEnd,
												std::vector<unsigned long>& vecAddresses,
												std::vector<std::vector<unsigned long>>& vecAddressReferences);
	
	static uint32_t							getEIP(HANDLE hThread);
	static bool								setEIP(HANDLE hThread, uint32_t uiAddr);
	static bool								resumeAt(HANDLE hThread, uint32_t uiAddr);
	static bool								addCode(HANDLE hProcess, uint8_t* pCodeData, uint32_t uiCodeDataSize, uint32_t& uiCodeAddrOut);
	static bool								loadDLL(HANDLE hProcess, HANDLE hThread, std::string& strDllFilePath);

private:
	__forceinline void						pushVL(unsigned short usStackEntryCount, va_list vlArguments);
	__forceinline void						push(unsigned long uiArgument);
	__forceinline void						removeStackArguments(unsigned short usStackEntryCount);
	__forceinline static void				call(unsigned long uiFunctionAddress);
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
__declspec(naked) void			mx::Memory::call(unsigned long uiFunctionAddress)
{
	__asm
	{
		call	uiFunctionAddress
	}
}

template <size_t uiInnerDimensionSize>
void							mx::Memory::updateAddresses(void *pNewDataStart, uint32 pAddresses[][uiInnerDimensionSize], uint32 uiMaxAddrMatch)
{
	uint32 uiNewAddr = (uint32)pNewDataStart;
	for (uint32 i = 0; i < sizeof(pAddresses) / uiInnerDimensionSize; i++)
	{
		uint32 i2 = 0;
		uint32 uiAddrMatch;
		uint32 uiNewAddr2 = uiNewAddr + i;
		while ((uiAddrMatch = pAddresses[i][i2]) != 0)
		{
			if (uiAddrMatch <= uiMaxAddrMatch)
				*(uint32*)uiAddrMatch = uiNewAddr2;

			i2++;
		}
	}
}
#endif