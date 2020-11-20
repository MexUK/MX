//#include <Windows.h>
#include "Registry.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

void			Registry::setRegistryValueString(string& strKey1, string& strKey2, string& strValue)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_FILE_NOT_FOUND)
	{
		RegCreateKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	RegSetValueEx(hKey, String::convertStdStringToStdWString(strKey2).c_str(), 0, REG_SZ, (BYTE*)String::convertStdStringToStdWString(strValue).c_str(), strValue.length()*sizeof(wchar_t));
	RegCloseKey(hKey);
}

string			Registry::getRegistryValueString(string& strKey1, string& strKey2)
{
	wchar_t szBuffer[16383];
	DWORD uiBufferSize = 16383;

	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), &hKey) != ERROR_SUCCESS)
	{
		return "";
	}
	if (RegQueryValueEx(hKey, String::convertStdStringToStdWString(strKey2).c_str(), NULL, NULL, (LPBYTE)szBuffer, &uiBufferSize) != ERROR_SUCCESS)
	{
		return "";
	}
	RegCloseKey(hKey);

	szBuffer[uiBufferSize] = '\0';

	wstring wstr = szBuffer;
	string strData = String::convertStdWStringToStdString(wstr);
	return strData;
}

void			Registry::setRegistryValueInt(string& strKey1, string& strKey2, int iValue)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_FILE_NOT_FOUND)
	{
		RegCreateKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	RegSetValueEx(hKey, String::convertStdStringToStdWString(strKey2).c_str(), 0, REG_DWORD, (BYTE*)&iValue, 4);
	RegCloseKey(hKey);
}

int				Registry::getRegistryValueInt(string& strKey1, string& strKey2)
{
	DWORD uiValue = 0;
	DWORD uiBufferSize = 4;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		return 0;
	}
	if (RegQueryValueEx(hKey, String::convertStdStringToStdWString(strKey2).c_str(), NULL, NULL, (LPBYTE)&uiValue, &uiBufferSize) != ERROR_SUCCESS)
	{
		return 0;
	}
	RegCloseKey(hKey);

	return uiValue;
}

void			Registry::removeRegistryValue(string& strKey1, string& strKey2)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
	{
		return;
	}

	RegDeleteValue(hKey, String::convertStdStringToStdWString(strKey2).c_str());
	RegCloseKey(hKey);
}

void			Registry::removeRegistryKey(string& strKey1, string& strKey2)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, String::convertStdStringToStdWString(strKey1).c_str(), 0, DELETE, &hKey) != ERROR_SUCCESS)
	{
		return;
	}

	RegDeleteKey(hKey, String::convertStdStringToStdWString(strKey2).c_str());
	RegCloseKey(hKey);
}

void			Registry::assoicateFileExtension(string& strFileExtension, string& strExePath, string& strAppIdentifier)
{
	string strSlashType1 = "/";
	string strSlashType2 = "\\";
	string strExePath2 = String::replace(strExePath, strSlashType1, strSlashType2);
	string strKey1, strKey2, strValue;

	strKey1 = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\." + strFileExtension;
	strKey2 = "UserChoice";
	removeRegistryKey(strKey1, strKey2);

	strKey1 = "Software\\Classes\\." + strFileExtension;
	setRegistryValueString(strKey1, g_strBlankString, strAppIdentifier);

	strKey1 = "Software\\Classes\\" + strAppIdentifier + "\\shell\\open\\command";
	strValue = "\"" + strExePath2 + "\" \"%1\"";
	setRegistryValueString(strKey1, g_strBlankString, strValue);
}

void				Registry::setSoftwareValueString(string& strKey1, string& strKey2, string& strValue)
{
	string strSoftwareKey1 = "SOFTWARE\\" + strKey1;
	return setRegistryValueString(strSoftwareKey1, strKey2, strValue);
}

string				Registry::getSoftwareValueString(string& strKey1, string& strKey2)
{
	string strSoftwareKey1 = "SOFTWARE\\" + strKey1;
	return getRegistryValueString(strSoftwareKey1, strKey2);
}

void				Registry::setSoftwareValueInt(string& strKey1, string& strKey2, int iValue)
{
	string strSoftwareKey1 = "SOFTWARE\\" + strKey1;
	return setRegistryValueInt(strSoftwareKey1, strKey2, iValue);
}

int					Registry::getSoftwareValueInt(string& strKey1, string& strKey2)
{
	string strSoftwareKey1 = "SOFTWARE\\" + strKey1;
	return getRegistryValueInt(strSoftwareKey1, strKey2);
}

void				Registry::removeSoftwareValue(string& strKey1, string& strKey2)
{
	string strSoftwareKey1 = "SOFTWARE\\" + strKey1;
	return removeRegistryValue(strSoftwareKey1, strKey2);
}