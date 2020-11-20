#pragma once

#include "mx.h"
#include "Types.h"
#include "Format/Image/BMP/BMPFormat.h"

class mx::CURFormat : public BMPFormat
{
public:
	CURFormat(mx::Stream& stream);

	void							_unserialize(void);
	void							_serialize(void);

	static CURFormat*				createFormatFromBMP(BMPFormat *pBMPFormat);
};