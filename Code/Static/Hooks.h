#pragma once

#include "mx.h"
#include "Types.h"
#include <functional>

/*
Notes:

This hook API does not work with lambda callbacks.

//----------------------------------------------------------------

Example 1:

int16_t callback(uint32_t arg0, bool arg1)
{
	return 5;
}
Hooks::hook(0x102030, 8, callback);

//----------------------------------------------------------------

Example 2:

int16_t callback(uint32_t ecx, uint32_t arg0, bool arg1)
{
	return 5;
}
Hooks::hookThis(0x102030, 8, callback);
*/

class mx::Hooks
{
public:
	template<class Func>
	static void								hook(size_t uiInstructionAddress, size_t uiInstructionsOctetCount, Func&& callback)
	{
		auto callback2 = std::function{ std::forward<Func>(callback) };
		hook2(uiInstructionAddress, uiInstructionsOctetCount, callback2, false);
	}

	template<class Func>
	static void								hookThis(size_t uiInstructionAddress, size_t uiInstructionsOctetCount, Func&& callback)
	{
		auto callback2 = std::function{ std::forward<Func>(callback) };
		hook2(uiInstructionAddress, uiInstructionsOctetCount, callback2, true);
	}

	template<class R, class... Args>
	static void								hook2(size_t uiInstructionAddress, size_t uiInstructionsOctetCount, std::function<R(Args...)>& fCallback, bool bUseArgForThis)
	{
		size_t uiArgCount = 10;
		size_t uiHookOctetCount = uiInstructionsOctetCount + (4 * uiArgCount) + 22 + (bUseArgForThis ? 1 : 0);

		void* pHookCode1 = VirtualAlloc(NULL, uiHookOctetCount, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (pHookCode1 == NULL)
		{
			return;
		}

		DWORD uiOld;
		if (VirtualProtect(pHookCode1, uiHookOctetCount, PAGE_EXECUTE_READWRITE, &uiOld) == 0)
		{
			return;
		}

		if (VirtualProtect((char*)uiInstructionAddress, uiInstructionsOctetCount, PAGE_EXECUTE_READWRITE, &uiOld) == 0)
		{
			return;
		}

		size_t uiJmpBackToAddr = uiInstructionAddress + uiInstructionsOctetCount;
		size_t uiCallbackAddr = getAddress(fCallback);
		size_t uiHookAddr = (size_t)pHookCode1;
		uint8_t* pHookCode = (uint8_t*)pHookCode1;
		size_t uiNewEsp = uiHookAddr + 12 + (4 * uiArgCount) + (bUseArgForThis ? 1 : 0);

		size_t* pHook = new size_t;
		*pHook = uiHookAddr;
		size_t* pCallback = new size_t;
		*pCallback = uiCallbackAddr;
		size_t* pJmpBack = new size_t;
		*pJmpBack = uiJmpBackToAddr;

		size_t uiHookAddr2 = (size_t)pHook;
		size_t uiCallbackAddr2 = (size_t)pCallback;
		size_t uiJmpBackToAddr2 = (size_t)pJmpBack;

		/*
		HOOK CODE
		*/

		// preserve registers
		*pHookCode = 0x60; // pushad
		pHookCode++;

		// duplicate N function arguments in stack
		for (size_t i = 0; i < uiArgCount; i++)
		{
			*pHookCode = 0xFF; // push [esp + n]
			pHookCode++;
			*pHookCode = 0x74;
			pHookCode++;
			*pHookCode = 0x24;
			pHookCode++;
			*pHookCode = (4 * uiArgCount) + (8 * 4); // +32 for the previous pushad
			pHookCode++;
		}
		
		if (bUseArgForThis)
		{
			*pHookCode = 0x51; // push ecx
			pHookCode++;
		}

		// change esp value, so that fCallback ret's to this hook mid-way through
		*pHookCode = 0x68; // push 0x10203040
		pHookCode++;
		*(size_t*)pHookCode = uiNewEsp;
		pHookCode += sizeof(size_t);

		// jmp to fCallback
		*pHookCode = 0xFF;
		pHookCode++;
		*pHookCode = 0x25;
		pHookCode++;
		*(size_t*)pHookCode = uiCallbackAddr2;
		pHookCode += sizeof(size_t);

		// unduplicate N function arguments in stack
		*pHookCode = 0x83;
		pHookCode++;
		*pHookCode = 0xC4;
		pHookCode++;
		*pHookCode = (4 * uiArgCount) + (bUseArgForThis ? 4 : 0);
		pHookCode++;

		// restore registers
		*pHookCode = 0x61; // popad
		pHookCode++;

		// run first few octets from original function, but at a different address
		memcpy(pHookCode, (uint8_t*)uiInstructionAddress, uiInstructionsOctetCount);
		pHookCode += uiInstructionsOctetCount;

		// jmp back to original function's remaining octets
		*pHookCode = 0xFF;
		pHookCode++;
		*pHookCode = 0x25;
		pHookCode++;
		*(size_t*)pHookCode = uiJmpBackToAddr2;
		pHookCode += sizeof(size_t);

		/*
		APPLY HOOK
		*/

		// jmp from original game function to hook
		memset((uint8_t*)uiInstructionAddress, '\0', uiInstructionsOctetCount);
		*(uint8_t*)uiInstructionAddress = (uint8_t)0xFFu;
		*(uint8_t*)(uiInstructionAddress + 1) = (uint8_t)0x25u;
		*(size_t*)(uiInstructionAddress + 2) = uiHookAddr2;
	}

	template<typename T, typename... U>
	size_t static getAddress(std::function<T(U...)> f)
	{
		typedef T(fnType)(U...);
		fnType** fnPointer = f.template target<fnType*>();
		return (size_t)*fnPointer;
	}
};