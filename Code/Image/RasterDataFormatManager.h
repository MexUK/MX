#pragma once

#include "mx.h"
#include "Image/RasterDataFormat.h"
#include "Pool/VectorPool.h"

class mx::RasterDataFormatManager : public mx::VectorPool<mx::RasterDataFormat*>
{
public:
	void							init(void);
	void							uninit(void);

private:
	void							initRasterDataFormats(void);
};