#pragma once

#include "mx.h"
#include "Stream.h"

class mx::MemoryStream : mx::Stream
{
public:
	MemoryStream(char *pBuffer);
};