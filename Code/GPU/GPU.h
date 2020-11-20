#pragma once

#include "mx.h"
#include "GPU/OpenGL/OpenGL.h"
#include "GPU/DirectX/DirectX.h"
#include "Configuration/EGraphicsLibrary.h"

class mx::GPU
{
private:
	static mx::GraphicsLibrary*			m_pGraphicsLibrary;

public:
	inline static mx::GraphicsLibrary*	get(void) { return m_pGraphicsLibrary; }

	static void							setGraphicsLibrary(mx::EGraphicsLibrary uiGraphicsLibrary);
};