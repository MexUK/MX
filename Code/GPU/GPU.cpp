#include "GPU.h"

using namespace std;
using namespace glm;
using namespace mx;

// static properties
GraphicsLibrary*			GPU::m_pGraphicsLibrary			= nullptr;

// active graphics library
void						GPU::setGraphicsLibrary(EGraphicsLibrary uiGraphicsLibrary)
{
	switch (uiGraphicsLibrary)
	{
	case GRAPHICS_LIBRARY_OPEN_GL:
		m_pGraphicsLibrary = new OpenGL;
		break;
	case GRAPHICS_LIBRARY_DIRECT_X:
		m_pGraphicsLibrary = new DirectX;
		break;
	}
}