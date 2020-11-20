#pragma once

#include "mx.h"
#include "Object/Manager.h"
#include "Image/RasterDataFormat.h"
#include "Pool/VectorPool.h"

class mx::RasterDataFormatManager : public mx::Manager, public mx::VectorPool<mx::RasterDataFormat*>
{
public:
	void							init(void);
	void							uninit(void);

private:
	void							initRasterDataFormats(void);
};