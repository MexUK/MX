#pragma once

#include "mx.h"

class mx::GPUImage
{
public:
	virtual void					unload(void) = 0;

	virtual void					render(void) = 0;
};