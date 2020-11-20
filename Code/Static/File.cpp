//#include <Windows.h>
#include "File.h"
#include "Static/Path.h"
#include "Static/String.h"
#include "Static/Folder.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"
#include <fstream>
//#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"

using namespace std;
using namespace mx;

// file data - private
void		File::_editFile(string& strPath, string& strData, bool bAppend, bool bBinaryMode)
{
	string str = Path::getDirectory(strPath);
	Folder::create(str);

	uint32 uiFileOpenFlags = ios::out | (bBinaryMode ? ios::binary : 0) | (bAppend ? ios::app : ios::trunc);
	ofstream file(strPath.c_str(), uiFileOpenFlags);
	if (!file.is_open())
	{
		return;
	}
	file.write(strData.c_str(), strData.length());
	file.close();
}

string		File::_readFile(string& strPath, bool bBinaryMode)
{
	uint32 uiFileOpenFlags = ios::in | (bBinaryMode ? ios::binary : 0);
	ifstream t(strPath.c_str(), uiFileOpenFlags);
	if (!t.is_open())
	{
		return "";
	}
	t.seekg(0, ios::end);
	streamoff size = t.tellg();
	string buffer((unsigned int)size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);
	if(!bBinaryMode)
	{
		buffer.resize((unsigned int)t.gcount());
	}
	t.close();
	return buffer;
}

// file data - public
string			File::getText(string& strPath)
{
	return _readFile(strPath, false);
}

string			File::getBinary(string& strPath)
{
	return _readFile(strPath, true);
}

string			File::getPartialBinary(string& strPath, uint32 uiSeek, uint32 uiByteCount)
{
	ifstream t(strPath.c_str(), ios::in | ios::binary);
	if (!t.is_open())
	{
		return "";
	}
	t.seekg(uiSeek, ios::beg);
	string buffer(uiByteCount, ' ');
	t.read(&buffer[0], uiByteCount);
	t.close();
	return buffer;
}

vector<string>	File::getLines(string& strPath)
{
	ifstream file(strPath.c_str(), ios::in);
	string strLine;
	vector<string> vecFileLines;
	while (getline(file, strLine))
	{
		vecFileLines.push_back(strLine);
	}
	return vecFileLines;
}

uint32			File::getSize(string& strPath)
{
	FILE *pFile;
	if(fopen_s(&pFile, strPath.c_str(), "rb") != 0)
	{
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	uint32 iSeek = ftell(pFile);
	fclose(pFile);
	return iSeek;
}

void			File::setBinary(string& strPath, string& strData)
{
	_editFile(strPath, strData, false, true);
}

void			File::appendBinary(string& strPath, string& strData)
{
	_editFile(strPath, strData, true, true);
}

void			File::setText(string& strPath, string& strData)
{
	string strEOLCharacters = "\n";
	string str = String::fixEOLs(strData, strEOLCharacters);
	_editFile(strPath, str, false, false);
}

void			File::appendText(string& strPath, string& strData)
{
	string strEOLCharacters = "\n";
	string str = String::fixEOLs(strData, strEOLCharacters);
	_editFile(strPath, str, true, false);
}

void			File::rename(string& strPath, string& strNewPath)
{
	::rename(strPath.c_str(), strNewPath.c_str());
}

void			File::renameFoldersWildcard(string& strInputOutputPath, vector<string>& vecPreviousWildcardNames, string& strNewWildcardName, bool bRecursively, bool bCaseSensitive)
{
	vector<string> vecFolderPathsOneLevel = Folder::getFolderPaths(strInputOutputPath, bRecursively);
	for(string& strFolderPath : vecFolderPathsOneLevel)
	{
		string strFolderName = Path::getFolderName(strFolderPath);
		bool bFolderRenamed = false;
		for(string& strPreviousWildcardName : vecPreviousWildcardNames)
		{
			if(String::isIn(strFolderName, strPreviousWildcardName, bCaseSensitive))
			{
				string strFolderNameRenamed = String::replace(strFolderName, strPreviousWildcardName, strNewWildcardName, bCaseSensitive);
				string strFolderPathRenamed = Path::replaceFolderName(strFolderPath, strFolderNameRenamed);
				Folder::rename(strFolderPath, strFolderPathRenamed);
				if(bRecursively)
				{
					renameFoldersWildcard(strFolderPathRenamed, vecPreviousWildcardNames, strNewWildcardName, true, bCaseSensitive);
					bFolderRenamed = true;
					break;
				}
			}
		}
		
		if(bRecursively && !bFolderRenamed)
		{
			renameFoldersWildcard(strFolderPath, vecPreviousWildcardNames, strNewWildcardName, true, bCaseSensitive);
		}
	}
}

void			File::renameFilesWildcard(string& strInputOutputPath, vector<string>& vecPreviousWildcardNames, string& strNewWildcardName, bool bRecursively, bool bCaseSensitive)
{
	vector<string> vecFolderPathsOneLevel = Folder::getFolderPaths(strInputOutputPath, bRecursively);
	for(string& strFolderOrFilePath : vecFolderPathsOneLevel)
	{
		bool bIsFolder = Path::hasSlash(strFolderOrFilePath);
		if (!bIsFolder)
		{
			// file
			string strFileName = Path::getFileName(strFolderOrFilePath);
			for (string& strPreviousWildcardName : vecPreviousWildcardNames)
			{
				if (String::isIn(strFileName, strPreviousWildcardName, bCaseSensitive))
				{
					string strFileNameRenamed = String::replace(strFileName, strPreviousWildcardName, strNewWildcardName, bCaseSensitive);
					string strFilePathRenamed = Path::replaceFileName(strFolderOrFilePath, strFileNameRenamed);
					rename(strFolderOrFilePath, strFilePathRenamed);
				}
			}
		}
		else
		{
			// folder
			if(bRecursively)
			{
				renameFilesWildcard(strFolderOrFilePath, vecPreviousWildcardNames, strNewWildcardName, true, bCaseSensitive);
			}
		}
	}
}

void			File::renameFilesContentWildcard(string& strInputOutputPath, vector<string>& vecPreviousWildcardNames, string& strNewWildcardName, bool bRecursively, bool bCaseSensitive)
{
	vector<string> vecFolderPathsOneLevel = Folder::getFolderPaths(strInputOutputPath, bRecursively);
	for(string& strFolderOrFilePath : vecFolderPathsOneLevel)
	{
		for(string& strPreviousWildcardName : vecPreviousWildcardNames)
		{
			renameFileContentWildcard(strFolderOrFilePath, strPreviousWildcardName, strNewWildcardName, bCaseSensitive);
		}
		
		if(bRecursively && Path::hasSlash(strFolderOrFilePath))
		{
			renameFilesContentWildcard(strFolderOrFilePath, vecPreviousWildcardNames, strNewWildcardName, true, bCaseSensitive);
		}
	}
}

void			File::renameFileContentWildcard(string& strInputOutputPath, string& strPreviousWildcardName, string& strNewWildcardName, bool bCaseSensitive)
{
	string strFileData = getText(strInputOutputPath);
	string strNewFileData = String::replace(strFileData, strPreviousWildcardName, strNewWildcardName, bCaseSensitive);
	setText(strInputOutputPath, strNewFileData);
}

void			File::remove(string& strPath)
{
	// remove read-only flag for file
	SetFileAttributesA(strPath.c_str(), GetFileAttributesA(strPath.c_str()) & ~FILE_ATTRIBUTE_READONLY);

	//remove(strPath.c_str());
	//DeleteFile(strPath.c_str());
	DeleteFileW(String::convertStdStringToStdWString(strPath).c_str());
}

string			File::getFileNameFromNameWithoutExtension(string& strFolderPath, string& strFileNameWithoutExtension)
{
	string strFolderPath2 = Path::slash(strFolderPath);
	string strFileNameWithoutExtension2 = String::upper(strFileNameWithoutExtension);
	strFolderPath2 += "*";
	
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(String::convertStdStringToStdWString(strFolderPath2).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return "";
	}

	do
	{
		wstring wstr = ffd.cFileName;
		string str = String::convertStdWStringToStdString(wstr);
		str = Path::removeExt(str);
		if (String::upper(str) == strFileNameWithoutExtension2)
		{
			FindClose(hFind);
			return String::convertStdWStringToStdString(wstr);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return "";
}

string			File::getNextIncrementingFileName(string& strFilePath)
{
	string strNextFilePath = strFilePath;
	uint32 uiSuffix = 1;
	while (isFile(strNextFilePath))
	{
		strNextFilePath = Path::removeExt(strFilePath) + String::toString(uiSuffix) + "." + Path::ext(strFilePath);
		uiSuffix++;
	}
	return strNextFilePath;
}

void			File::setPartialBinary(string& strPath, string& strData, uint32 uiSeek)
{
	ofstream file(strPath, ios::out | ios::binary | ios::in);
	if (!file.is_open())
	{
		// remove read-only flag for file
		SetFileAttributesA(strPath.c_str(), GetFileAttributesA(strPath.c_str()) & ~FILE_ATTRIBUTE_READONLY);

		// create the file so that the file can be opened with the 'in' flag
		file.open(strPath.c_str(), ofstream::out | ofstream::binary);
		file.close();

		file.open(strPath.c_str(), ofstream::out | ofstream::binary | ios::in);
	}
	if (!file.is_open())
	{
		return;
	}
	file.seekp(uiSeek);
	file.write(strData.c_str(), strData.length());
	file.close();
}

bool						File::isFile(string& strPath)
{
	uint32 uiAttributes = GetFileAttributes(String::convertStdStringToStdWString(strPath).c_str());
	return uiAttributes != INVALID_FILE_ATTRIBUTES && (uiAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY;
}

void						File::copy(string& strSourcePath, string& strDestPath, bool bOverwriteDestPath)
{
	if (!File::isFile(strSourcePath) || File::getSize(strSourcePath) == 0)
	{
		return;
	}

	if ((!bOverwriteDestPath) && File::isFile(strDestPath))
	{
		return;
	}

	string strDestFolderPath = Path::getDirectory(strDestPath);
	if (!Folder::isFolder(strDestFolderPath))
	{
		Folder::create(strDestFolderPath);
	}

	const uint32 uiBufferSize = 4096;
	char szBuffer[uiBufferSize];
	size_t size;

	FILE
		*pFileSource = nullptr,
		*pFileDest = nullptr;
	if (fopen_s(&pFileSource, strSourcePath.c_str(), "rb") != 0)
	{
		return;
	}
	if (fopen_s(&pFileDest, strDestPath.c_str(), "wb") != 0)
	{
		fclose(pFileSource);
		return;
	}

	while (size = fread(szBuffer, 1, uiBufferSize, pFileSource)) {
		/*
		if (ferror(pFileSource) != 0 || feof(pFileSource) != 0)
		{
			break;
		}
		*/

		fwrite(szBuffer, 1, size, pFileDest);
		/*
		if (ferror(pFileDest) != 0)
		{
			break;
		}
		*/
	}

	fclose(pFileSource);
	fclose(pFileDest);
}

bool						File::move(string& strSourcePath, string& strDestPath)
{
	bool bFileExistsInDest = isFile(strDestPath);
	string strMovedFilePath;
	
	string strDestFolderPath = Path::getDirectory(strDestPath);
	if (!Folder::isFolder(strDestFolderPath))
	{
		Folder::create(strDestFolderPath);
	}

	// move existing file in destination to other location
	if (bFileExistsInDest)
	{
		strMovedFilePath = getNextIncrementingFileName(strDestPath);
		if (::rename(strDestPath.c_str(), strMovedFilePath.c_str()) != 0)
		{
			// move failed, just attempt to move requested file
			return ::rename(strSourcePath.c_str(), strDestPath.c_str()) == 0;
		}
	}

	// move requested file
	if (::rename(strSourcePath.c_str(), strDestPath.c_str()) != 0)
	{
		if (bFileExistsInDest)
		{
			// move failed, attempt to restore file from other location
			::rename(strMovedFilePath.c_str(), strDestPath.c_str());
		}
		return false;
	}

	// remove file in other location
	if (bFileExistsInDest)
	{
		remove(strMovedFilePath);
	}

	return true;
}

string						File::findFile(string& strFileName, string& strFolderPath)
{
	string strFolderPath2 = Path::slash(strFolderPath);
	
	// check if file exists in root folder
	string strPotentialFilePath = strFolderPath + strFileName;
	if (isFile(strPotentialFilePath))
	{
		return strPotentialFilePath;
	}

	// search deep folders for file
	vector<string> vecFolderPaths = Folder::getFolderPaths(strFolderPath, true);
	for (string& strFolderPath2 : vecFolderPaths)
	{
		strPotentialFilePath = strFolderPath2 + strFileName;
		if (isFile(strPotentialFilePath))
		{
			return strPotentialFilePath;
		}
	}

	// file not found
	return "";
}

string							File::findImageFile(string& strFileNameWithoutExtension, string& strFolderPath)
{
	strFolderPath = Path::slash(strFolderPath);

	string strFilePath;
	vector<string> vecImageExtensions = {
		"bmp",
		"png",
		"dds",
		"png",
		"gif",
		"jpg",
		"jpeg",
		"tga",
		"pgm",
		"ppm",
		"hdr"
	}; // must match list in ImageManager::loadImageFromFile

	// check if file exists in root folder
	for (string& strImageExtension : vecImageExtensions)
	{
		strFilePath = strFolderPath + strFileNameWithoutExtension + "." + strImageExtension;
		if (isFile(strFilePath))
		{
			return strFilePath;
		}
	}

	// search deep folders for file
	vector<string> vecFolderPaths = Folder::getFolderPaths(strFolderPath, true);
	for (string& strFolderPath2 : vecFolderPaths)
	{
		for (string& strImageExtension : vecImageExtensions)
		{
			strFilePath = strFolderPath2 + strFileNameWithoutExtension + "." + strImageExtension;
			if (isFile(strFilePath))
			{
				return strFilePath;
			}
		}
	}

	// file not found
	return "";
}

uint32					File::getCreationDate(string& strFilePath)
{
	FILETIME fileTimeCreation, fileTimeLastAccess, fileTimeLastModified;
	FILE *pFile = nullptr;
	fopen_s(&pFile, strFilePath.c_str(), "rb");
	if (!pFile)
	{
		return 0;
	}

	int iResult = GetFileTime(pFile, &fileTimeCreation, &fileTimeLastAccess, &fileTimeLastModified);
	fclose(pFile);
	if (iResult == 0)
	{
		return 0;
	}

	GetSystemTimeAsFileTime(&fileTimeCreation);
	__int64* val = (__int64*)&fileTimeCreation;
	uint32 uiFileCreationDate = (uint32)(static_cast<uint32>(*val) / 10000000.0 - 11644473600.0);   // epoch is Jan. 1, 1601: 134774 days to Jan. 1, 1970
	return uiFileCreationDate;
}

void					File::createLink(string& strFilePath, string& strLinkFilePath, string& strLinkDescription)
{
	// https://stackoverflow.com/questions/3906974/how-to-programmatically-create-a-shortcut-using-win32

	static bool bCoInitialized = false; // todo - initialize this elsewhere
	if (!bCoInitialized)
	{
		CoInitialize(NULL);
		bCoInitialized = true;
	}

	HRESULT hres;
	IShellLink* psl;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description.
		psl->SetPath(String::convertStdStringToStdWString(strFilePath).c_str());
		psl->SetDescription(String::convertStdStringToStdWString(strLinkDescription).c_str());

		// Query IShellLink for the IPersistFile interface, used for saving the
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode.
			MultiByteToWideChar(CP_ACP, 0, strLinkFilePath.c_str(), -1, wsz, MAX_PATH);

			// Save the link by calling IPersistFile::Save.
			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
}