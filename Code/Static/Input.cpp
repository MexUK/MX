#include "Input.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

// key states
bool				Input::isControlKeyDown(void)
{
	return (GetKeyState(VK_CONTROL) & 0x8000) == 0x8000;
}

bool				Input::isShiftKeyDown(void)
{
	return (GetKeyState(VK_SHIFT) & 0x8000) == 0x8000;
}

bool				Input::isAltKeyDown(void)
{
	return (GetKeyState(VK_MENU) & 0x8000) == 0x8000;
}

bool				Input::isCapsLockOn(void)
{
	return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

// key input states
bool				Input::isTextInputUppercase(void)
{
	bool
		bCapsLockIsOn = isCapsLockOn(),
		bShiftIsDown = isShiftKeyDown();
	return (bCapsLockIsOn && !bShiftIsDown) || (!bCapsLockIsOn && bShiftIsDown);
}

string				Input::getTextInInputCase(string& strText)
{
	if (Input::isTextInputUppercase())
		return String::upper(strText);
	else
		return String::lower(strText);
}