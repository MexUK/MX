#include "DataPath.h"
#include <Shlobj.h> // for getLocalAppDataPath()
#include <sstream> // for getLocalAppDataPath()
#include "Static/Path.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

// folder paths
string					DataPath::getLocalAppDataPath(void)
{
	wchar_t *pLocalAppDataFolderPath = 0;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pLocalAppDataFolderPath);
	wstringstream ss;
	ss << pLocalAppDataFolderPath;
	wstring wstr = ss.str();
	string str = String::convertStdWStringToStdString(wstr);
	string strLocalAppFolderPath = Path::slash(str);
	CoTaskMemFree(static_cast<void*>(pLocalAppDataFolderPath));
	return Path::slash(strLocalAppFolderPath);
}

string					DataPath::getDesktopFolderPath(void)
{
	static char szFilePath[MAX_PATH + 1];
	if (SHGetSpecialFolderPathA(HWND_DESKTOP, szFilePath, CSIDL_DESKTOP, FALSE))
	{
		string str = szFilePath;
		return Path::slash(str);
	}
	else
	{
		return "";
	}
}

string					DataPath::getStartMenuFolderPath(void)
{
	static char szFilePath[MAX_PATH + 1];
	if (SHGetSpecialFolderPathA(HWND_DESKTOP, szFilePath, CSIDL_PROGRAMS, FALSE))
	{
		string str = szFilePath;
		return Path::slash(str);
	}
	else
	{
		return "";
	}
}