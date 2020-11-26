#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>

class mx::GUI
{
public:
	static void								setParentWindow(HWND hParentWindow) { m_hParentWindow = hParentWindow; }
	static HWND								getParentWindow(void) { return m_hParentWindow; }

	static std::vector<std::string>			openFile(std::string& strInitialDirKey = mx::g_strBlankString, std::string& strExtensionFilters = mx::g_strBlankString, bool bAllowMultiSelect = true, std::string& strDefaultFileName = mx::g_strBlankString);
	static std::string						saveFile(std::string& strInitialDirKey = mx::g_strBlankString, std::string& strExtensionFilters = mx::g_strBlankString, std::string& strDefaultFileName = mx::g_strBlankString);
	static std::string						chooseFolder(HWND hOwnerHwnd, std::string& strTitle, std::string& strInitialDir = mx::g_strBlankString);
	static int CALLBACK						onChooseFolderDialogInit(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	static uint32							showMessage(std::string strMessage, std::string strTitle, uint32 uiButtonType = MB_OKCANCEL);

private:
	static HWND								m_hParentWindow;
};