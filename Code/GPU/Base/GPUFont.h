#pragma once

#include <string>
#include "mx.h"

class mx::GPUFont
{
public:
	virtual bool			init(std::string& strFontName, glm::uvec2& vecScreenSize) = 0;
	virtual void			render(std::string& strText, glm::vec2& vecPosition, float fScale, glm::vec3& vecColour) = 0;
};