#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>

class mx::Folder
{
public:
	static bool							isFolder(std::string& strPath);
	static bool							create(std::string& strPath);

	static bool							isEmpty(std::string& strPath);
	static void							empty(std::string& strPath);
	static void							remove(std::string& strFolderPath, bool bRecursive = false);

	static void							rename(std::string& strPath, std::string& strNewPath);
	static void							copy(std::string& strSourceFolderPath, std::string& strDestinationFolderPath, std::string& strFileExtensionsIncluded = mx::g_strBlankString, bool bOverwrite = false);

	static std::vector<std::string>		getFileNames(std::string& strFolderPath, std::string strFileExtensions = ""); // multiple extensions can be provided by comma delimiters.
	static std::vector<std::string>		getFilePaths(std::string& strFolderPath, bool bDeep = false, std::string strFileExtensions = ""); // multiple extensions can be provided by comma delimiters.
	static std::vector<std::string>		getFolderNames(std::string& strFolderPath);
	static std::vector<std::string>		getFolderPaths(std::string& strFolderPath, bool bDeep = false);

private:
	static bool							createSingle(std::string& strPath);
};