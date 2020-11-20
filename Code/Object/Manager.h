#pragma once

#include "mx.h"

class mx::Manager
{
public:
	virtual void				init(void) = 0;
	virtual void				uninit(void) = 0;
};