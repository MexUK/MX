#pragma once

#include "mx.h"
#include <string>

class mx::DataPath
{
public:
	static std::string			getLocalAppDataPath(void);		// e.g. "C:/Users/<username>/AppData/Local/"
	static std::string			getDesktopFolderPath(void);		// e.g. "C:/Users/<username>/Desktop/"
	static std::string			getStartMenuFolderPath(void);
};