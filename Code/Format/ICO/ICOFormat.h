#pragma once

#include "mx.h"
#include "Types.h"
#include "Format/BMP/BMPFormat.h"
#include "Static/Components/ImageData.h"

class mx::ICOFormat : public mx::BMPFormat
{
public:
	ICOFormat(mx::Stream& stream);
	ICOFormat(mx::Stream& stream, mx::ImageData& image);

	static ICOFormat* createFromBMP(BMPFormat* pBMPFormat);

private:
	void							_unserialize(void);
	void							_serialize(void);
};