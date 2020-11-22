#pragma once

#include "mx.h"
#include "Format/Format.h"
#include "Static/Components/ImageData.h"

class mx::DDSFormat : public mx::Format
{
public:
	mx::ImageData						m_image;

public:
	DDSFormat(mx::Stream& stream);
	DDSFormat(mx::Stream& stream, mx::ImageData& image);

private:
	void								_unserialize(void);
	void								_serialize(void);
};