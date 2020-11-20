#pragma once

#include "mx.h"

class mx::Number
{
public:
	static bool								isInRange(double dNumber, double dMin, double dMax);
	static bool								isInRange(int32_t iNumber, int32_t iMin, int32_t iMax);
	static bool								isInteger(double dNumber);
	static bool								isInfinite(uint32_t uiNumber);
	static bool								isInfinite(int32_t iNumber);
	static bool								isInfinite(float fNumber);
	static bool								isNaN(int32_t uiNumber);
	static bool								isNaN(float fNumber);
	static unsigned long					reverseUlong(unsigned long uiNumber);
	static unsigned long					getRandomColour(void);
};