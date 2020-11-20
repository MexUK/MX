#pragma once

#include <string>
#include <map>
#include "ThirdParty/freetype-2.9.1/include/ft2build.h"
#include FT_FREETYPE_H
#include "mx.h"
#include "GPU/Base/GPUFont.h"

struct Character
{
	GLuint		TextureID;  // ID handle of the glyph texture
	glm::ivec2	Size;       // Size of glyph
	glm::ivec2	Bearing;    // Offset from baseline to left/top of glyph
	FT_Pos		Advance;    // Offset to advance to next glyph
};

class mx::GLFont : public mx::GPUFont
{
public:
	bool			init(std::string& strFontName, glm::uvec2& vecScreenSize);
	void			render(std::string& strText, glm::vec2& vecPosition, float fScale, glm::vec3& vecColour);

public:
	glm::uvec2						m_vecRenderSize;
	mx::GPUProgram*					m_pProgram;

private:
	GLuint							m_VAO, m_VBO;
	std::map<GLchar, Character>		m_Characters;
};