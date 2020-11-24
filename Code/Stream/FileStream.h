#pragma once

#include <string>

#include "mx.h"
#include "Stream.h"

class mx::FileStream : mx::Stream
{
public:
	FileStream(std::string& strFilePath, bool bCheckForErrors = true);
};