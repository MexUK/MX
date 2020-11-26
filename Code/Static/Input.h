#pragma once

#include "mx.h"

class mx::Input
{
public:
	static bool								isControlKeyDown(void);
	static bool								isShiftKeyDown(void);
	static bool								isAltKeyDown(void);
	static bool								isCapsLockOn(void);
	static bool								isTextInputUppercase(void);
	static std::string						getTextInInputCase(std::string& strText);
};