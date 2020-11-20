#include "GLFont.h"
#include "OpenGL.h"
#include "GLProgram.h"
#include "GPU/GPU.h"

using namespace std;
using namespace glm;
using namespace mx;

bool			GLFont::init(string& strFontName, uvec2& vecRenderSize)
{
	m_vecRenderSize = vecRenderSize;

	string strVertexShaderFilePath = "Shaders/Text.vert";
	string strFragmentShaderFilePath = "Shaders/Text.frag";
	m_pProgram = GPU::get()->addProgram(strVertexShaderFilePath, strFragmentShaderFilePath);

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		//cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, ("Fonts/" + strFontName + ".ttf").c_str(), 0, &face))
	{
		//cout << "ERROR::FREETYPE: Failed to load font" << endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		//cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
		return false;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			//cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
			//return false;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		// Now store character for later use
		Character character = {
			texture,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m_Characters.insert(pair<GLchar, Character>(c, character));
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_STATIC_DRAW);
	GLint iAttributeLocation = glGetAttribLocation(m_pProgram->getId(), "vertex");
	glEnableVertexAttribArray(iAttributeLocation);
	//glVertexAttribPointer(iAttributeLocation, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribPointer(iAttributeLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glDisable(GL_BLEND);
	

	int a;
	a = 1;
	a++;

	return true;
}

void			GLFont::render(string& text, vec2& vecPosition, float scale, vec3& vecColour)
{
	GLfloat x = vecPosition.x;
	GLfloat y = m_vecRenderSize.y - vecPosition.y;

	int iWindowBorderHeight = 30; // todo - probably different per OS - use GetClientRect.top or something
	y -= iWindowBorderHeight;

	m_pProgram->use();

	glEnable(GL_BLEND);

	// Activate corresponding render state
	mat4 projection = ortho(0.0f, (GLfloat)m_vecRenderSize.x, 0.0f, (GLfloat)m_vecRenderSize.y);
	glUniformMatrix4fv(glGetUniformLocation(m_pProgram->getId(), "projection"), 1, GL_FALSE, value_ptr(projection));

	glUniform3f(glGetUniformLocation(m_pProgram->getId(), "textColor"), vecColour.x, vecColour.y, vecColour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// Iterate through all characters
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}