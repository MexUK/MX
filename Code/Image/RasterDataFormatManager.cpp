#include "RasterDataFormatManager.h"

using namespace std;
using namespace mx;

void			RasterDataFormatManager::init(void)
{
	initRasterDataFormats();
}
void			RasterDataFormatManager::uninit(void)
{
	removeAllEntries();
}

void			RasterDataFormatManager::initRasterDataFormats(void)
{
	RasterDataFormat *pRasterDataFormat;
	string str;

	pRasterDataFormat = new RasterDataFormat;
	str = "BGRA 32BPP";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGRA32);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_1");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "BGR 32BPP";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGR32);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_2");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "BGR 24BPP";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGR24);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_3");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "DXT1 (Compressed)";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT1);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_4");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "DXT3 (Compressed)";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT3);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_5");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "DXT5 (Compressed)";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT5);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_6");
	addEntry(pRasterDataFormat);

	/*
	pRasterDataFormat = new RasterDataFormat;
	str = "RGBA 4BPP";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_PAL4);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_7");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new RasterDataFormat;
	str = "RGBA 8BPP";
	pRasterDataFormat->setText(str);
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_PAL8);
	//pRasterDataFormat->setLocalizationKey("TextureFormat_8");
	addEntry(pRasterDataFormat);
	*/
}