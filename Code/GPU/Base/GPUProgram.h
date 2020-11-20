#pragma once

#include <string>
#include <unordered_map>
#include "mx.h"

class mx::GPUProgram
{
public:
	std::unordered_map<uint32, uint32>			m_umapUniformLocations;

public:
	virtual void								remove(void) = 0;

	virtual uint32								getId(void) = 0;

	virtual void								use(void) = 0;
	virtual void								setVariableLocation(uint32 uiVariableLocation, std::string& strVariableName) = 0;
};