#pragma once

#include "mx.h"
#include "Types.h"
#include "Static/String.h"
#include "Static/Path.h"
#include <string>

int GetEncoderClsid2(const WCHAR* format, CLSID* pClsid);

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

private:
	static void convertImageFormat(std::string& strPathIn, std::string& strPathOut, std::wstring wstrEncodingTypeOut);

	static std::wstring getEncodingTypeFromFileExtension(std::string& strFileExtension);

	static void loadGdiplus();
	static void unloadGdiplus();
	
	static Gdiplus::Image* loadImage(std::string& strPathIn);
	static void saveImage(std::string& strPathOut, Gdiplus::Image* pImageOut);

	static void saveDDSDXT1(std::string& strFilePathOut, int width, int height, int channels, unsigned char *pData);

private:
	static Gdiplus::GdiplusStartupInput m_startupInput;
	static ULONG_PTR m_uiToken;
};