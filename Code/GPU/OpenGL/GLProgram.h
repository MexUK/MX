#pragma once

#include <string>
#include "mx.h"
#include "Types.h"
#include "GPU/Base/GPUProgram.h"

class mx::GLProgram : public mx::GPUProgram
{
public:
	uint32								m_uiId;
	uint32								m_uiVertexShader;
	uint32								m_uiFragmentShader;

public:
	void								remove(void);

	uint32								getId(void);

	void								use(void);
	void								setVariableLocation(uint32 uiVariableLocation, std::string& strVariableName);
};