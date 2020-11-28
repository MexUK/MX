#include "Image.h"
#include "Static/Dir.h"
#include "squish.h"
#include "Stream/Stream.h"
#include "Static/String.h"
#include "Format/BMP/BMPFormat.h"
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
	mx::ImageData image;
	load(strPathIn, image, 4);
	if (!image.m_pRaster)
		return;

	saveDDSDXT1(strPathOut, image);
}

void mx::Image::saveDDSDXT1(std::string& strFilePathOut, mx::ImageData& image)
{
	Dir::create(strFilePathOut);

	unsigned char* pRasterOut = new unsigned char[(image.m_vecSize.x * image.m_vecSize.y) / 2];
	squish::CompressImage(image.m_pRaster, image.m_vecSize.x, image.m_vecSize.y, pRasterOut, kDxt1);

	Stream stream(strFilePathOut);
	DDSFormat format(stream);
	format.m_image.m_uiFormat = COMPRESSED_RGB_DXT1;
	format.m_image.m_vecSize.x = image.m_vecSize.x;
	format.m_image.m_vecSize.y = image.m_vecSize.y;
	format.m_image.m_pRaster = pRasterOut;
	format.serialize();

	delete[] pRasterOut;
}

// size
void mx::Image::resize(std::string& strPathIn, std::string& strPathOut, int iWidth, int iHeight)
{
	loadGdiplus();

	{
		Gdiplus::Image* pImage = loadImageGDIPlus(strPathIn);

		Gdiplus::Image* pNewImage = new Gdiplus::Bitmap(iWidth, iHeight);
		Gdiplus::Graphics g(pNewImage);
		g.DrawImage(pImage, 0, 0, iWidth, iHeight);

		saveImageGDIPlus(strPathOut, pNewImage);

		delete pImage;
		delete pNewImage;
	}

	unloadGdiplus();
}

void mx::Image::resize(std::string& strPathIn, std::string& strPathOut, float fScale)
{
	loadGdiplus();

	{
		Gdiplus::Image* pImage = loadImageGDIPlus(strPathIn);

		int iNewWidth = (int)(fScale * ((float)pImage->GetWidth()));
		int iNewHeight = (int)(fScale * ((float)pImage->GetHeight()));

		Gdiplus::Image* pNewImage = new Gdiplus::Bitmap(iNewWidth, iNewHeight);
		Gdiplus::Graphics g(pNewImage);
		g.DrawImage(pImage, 0, 0, iNewWidth, iNewHeight);

		saveImageGDIPlus(strPathOut, pNewImage);

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

// load image
void mx::Image::load(std::string& strPathIn, ImageData& imageData, uint32 uiChannelCount)
{
	std::string strExt = Path::ext(strPathIn);
	std::string strExtUpper = String::upper(strExt);
	
	if (strExtUpper == "BMP")
	{
		Stream stream(strPathIn);
		BMPFormat bmp(stream);
		Format::unserializeFile<BMPFormat>(bmp);
		
		imageData = bmp.m_image;
		
		return;
	}
	else if (strExtUpper == "DDS")
	{
		Stream stream(strPathIn);
		DDSFormat dds(stream);
		Format::unserializeFile<DDSFormat>(dds);
		
		imageData = dds.m_image;
		
		return;
	}
	else if(strExtUpper == "PNG"
		|| strExtUpper == "GIF"
		|| strExtUpper == "JPG"
		|| strExtUpper == "JPEG"
		|| strExtUpper == "TGA"
		|| strExtUpper == "PGM"
		|| strExtUpper == "PPM"
		|| strExtUpper == "HDR")
	{
		int w, h, n;
		uint8* pRaster = stbi_load(strPathIn.c_str(), &w, &h, &n, uiChannelCount);
		if (pRaster)
		{
			if (uiChannelCount == 0)
				uiChannelCount = n;
			imageData.m_uiFormat = uiChannelCount == 3 ? UNCOMPRESSED_RGB : UNCOMPRESSED_RGBA;
			imageData.m_vecSize.x = w;
			imageData.m_vecSize.y = h;
			imageData.m_pRaster = pRaster;
			
			return;
		}
	}
	
	imageData.m_uiFormat = UNKNOWN_IMAGE_FORMAT;
	imageData.m_vecSize.x = 0;
	imageData.m_vecSize.y = 0;
	imageData.m_pRaster = nullptr;
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

Gdiplus::Image* mx::Image::loadImageGDIPlus(std::string& strPathIn)
{
	return new Gdiplus::Image(String::atow(strPathIn).c_str(), FALSE);
}

void mx::Image::saveImageGDIPlus(std::string& strPathOut, Gdiplus::Image *pImageOut)
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

	pImageOut->Save(String::atow(strPathOut).c_str(), &clsid);
}

void mx::Image::convertImageFormat(std::string& strPathIn, std::string& strPathOut, std::wstring wstrEncodingTypeOut)
{
	loadGdiplus();

	Gdiplus::Image *pImage = loadImageGDIPlus(strPathIn);
	saveImageGDIPlus(strPathOut, pImage);

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