#include "ICOFormat.h"
#include "Data/Reader.h"
#include "Data/Writer.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

ICOFormat::ICOFormat(mx::Stream& stream) :
	BMPFormat(stream)
{
	setShouldSerializeHeader1(false);
	setBMPVersion(3);
}

ICOFormat::ICOFormat(mx::Stream& stream, ImageData& image) :
	BMPFormat(stream, image)
{
	setShouldSerializeHeader1(false);
	setBMPVersion(3);
}

// serialization
void			ICOFormat::_unserialize(void)
{
	m_reader.ui16();
	uint16 uiImageType = m_reader.ui16();
	uint16 uiImageCount = m_reader.ui16();
	setWidth(m_reader.ui8());
	setHeight(m_reader.ui8());
	uint8 uiPaletteColourCount = m_reader.ui8();
	uint8 uiReserved = m_reader.ui8();
	uint16 xHotspot = m_reader.ui16();
	uint16 yHotspot = m_reader.ui16();
	uint32 uiBPP = m_reader.ui32();
	uint32 uiBMPDataFileOffset = m_reader.ui32();
	BMPFormat::_unserialize();
}

void			ICOFormat::_serialize(void)
{
	m_writer.ui16(0);
	m_writer.ui16(1); // 1 for icon, 2 for cursor
	m_writer.ui16(1); // image count
	m_writer.ui8((uint8)getWidth());
	m_writer.ui8((uint8)getHeight());
	m_writer.ui8(0); // palette colour count
	m_writer.ui8(0); // reserved
	m_writer.ui16((uint8)getWidth() / 2); // x hotspot
	m_writer.ui16((uint8)getHeight() / 2); // y hotspot
	m_writer.ui32((getWidth() * getHeight()) * (32 / 8)); // BPP
	m_writer.ui32(22); // offset to BMP data from beginning of file
	BMPFormat::_serialize();
}

// create from BMP
ICOFormat* ICOFormat::createFromBMP(BMPFormat* pBMPFormat)
{
	Stream stream;
	ICOFormat* pICOFormat = new ICOFormat(stream);
	pICOFormat->setImage(pBMPFormat->getImage());
	pICOFormat->setBMPVersion(pBMPFormat->getBMPVersion());
	pICOFormat->setColourPlaneCount(pBMPFormat->getColourPlaneCount());
	return pICOFormat;
}