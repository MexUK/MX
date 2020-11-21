#include "Image.h"
#include "Static/Dir.h"
#include "squish.h"
#include "Stream/Stream.h"
#include "Format/DDS/DDSFormat.h"

using namespace mx;
using namespace squish;

Gdiplus::GdiplusStartupInput mx::Image::m_startupInput;
ULONG_PTR mx::Image::m_uiToken;

// format
void mx::Image::convert(std::string& strPathIn, std::string& strPathOut)
{
	std::string strExt = Path::ext(strPathOut);
	convertImageFormat(strPathIn, strPathOut, getEncodingTypeFromFileExtension(strExt));
}

void mx::Image::convertToBMP(std::string& strPathIn, std::string& strPathOut)
{
	convertImageFormat(strPathIn, strPathOut, L"image/bmp");
}

void mx::Image::convertToJPG(std::string& strPathIn, std::string& strPathOut)
{
	convertImageFormat(strPathIn, strPathOut, L"image/jpeg");
}

void mx::Image::convertToPNG(std::string& strPathIn, std::string& strPathOut)
{
	convertImageFormat(strPathIn, strPathOut, L"image/png");
}

void mx::Image::convertToGIF(std::string& strPathIn, std::string& strPathOut)
{
	convertImageFormat(strPathIn, strPathOut, L"image/gif");
}

void mx::Image::convertToDDS(std::string& strPathIn, std::string& strPathOut)
{
	// TODO
	/*
	int width, height, channels;
	int forceChannels = 4;// SOIL_LOAD_AUTO;
	unsigned char *pData = SOIL_load_image(strPathIn.c_str(), &width, &height, &channels, forceChannels);
	if (!pData)
		return;

	saveDDSDXT1(strPathOut, width, height, channels, pData);
	*/
}

void mx::Image::saveDDSDXT1(std::string& strFilePathOut, int width, int height, int channels, unsigned char* pData)
{
	Dir::create(strFilePathOut);

	//SOIL_save_image(strPathOut.c_str(), SOIL_SAVE_TYPE_DDS, width, height, channels, pData);

	unsigned char* pDataOut = new unsigned char[(width * height) / 2];
	squish::CompressImage(pData, width, height, pDataOut, kDxt1);

	Stream s(strFilePathOut);
	DDSFormat format(s);
	format.m_uiWidth = width;
	format.m_uiHeight = height;
	format.m_uiBPP = 4;
	format.m_strRasterData = std::string((char*)pDataOut, (width * height) / 2);
	format.serialize();
}

// size
void mx::Image::resize(std::string& strPathIn, std::string& strPathOut, int iWidth, int iHeight)
{
	loadGdiplus();

	{
		Gdiplus::Image* pImage = loadImage(strPathIn);

		Gdiplus::Image* pNewImage = new Gdiplus::Bitmap(iWidth, iHeight);
		Gdiplus::Graphics g(pNewImage);
		g.DrawImage(pImage, 0, 0, iWidth, iHeight);

		saveImage(strPathOut, pNewImage);

		delete pImage;
		delete pNewImage;
	}

	unloadGdiplus();
}

void mx::Image::resize(std::string& strPathIn, std::string& strPathOut, float fScale)
{
	loadGdiplus();

	{
		Gdiplus::Image* pImage = loadImage(strPathIn);

		int iNewWidth = (int)(fScale * ((float)pImage->GetWidth()));
		int iNewHeight = (int)(fScale * ((float)pImage->GetHeight()));

		Gdiplus::Image* pNewImage = new Gdiplus::Bitmap(iNewWidth, iNewHeight);
		Gdiplus::Graphics g(pNewImage);
		g.DrawImage(pImage, 0, 0, iNewWidth, iNewHeight);

		saveImage(strPathOut, pNewImage);

		delete pNewImage;
	}

	unloadGdiplus();
}

// bulk
void mx::Image::convertImages(std::string& strDirIn, std::string& strDirOut, std::string strTargetImageExtension)
{
	std::string strDirInSlashed = Path::slash(strDirIn);
	std::string strDirOutSlashed = Path::slash(strDirOut);
	std::vector<std::string> vecFileNames = Dir::getFileNames(strDirInSlashed, "");
	for (std::string& strFileName : vecFileNames)
	{
		std::string strImageFilePathIn = strDirInSlashed + strFileName;
		std::string strImageFilePathOut = strDirOutSlashed + Path::setExt(strFileName, strTargetImageExtension);
		convert(strImageFilePathIn, strImageFilePathOut);
	}
}

void mx::Image::resizeImages(std::string& strDirIn, std::string& strDirOut, int iWidth, int iHeight)
{
	std::string strDirInSlashed = Path::slash(strDirIn);
	std::string strDirOutSlashed = Path::slash(strDirOut);
	std::vector<std::string> vecFileNames = Dir::getFileNames(strDirInSlashed, "");
	for (std::string& strFileName : vecFileNames)
	{
		std::string strImageFilePathIn = strDirInSlashed + strFileName;
		std::string strImageFilePathOut = strDirOutSlashed + strFileName;
		resize(strImageFilePathIn, strImageFilePathOut, iWidth, iHeight);
	}
}

void mx::Image::resizeImages(std::string& strDirIn, std::string& strDirOut, float fScale)
{
	std::string strDirInSlashed = Path::slash(strDirIn);
	std::string strDirOutSlashed = Path::slash(strDirOut);
	std::vector<std::string> vecFileNames = Dir::getFileNames(strDirInSlashed, "");
	for (std::string& strFileName : vecFileNames)
	{
		std::string strImageFilePathIn = strDirInSlashed + strFileName;
		std::string strImageFilePathOut = strDirOutSlashed + strFileName;
		resize(strImageFilePathIn, strImageFilePathOut, fScale);
	}
}

// internal
void mx::Image::loadGdiplus()
{
	Gdiplus::GdiplusStartup(&m_uiToken, &m_startupInput, nullptr);
}

void mx::Image::unloadGdiplus()
{
	Gdiplus::GdiplusShutdown(m_uiToken);
}

Gdiplus::Image* mx::Image::loadImage(std::string& strPathIn)
{
	return new Gdiplus::Image(String::convertStdStringToStdWString(strPathIn).c_str(), FALSE);
}

void mx::Image::saveImage(std::string& strPathOut, Gdiplus::Image *pImageOut)
{
	std::string strExt = Path::ext(strPathOut);
	
	CLSID clsid;
	if (GetEncoderClsid(getEncodingTypeFromFileExtension(strExt).c_str(), &clsid) == -1)
	{
		unloadGdiplus();
		return;
	}

	std::string strDir = Path::getDirectory(strPathOut);
	Dir::create(strDir);

	pImageOut->Save(String::convertStdStringToStdWString(strPathOut).c_str(), &clsid);
}

void mx::Image::convertImageFormat(std::string& strPathIn, std::string& strPathOut, std::wstring wstrEncodingTypeOut)
{
	loadGdiplus();

	Gdiplus::Image *pImage = loadImage(strPathIn);
	saveImage(strPathOut, pImage);

	unloadGdiplus();
}

std::wstring mx::Image::getEncodingTypeFromFileExtension(std::string& strFileExtension)
{
	std::string strExtUpper = String::upper(strFileExtension);
	if (strExtUpper == "BMP")
		return L"image/bmp";
	else if (strExtUpper == "JPG" || strExtUpper == "JPEG")
		return L"image/jpeg";
	else if (strExtUpper == "PNG")
		return L"image/png";
	else if (strExtUpper == "GIF")
		return L"image/gif";
	else if (strExtUpper == "DDS")
		return L"image/dds";
	else
		return L"";
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;		  // number of image encoders
	UINT  size = 0;		 // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	free(pImageCodecInfo);
	return -1;
}