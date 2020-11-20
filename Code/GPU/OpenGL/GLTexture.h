#pragma once

#include "mx.h"
#include "GPU/Base/GPUTexture.h"

class mx::GLTexture : public mx::GPUTexture
{
public:
	GLTexture(void);

public:
	GLuint						m_uiTexture;
};