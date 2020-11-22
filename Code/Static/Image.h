#pragma once

#include "mx.h"
#include "Types.h"
#include "Static/String.h"
#include "Static/Path.h"
#include "Static/Components/ImageData.h"
#include <string>

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

class mx::Image
{
public:
	static void convert(std::string& strPathIn, std::string& strPathOut);
	static void convertToBMP(std::string& strPathIn, std::string& strPathOut);
	static void convertToJPG(std::string& strPathIn, std::string& strPathOut);
	static void convertToPNG(std::string& strPathIn, std::string& strPathOut);
	static void convertToGIF(std::string& strPathIn, std::string& strPathOut);
	static void convertToDDS(std::string& strPathIn, std::string& strPathOut);

	static void resize(std::string& strPathIn, std::string& strPathOut, int iWidth, int iHeight);
	static void resize(std::string& strPathIn, std::string& strPathOut, float fScale);

	static void convertImages(std::string& strDirIn, std::string& strDirOut, std::string strTargetImageExtension);
	static void resizeImages(std::string& strDirIn, std::string& strDirOut, int iWidth, int iHeight);
	static void resizeImages(std::string& strDirIn, std::string& strDirOut, float fScale);

	static void load(std::string& strPathIn, ImageData& imageData);

private:
	static void convertImageFormat(std::string& strPathIn, std::string& strPathOut, std::wstring wstrEncodingTypeOut);

	static std::wstring getEncodingTypeFromFileExtension(std::string& strFileExtension);

	static void loadGdiplus();
	static void unloadGdiplus();
	
	static Gdiplus::Image* loadImageGDIPlus(std::string& strPathIn);
	static void saveImageGDIPlus(std::string& strPathOut, Gdiplus::Image* pImageOut);

	static void saveDDSDXT1(std::string& strFilePathOut, mx::ImageData& image);

private:
	static Gdiplus::GdiplusStartupInput m_startupInput;
	static ULONG_PTR m_uiToken;
};