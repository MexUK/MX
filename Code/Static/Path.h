#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>

class mx::Path
{
public:
	static bool					hasSlash(std::string& strPath);
	static std::string			slash(std::string& strPath);
	static std::string			unslash(std::string& strPath);
	static std::string			slashFront(std::string& strPath);
	static std::string			unslashFront(std::string& strPath);
	static std::string			replaceSlashes(std::string& strPath);

	static std::string			getFileName(std::string& strPath);
	static std::string			getFolderName(std::string& strPath);
	static std::string			getDirectory(std::string& strPath);

	static std::string			removeFileName(std::string& strPath);
	static std::string			removeFolderName(std::string& strPath);

	static std::string			replaceFolderName(std::string& strPath, std::string& strNewFolderName);
	static std::string			replaceFileName(std::string& strPath, std::string& strNewFileName);

	static std::string			ext(std::string& strPath);
	static bool					isExt(std::string& strPath, std::string strExt);
	static std::string			setExt(std::string& strPath, std::string& strExtension);
	static std::string			setExtWithCase(std::string& strPath, std::string& strExtension);
	static std::string			removeExt(std::string& strPath);

	static bool					isValidPath(std::string& strPath);
	
	static std::string			getNextFileName(std::string& strFilePath, uint32 uiFilenameIndex, std::string& strNextSuffix);
	static std::vector<std::string>	getUniqueFilePaths(std::vector<std::string>& vecFilePaths, std::vector<std::string>& vecFileNames);

	static bool					comparePaths(std::string& strFilePath1, std::string& strFilePath2);

	static std::string			getDisplayableFilePath(std::string& strFilePath);
};