#include "GLImage.h"
#include "GLProgram.h"
#include "GPU/Base/GPUImageBuffers.h"

using namespace mx;

// unloading
void					GLImage::unload(void)
{
	m_pProgram->use();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	/*
	glDeleteVertexArrays(1, &m_vao);

	glDeleteBuffers(1, &m_vboVP);
	glDeleteBuffers(1, &m_vboTP);
	*/

	glDeleteTextures(1, &m_uiTextureBuffer);
}

// rendering
void					GLImage::render(void)
{
	m_pProgram->use();

	glBindVertexArray(m_pImageBuffers->m_vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureBuffer);
	//glUniform1i(glGetUniformLocation(m_pProgram->getId(), "tex"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}