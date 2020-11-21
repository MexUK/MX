//#include <Windows.h>
#include "Dir.h"
#include "Static/String.h"
#include "Static/Path.h"
#include "Static/File.h"
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

bool						mx::Dir::isFolder(string& strPath)
{
	uint32 uiAttributes = GetFileAttributes(String::convertStdStringToStdWString(strPath).c_str());
	return uiAttributes != INVALID_FILE_ATTRIBUTES && (uiAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
}

bool						mx::Dir::create(string& strPath)
{
	if (strPath == "")
		return false;

	string strSlashType1 = "\\";
	string strSlashType2 = "/";
	string str = String::replace(strPath, strSlashType1, strSlashType2);

	vector<string> vecFolders = String::split(str, strSlashType2);
	if (strPath.substr(strPath.length() - 1, 1) != strSlashType2)
	{
		vecFolders.erase(vecFolders.end() - 1);
	}

	string strPath2 = "";
	for (string& strFolderName : vecFolders)
	{
		strPath2 += strFolderName + strSlashType2;

		if (isFolder(strPath2))
		{
		}
		else if (File::isFile(strPath2))
		{
			return false;
		}
		else
		{
			if (!createSingle(strPath2))
				return false;
		}
	}

	return true;
}

// empty
bool						mx::Dir::isEmpty(string& strFolderPath)
{
#ifdef WIN32
	bool bEntryFound = false;
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = FindFirstFile((String::convertStdStringToStdWString(strFolderPath) + L"*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	do
	{
		if (wstring(FindFileData.cFileName) != L"." && wstring(FindFileData.cFileName) != L"..")
		{
			bEntryFound = true;
			break;
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	return !bEntryFound;
#else
	bool bEntryFound = false;
	DIR* pFolder = opendir(strFolderPath.c_str());
	do
	{
		dirent* pEntry = readdir(pFolder);
		if (dirent != nullptr)
		{
			if (string(dirent.d_name) != "." && string(dirent.d_name) != "..")
			{
				bEntryFound = true;
				break;
			}
		}
	} while (dirent != nullptr);
	closedir(pFolder);
	return !bEntryFound;
#endif
}

void						mx::Dir::empty(string& strFolderPath)
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = FindFirstFile((String::convertStdStringToStdWString(strFolderPath) + L"*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		wstring wstr = FindFileData.cFileName;

		if (wstr != L"." && wstr != L"..")
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				string str = strFolderPath + String::convertStdWStringToStdString(wstr) + "/";
				empty(str);
			}
			else
			{
				::remove((strFolderPath + String::convertStdWStringToStdString(wstr)).c_str());
			}
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	RemoveDirectory(String::convertStdStringToStdWString(strFolderPath).c_str());
}

void						mx::Dir::remove(string& strFolderPath, bool bRecursive)
{
	if (bRecursive)
	{
		WIN32_FIND_DATA FindFileData;
		string str1 = strFolderPath + "*";
		HANDLE hFind = FindFirstFile(String::convertStdStringToStdWString(str1).c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return;
		}
		do
		{
			if (FindFileData.cFileName != L"." && FindFileData.cFileName != L"..")
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					wstring wstr = FindFileData.cFileName;
					string str = strFolderPath + String::convertStdWStringToStdString(wstr) + "/";
					remove(str);
				}
				else
				{
					wstring wstr = FindFileData.cFileName;
					string str = strFolderPath + String::convertStdWStringToStdString(wstr);
					File::remove(str);
				}
			}
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	RemoveDirectory(String::convertStdStringToStdWString(strFolderPath).c_str());
}

void						mx::Dir::rename(std::string& strPath, std::string& strNewPath)
{
	return File::rename(strPath, strNewPath);
}

void						mx::Dir::copy(string& strSourceFolderPath, string& strDestinationFolderPath, string& strFileExtensionsIncluded, bool bOverwrite)
{
	vector<string>
		vecDirs;
	string
		strSourceFolderPath2 = Path::slash(strSourceFolderPath),
		strDestinationFolderPath2 = Path::slash(strDestinationFolderPath);

	if (!isFolder(strSourceFolderPath2))
	{
		return;
	}
	if (!isFolder(strDestinationFolderPath2))
	{
		create(strDestinationFolderPath2);
	}

	vecDirs.push_back(strSourceFolderPath2);
	for (uint32 i = 0; i < vecDirs.size(); i++)
	{
		for (string& strFilePath : getFilePaths(vecDirs[i], true, strFileExtensionsIncluded)) // files
		{
			string strNewFilePath = strDestinationFolderPath2 + Path::getFileName(strFilePath);
			File::copy(strFilePath, strNewFilePath, bOverwrite);
		}
		for (string& strFilePath : getFolderPaths(vecDirs[i], true)) // folders
		{
			string
				strNewFolderPath = strDestinationFolderPath2 + Path::getFolderName(strFilePath) + "/";
			copy(strFilePath, strNewFolderPath, strFileExtensionsIncluded, bOverwrite);
		}
	}
}

vector<string>				mx::Dir::getFileNames(string& strFolderPath, string strFileExtensions)
{
	string strFolderPath2 = Path::slash(strFolderPath);
	string strExtensionFilterUpper = String::upper(strFileExtensions);

	unordered_map<string, bool> umapExtensionsFilterKeyed;
	string strDelimiter = ",";
	for (string& strExtensionUpper : String::split(strExtensionFilterUpper, strDelimiter))
	{
		umapExtensionsFilterKeyed[String::trim(strExtensionUpper)] = true;
	}

	vector<string> vecFileNames;
	strFolderPath2 += "*";
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(String::convertStdStringToStdWString(strFolderPath2).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFileNames;
	}

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			// file
			wstring wstrFileName(ffd.cFileName);
			string strFileName = String::convertStdWStringToStdString(wstrFileName);
			string strFileExt = Path::ext(strFileName);
			if (strExtensionFilterUpper.length() == 0 || umapExtensionsFilterKeyed.count(String::upper(strFileExt)) == 1)
				vecFileNames.push_back(strFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFileNames;
}

vector<string>				mx::Dir::getFilePaths(string& strFolderPath, bool bDeep, string strFileExtensions)
{
	string strFolderPathSlashed = Path::slash(strFolderPath);
	string strFolderPathWildcard = strFolderPathSlashed + "*";
	string strExtensionFilterUpper = String::upper(strFileExtensions);

	vector<string> vecFilePathsToReturn;
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile((String::convertStdStringToStdWString(strFolderPathWildcard)).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFilePathsToReturn;
	}

	unordered_map<string, bool> umapExtensionsFilterKeyed;
	string strDelimiter = ",";
	for (string& strExtensionUpper : String::split(strExtensionFilterUpper, strDelimiter))
	{
		umapExtensionsFilterKeyed[String::trim(strExtensionUpper)] = true;
	}

	do
	{
		wstring wstrEntryName = ffd.cFileName;
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			// folder
			if (bDeep && wstrEntryName != L"." && wstrEntryName != L"..")
			{
				string strNextFolderPath = strFolderPathSlashed + String::convertStdWStringToStdString(wstrEntryName) + "/";
				vector<string> vec2 = getFilePaths(strNextFolderPath, true, strExtensionFilterUpper);
				vecFilePathsToReturn = StdVector::combineVectors(vecFilePathsToReturn, vec2);
			}
		}
		else
		{
			// file
			string strFileName = String::convertStdWStringToStdString(wstrEntryName);
			string strFileExt = Path::ext(strFileName);
			if (strExtensionFilterUpper.length() == 0 || umapExtensionsFilterKeyed.count(String::upper(strFileExt)) == 1)
				vecFilePathsToReturn.push_back(strFolderPathSlashed + strFileName);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFilePathsToReturn;
}

vector<string>				mx::Dir::getFolderNames(string& strFolderPath)
{
	string strFolderPathWildcard = Path::slash(strFolderPath) + "*";

	vector<string> vecFolderNames;
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(String::convertStdStringToStdWString(strFolderPathWildcard).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFolderNames;
	}

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			// folder
			wstring wstr = ffd.cFileName;
			string strFolderName = String::convertStdWStringToStdString(wstr);
			if (strFolderName != "." && strFolderName != "..")
			{
				vecFolderNames.push_back(strFolderName);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFolderNames;
}

vector<string>				mx::Dir::getFolderPaths(string& strFolderPath, bool bDeep)
{
	string strFolderPathSlashed = Path::slash(strFolderPath);
	string strFolderPathWildcard = strFolderPathSlashed + "*";

	vector<string> vecFolderPathsToReturn;
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile((String::convertStdStringToStdWString(strFolderPathWildcard)).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFolderPathsToReturn;
	}

	do
	{
		wstring wstrEntryName = ffd.cFileName;
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			// folder
			if (wstrEntryName != L"." && wstrEntryName != L"..")
			{
				string strFoundFolderPath = strFolderPathSlashed + String::convertStdWStringToStdString(wstrEntryName) + "/";
				vecFolderPathsToReturn.push_back(strFoundFolderPath);

				if (bDeep)
				{
					vector<string> vec2 = getFolderPaths(strFoundFolderPath, true);
					vecFolderPathsToReturn = StdVector::combineVectors(vecFolderPathsToReturn, vec2);
				}
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFolderPathsToReturn;
}

// internal
bool						mx::Dir::createSingle(string& strPath)
{
	return CreateDirectory(String::convertStdStringToStdWString(strPath).c_str(), NULL) != 0;
}