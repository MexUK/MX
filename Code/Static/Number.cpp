#include "Number.h"
#include <cstdio>
#include <cstring>
#include <cmath>

using namespace mx;

bool				Number::isInRange(double dNumber, double dMin, double dMax)
{
	return dNumber >= dMin && dNumber <= dMax;
}

bool				Number::isInRange(int32_t iNumber, int32_t iMin, int32_t iMax)
{
	return iNumber >= iMin && iNumber <= iMax;
}

bool				Number::isInteger(double dNumber)
{
	char szBuffer[50];
	sprintf_s(szBuffer, "%g", dNumber);
	return strstr(szBuffer, ".") == nullptr;
}

bool				Number::isInfinite(uint32_t uiNumber)
{
	return false; // todo isinf(uiNumber);
}

bool				Number::isInfinite(int32_t iNumber)
{
	return false; // todo isinf(iNumber);
}

bool				Number::isInfinite(float fNumber)
{
	return false; // todo isinf(fNumber);
}

bool				Number::isNaN(int32_t uiNumber)
{
	return false; // todo isnan(uiNumber);
}

bool				Number::isNaN(float fNumber)
{
	return false; // todo isnan(fNumber);
}

unsigned long		Number::reverseUlong(unsigned long uiNumber)
{
	return (uiNumber % 256) * 16777216,
		+((unsigned long)floor(uiNumber / 256)) * 65536,
		+((unsigned long)floor(uiNumber / 65536)) * 256,
		+((unsigned long)floor(uiNumber / 16777216));
}

// colour
unsigned long		Number::getRandomColour(void)
{
	return (rand() % 256) | ((rand() % 256) << 8) | ((rand() % 256) << 16) | (255 << 24);
}