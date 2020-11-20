#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>

class mx::File
{
public:
	// existence
	static bool							isFile(std::string& strPath);

	// data
	static void							setText(std::string& strPath, std::string& strData);
	static void							setBinary(std::string& strPath, std::string& strData);
	
	static void							appendText(std::string& strPath, std::string& strData);
	static void							appendBinary(std::string& strPath, std::string& strData);
	static void							setPartialBinary(std::string& strPath, std::string& strData, uint32 uiSeek);
	
	static std::string					getText(std::string& strPath);
	static std::string					getBinary(std::string& strPath);
	static std::string					getPartialBinary(std::string& strPath, uint32 uiSeek, uint32 uiByteCount);
	
	// link
	static void							createLink(std::string& strFilePath, std::string& strLinkFilePath, std::string& strLinkDescription);

	// size
	static uint32						getSize(std::string& strPath);
	
	// lines (text-mode)
	static std::vector<std::string>		getLines(std::string& strPath);
	
	// rename
	static void							rename(std::string& strPath, std::string& strNewPath);

	// creation date
	static uint32						getCreationDate(std::string& strFilePath);

	// remove
	static void							remove(std::string& strPath);

	// copy
	static void							copy(std::string& strSourcePath, std::string& strDestPath, bool bOverwriteDestPath = false);

	// move
	static bool							move(std::string& strSourcePath, std::string& strDestPath);









	// THESE RENAME FUNCTUONS ARE LIKELY BROKEN - they relied upon getFilePaths returning both files and folders.
	static void							renameFoldersWildcard(std::string& strInputOutputPath, std::vector<std::string>& vecPreviousWildcardNames, std::string& strNewWildcardName, bool bRecursively = false, bool bCaseSensitive = false);
	static void							renameFilesWildcard(std::string& strInputOutputPath, std::vector<std::string>& vecPreviousWildcardNames, std::string& strNewWildcardName, bool bRecursively = false, bool bCaseSensitive = false);
	static void							renameFilesContentWildcard(std::string& strInputOutputPath, std::vector<std::string>& vecPreviousWildcardNames, std::string& strNewWildcardName, bool bRecursively = false, bool bCaseSensitive = false);
	static void							renameFileContentWildcard(std::string& strInputOutputPath, std::string& strPreviousWildcardName, std::string& strNewWildcardName, bool bCaseSensitive = false);
	
	// file name
	static std::string					getFileNameFromNameWithoutExtension(std::string& strFolderPath, std::string& strFileNameWithoutExtension);
	static std::string					getNextIncrementingFileName(std::string& strFilePath);
	
	// file info fetching
	static std::string					findFile(std::string& strFileName, std::string& strFolderPath); // returns full path to file or blank string if not found
	static std::string					findImageFile(std::string& strFileNameWithoutExtension, std::string& strFolderPath); // returns full path to file or blank string if not found
	
private:
	// file data
	static void							_editFile(std::string& strPath, std::string& strData, bool bAppend, bool bBinaryMode);
	static std::string					_readFile(std::string& strPath, bool bBinaryMode);
};