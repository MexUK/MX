#pragma once

#include "mx.h"
#include "Types.h"
#include <string>

class mx::Registry
{
public:
	static void				setRegistryValueString(std::string& strKey1, std::string& strKey2, std::string& strValue);
	static std::string		getRegistryValueString(std::string& strKey1, std::string& strKey2);
	static void				setRegistryValueInt(std::string& strKey1, std::string& strKey2, int iValue);
	static int				getRegistryValueInt(std::string& strKey1, std::string& strKey2);
	static void				removeRegistryValue(std::string& strKey1, std::string& strKey2);
	static void				removeRegistryKey(std::string& strKey1, std::string& strKey2);

	static void				assoicateFileExtension(std::string& strFileExtension, std::string& strExePath, std::string& strAppIdentifier);

	static void				setSoftwareValueString(std::string& strKey1, std::string& strKey2, std::string& strValue);
	static std::string		getSoftwareValueString(std::string& strKey1, std::string& strKey2);
	static void				setSoftwareValueInt(std::string& strKey1, std::string& strKey2, int iValue);
	static int				getSoftwareValueInt(std::string& strKey1, std::string& strKey2);
	static void				removeSoftwareValue(std::string& strKey1, std::string& strKey2);
};