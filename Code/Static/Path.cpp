#include "Path.h"
#include "Static/String.h"
#include "Static/StdVector.h"
#include <set>

using namespace std;
using namespace mx;

string			Path::replaceSlashes(string& strPath)
{
	string strFind = "\\";
	string strReplaceWith = "/";
	return String::replace(strPath, strFind, strReplaceWith);
}

string			Path::getFileName(string& strPath)
{
	string strPath2 = replaceSlashes(strPath);
	size_t uiPosition = strPath2.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return strPath2;
	}
	return strPath2.substr(uiPosition + 1);
}

string			Path::getFolderName(string& strPath)
{
	string str = Path::unslash(strPath);
	return Path::getFileName(str);
}

string			Path::getDirectory(string& strPath)
{
	string strPath2 = replaceSlashes(strPath);
	size_t uiPosition = strPath2.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return "";
	}
	return strPath2.substr(0, uiPosition + 1);
}

string			Path::removeFileName(string& strPath)
{
	string strPath2 = replaceSlashes(strPath);
	size_t uiPosition = strPath2.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return strPath2;
	}
	return strPath2.substr(0, uiPosition + 1);
}

string			Path::removeFolderName(string& strPath)
{
	string strPath2 = unslash(strPath);
	
	string strPath3 = replaceSlashes(strPath2);
	size_t uiPosition = strPath3.find_last_of("/");
	if (uiPosition != string::npos)
	{
		strPath2 = strPath2.substr(0, uiPosition + 1);
	}
	
	return slash(strPath2);
}

string			Path::replaceFolderName(string& strPath, string& strNewFolderName)
{
	return removeFolderName(strPath) + strNewFolderName;
}

string			Path::replaceFileName(string& strPath, string& strNewFileName)
{
	return removeFileName(strPath) + strNewFileName;
}

string			Path::ext(string& strPath)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return "";
	}
	return strPath.substr(uiPosition + 1);
}

bool			Path::isExt(string& strPath, string strExt)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return "";
	}
	string strExt1 = strPath.substr(uiPosition + 1);
	return String::lower(strExt1) == String::lower(strExt);
}

string			Path::setExt(string& strPath, string& strExtension)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return strPath + "." + strExtension;
	}
	return strPath.substr(0, uiPosition + 1) + strExtension;
}

string			Path::setExtWithCase(string& strPath, string& strExtension)
{
	string str = ext(strPath);
	str = String::getTextInSameCase(strExtension, str);
	return setExt(strPath, str);
}

string			Path::removeExt(string& strPath)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return strPath;
	}
	return strPath.substr(0, uiPosition);
}

bool			Path::isValidPath(string& strPath)
{
	string strPath2 = String::trim(strPath);
	
	set<char> stReservedChars = {'<', '>', '"', '|', '?', '*'}; // colon and slashes skipped
	const char *pPath = strPath2.c_str();
	for(uint32 i = 0, j = strPath2.length(); i < j; i++)
	{
		char c = pPath[i];
		if(c < 32 || c == 127)
			return false;
		if(stReservedChars.count(c) > 0)
			return false;
	}
	
	set<string> stReservedNames = { "CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};
	string str = String::upper(strPath2);
	if(stReservedNames.count(getFileName(str)) > 0)
		return false;
	
	return true;
}

bool			Path::hasSlash(string& strPath)
{
	char c = strPath.c_str()[strPath.length() - 1];
	return c == '\\' || c == '/';
}

string			Path::slash(string& strPath)
{
	if (strPath.length() > 0 && strPath.c_str()[strPath.length() - 1] != '\\' && strPath.c_str()[strPath.length() - 1] != '/')
	{
		strPath += "\\";
	}
	return strPath;
}

string			Path::unslash(string& strPath)
{
	if (strPath.length() > 0 && strPath.c_str()[strPath.length() - 1] == '\\' || strPath.c_str()[strPath.length() - 1] == '/')
	{
		return strPath.substr(0, strPath.length() - 1);
	}
	return strPath;
}

string			Path::slashFront(string& strPath)
{
	if (strPath.c_str()[0] != '\\' && strPath.c_str()[0] != '/')
	{
		strPath = "\\" + strPath;
	}
	return strPath;
}

string			Path::unslashFront(string& strPath)
{
	if (strPath.c_str()[0] == '\\' || strPath.c_str()[0] == '/')
	{
		return strPath.substr(1);
	}
	return strPath;
}

string			Path::getNextFileName(string& strFilePath, uint32 uiFilenameIndex, string& strNextSuffix)
{
	if (uiFilenameIndex == 0)
	{
		return strFilePath;
	}
	return removeExt(strFilePath) + strNextSuffix + String::toString(uiFilenameIndex + 1) + "." + ext(strFilePath);
}

vector<string>	Path::getUniqueFilePaths(vector<string>& vecFilePaths, vector<string>& vecFileNames)
{
	vecFileNames = StdVector::upper(vecFileNames);
	vector<string> vecUniqueFilePaths;
	for (string& strFilePath : vecFilePaths)
	{
		string str = getFileName(strFilePath);
		str = String::upper(str);
		if (!StdVector::isIn(vecFileNames, str))
		{
			vecUniqueFilePaths.push_back(strFilePath);
		}
	}
	return vecUniqueFilePaths;
}

bool			Path::comparePaths(string& strFilePath1, string& strFilePath2)
{
	string str1 = replaceSlashes(strFilePath1);
	string str2 = replaceSlashes(strFilePath2);
	return slash(str1) == slash(str2);
}

string			Path::getDisplayableFilePath(string& strFilePath)
{
	return replaceSlashes(strFilePath);
}