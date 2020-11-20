#include "GLProgram.h"

using namespace std;
using namespace glm;
using namespace mx;

// unloading
void					GLProgram::remove(void)
{
	glDetachShader(m_uiId, m_uiVertexShader);
	glDetachShader(m_uiId, m_uiFragmentShader);

	glDeleteShader(m_uiVertexShader);
	glDeleteShader(m_uiFragmentShader);

	glDeleteProgram(m_uiId);
}

// program id
uint32					GLProgram::getId(void)
{
	return m_uiId;
}

// use program
void					GLProgram::use(void)
{
	glUseProgram(m_uiId);
}

// variable location
void					GLProgram::setVariableLocation(uint32 uiVariableLocation, string& strVariableName)
{
	m_umapUniformLocations[uiVariableLocation] = glGetUniformLocation(m_uiId, strVariableName.data());
}