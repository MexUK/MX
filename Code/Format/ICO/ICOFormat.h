#pragma once

#include "mx.h"
#include "Types.h"
#include "Format/BMP/BMPFormat.h"

class mx::ICOFormat : public mx::BMPFormat
{
public:
	ICOFormat(mx::Stream& stream);

private:
	void							_unserialize(void);
	void							_serialize(void);

	static ICOFormat*				createFormatFromBMP(BMPFormat* pBMPFormat);
};