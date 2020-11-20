#pragma once

#include "mx.h"
#include "GPU/Base/GPUImage.h"

class mx::GLImage : public mx::GPUImage
{
public:
	mx::GPUProgram*			m_pProgram;
	GLuint					m_uiTextureBuffer;
	mx::GPUImageBuffers*	m_pImageBuffers;

public:
	void					unload(void);

	void					render(void);
};