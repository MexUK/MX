#pragma warning(disable : 4005)

#include "GUI.h"
#include "Static/String.h"
#include "Static/Path.h"
#include "Static/Registry.h"
#include "Static/DataPath.h"
#include "Shlobj.h"
#include <ShObjIdl.h>

using namespace std;
using namespace mx;

HWND				GUI::m_hParentWindow = nullptr;

// file/folder windows
vector<string>		GUI::openFile(string& strInitialDirKey, string& strExtensionFilters, bool bAllowMultiSelect, string& strDefaultFileName)
{
	string str = "";
	string strSearchIn = Path::slash(str);
	string strFind = "/";
	string strReplaceWith = "\\";
	string strInitialDir = String::replace(strSearchIn, strFind, strReplaceWith);
	string strComma = ",";
	vector<string> vecFilePaths;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// set initial directory
			wchar_t wszInitialDir[MAX_PATH];
			wmemset(wszInitialDir, L'\0', MAX_PATH);
			wmemcpy(wszInitialDir, String::atow(strInitialDir).c_str(), strInitialDir.length());

			IShellItem *psiFolder = nullptr;
			hr = SHCreateItemFromParsingName(wszInitialDir, NULL, IID_PPV_ARGS(&psiFolder));
			if (SUCCEEDED(hr))
				pFileOpen->SetFolder(psiFolder);

			// set extensions filter
			vector<string> vecExtensionFilters2 = String::split(strExtensionFilters, strComma);
			COMDLG_FILTERSPEC *pFileTypes = new COMDLG_FILTERSPEC[vecExtensionFilters2.size()];
			uint32 i = 0;
			for (string& strExtensionFilter : vecExtensionFilters2)
			{
				wchar_t *wszExtFilter_Name = new wchar_t[50];
				wmemset(wszExtFilter_Name, L'\0', 50);
				str = strExtensionFilter + " Files";
				wmemcpy(wszExtFilter_Name, String::atow(str).c_str(), (strExtensionFilter + " Files").length());

				wchar_t *wszExtFilter_Spec = new wchar_t[50];
				wmemset(wszExtFilter_Spec, L'\0', 50);
				str = "*." + strExtensionFilter;
				wmemcpy(wszExtFilter_Spec, String::atow(str).c_str(), ("*." + strExtensionFilter).length());

				pFileTypes[i].pszName = wszExtFilter_Name;
				pFileTypes[i].pszSpec = wszExtFilter_Spec;
				i++;
			}
			pFileOpen->SetFileTypes(vecExtensionFilters2.size(), pFileTypes);

			// multiple file selection
			if (bAllowMultiSelect)
			{
				pFileOpen->SetOptions(FOS_ALLOWMULTISELECT);
			}

			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				if (bAllowMultiSelect)
				{
					// multiple file paths
					IShellItemArray *psiaResult;
					hr = pFileOpen->GetResults(&psiaResult);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath = NULL;
						DWORD dwNumItems = 0; // number of items in multiple selection
						wstring strSelected; // will hold file paths of selected items

						hr = psiaResult->GetCount(&dwNumItems);  // get number of selected items

																 // Loop through IShellItemArray and construct string for display
						for (DWORD i = 0; i < dwNumItems; i++)
						{
							IShellItem *psi = NULL;

							hr = psiaResult->GetItemAt(i, &psi); // get a selected item from the IShellItemArray

							if (SUCCEEDED(hr))
							{
								hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

								if (SUCCEEDED(hr))
								{
									wstring wstr = pszFilePath;
									vecFilePaths.push_back(String::wtoa(wstr));
									CoTaskMemFree(pszFilePath);
								}

								psi->Release();
							}
						}

						psiaResult->Release();
					}
				}
				else
				{
					// single file path
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							wstring wstr = pszFilePath;
							vecFilePaths.push_back(String::wtoa(wstr));
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}


			}
			
			for(uint32 i = 0, j = vecExtensionFilters2.size(); i < j; i++)
			{
				delete [] pFileTypes[i].pszName;
				delete [] pFileTypes[i].pszSpec;
			}
			
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	if (vecFilePaths.size() > 0)
	{
		string str = vecFilePaths[0];
		str = Path::getDirectory(str);
	}

	return vecFilePaths;
}

string				GUI::saveFile(string& strInitialDirKey, string& strExtensionFilters, string& strDefaultFileName)
{
	string str = "";
	string strSearchIn = Path::slash(str);
	string strFind = "/";
	string strReplaceWith = "\\";
	string strInitialDir = String::replace(strSearchIn, strFind, strReplaceWith);
	string strComma = ",";
	string strChosenFilePath;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr))
		{
			// set initial directory
			wchar_t wszInitialDir[MAX_PATH];
			wmemset(wszInitialDir, L'\0', MAX_PATH);
			wmemcpy(wszInitialDir, String::atow(strInitialDir).c_str(), strInitialDir.length());

			IShellItem *psiFolder = nullptr;
			hr = SHCreateItemFromParsingName(wszInitialDir, NULL, IID_PPV_ARGS(&psiFolder));
			if (SUCCEEDED(hr))
				pFileSave->SetFolder(psiFolder);

			// set extensions filter
			vector<string> vecExtensionFilters2 = String::split(strExtensionFilters, strComma);
			COMDLG_FILTERSPEC *pFileTypes = new COMDLG_FILTERSPEC[vecExtensionFilters2.size()];
			uint32 i = 0;
			for (string& strExtensionFilter : vecExtensionFilters2)
			{
				wchar_t *wszExtFilter_Name = new wchar_t[50];
				wmemset(wszExtFilter_Name, L'\0', 50);
				str = strExtensionFilter + " Files";
				wmemcpy(wszExtFilter_Name, String::atow(str).c_str(), (strExtensionFilter + " Files").length());

				wchar_t *wszExtFilter_Spec = new wchar_t[50];
				wmemset(wszExtFilter_Spec, L'\0', 50);
				str = "*." + strExtensionFilter;
				wmemcpy(wszExtFilter_Spec, String::atow(str).c_str(), ("*." + strExtensionFilter).length());

				pFileTypes[i].pszName = wszExtFilter_Name;
				pFileTypes[i].pszSpec = wszExtFilter_Spec;
				i++;
			}
			pFileSave->SetFileTypes(vecExtensionFilters2.size(), pFileTypes);

			// Show the Open dialog box.
			hr = pFileSave->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				// single file path
				IShellItem *pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						wstring wstr = pszFilePath;
						strChosenFilePath = String::wtoa(wstr);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			
			for(uint32 i = 0, j = vecExtensionFilters2.size(); i < j; i++)
			{
				delete [] pFileTypes[i].pszName;
				delete [] pFileTypes[i].pszSpec;
			}
			
			pFileSave->Release();
		}
		CoUninitialize();
	}

	if (strChosenFilePath != "")
	{
		string str = Path::getDirectory(strChosenFilePath);
	}

	return strChosenFilePath;
}

string				GUI::chooseFolder(HWND hOwnerHwnd, string& strTitle, string& strInitialDir)
{
	wstring szTitle = String::atow(strTitle);
	string strFind = "/";
	string strReplaceWith = "\\";
	string str = String::replace(strInitialDir, strFind, strReplaceWith);
	wstring wstrInitialDir = String::atow(str);

	BROWSEINFO bi = { 0 };
	bi.hwndOwner = hOwnerHwnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = szTitle.c_str();
	bi.ulFlags = BIF_USENEWUI;
	bi.lpfn = onChooseFolderDialogInit;
	bi.lParam = (LPARAM)wstrInitialDir.c_str();
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		wchar_t szFolderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, szFolderPath))
		{
			wstring wstr = szFolderPath;
			string str = String::wtoa(wstr);
			return Path::slash(str);
		}
	}

	return "";
}

int CALLBACK		GUI::onChooseFolderDialogInit(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}

// message windows
uint32				GUI::showMessage(string strMessage, string strTitle, uint32 uiButtonType)
{
	return MessageBox(m_hParentWindow, String::atow(strMessage).c_str(), String::atow(strTitle).c_str(), uiButtonType);
}