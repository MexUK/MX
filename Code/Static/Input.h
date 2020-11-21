#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>

class mx::Input
{
public:
	static void								setParentWindowHwnd(HWND hParentWindowHwnd) { m_hParentWindowHwnd = hParentWindowHwnd; }
	static HWND								getParentWindowHwnd(void) { return m_hParentWindowHwnd; }

	static std::vector<std::string>			openFile(std::string& strInitialDirKey = mx::g_strBlankString, std::string& strExtensionFilters = mx::g_strBlankString, bool bAllowMultiSelect = true, std::string& strDefaultFileName = mx::g_strBlankString);
	static std::string						saveFile(std::string& strInitialDirKey = mx::g_strBlankString, std::string& strExtensionFilters = mx::g_strBlankString, std::string& strDefaultFileName = mx::g_strBlankString);
	static std::string						openFolder(std::string& strTitle, std::string& strInitialDirKey = mx::g_strBlankString);
	static std::string						saveFolder(std::string& strTitle, std::string& strInitialDirKey = mx::g_strBlankString);

	static std::string						chooseFolderDialog(HWND hOwnerHwnd, std::string& strTitle, std::string& strInitialDir = mx::g_strBlankString);
	static int CALLBACK						onChooseFolderDialogInit(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	static uint32							showMessage(std::string strMessage, std::string strTitle, uint32 uiButtonType = MB_OKCANCEL);
	
	static bool								isControlKeyDown(void);
	static bool								isShiftKeyDown(void);
	static bool								isAltKeyDown(void);
	static bool								isCapsLockOn(void);
	static bool								isTextInputUppercase(void);
	static std::string						getTextInInputCase(std::string& strText);

private:
	static HWND								m_hParentWindowHwnd;
};