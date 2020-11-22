#pragma once

#include "mx.h"
#include "Types.h"
#include "Format/BMP/BMPFormat.h"
#include "Static/Components/ImageData.h"

class mx::CURFormat : public mx::BMPFormat
{
public:
	CURFormat(mx::Stream& stream);
	CURFormat(mx::Stream& stream, mx::ImageData& image);

	static CURFormat* createFromBMP(BMPFormat* pBMPFormat);

private:
	void							_unserialize(void);
	void							_serialize(void);
};