#include "DDSFormat.h"
#include "Data/Reader.h"
#include "Data/Writer.h"
#include "Static/File.h"
#include "Static/Memory.h"
#include "Format/DDS/DDSFormat_Header_DDS_HEADER.h"
#include "Format/DDS/DDSFormat_Header_DDS_PIXELFORMAT_Part1.h"
#include "Format/DDS/DDSFormat_Header_DDS_HEADER_DXT10.h"
#include "Format/DDS/DDSFormat_Header_DDS_PIXELFORMAT_Part2.h"
#include "D3D10.h"
#include "Data/EEndian.h"

using namespace std;
using namespace glm;
using namespace mx;

DDSFormat::DDSFormat(Stream& stream) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(ImageData(
		UNKNOWN_IMAGE_FORMAT,
		g_vecDefaultUvec2,
		nullptr
	))
{
}

DDSFormat::DDSFormat(Stream& stream, ImageData& image) :
	Format(stream, true, LITTLE_ENDIAN),
	m_image(image)
{
}

// serialization
void				DDSFormat::_unserialize(void)
{
	// file header
	uint32 uiMagicNumber = m_reader.ui32(); // 0x20534444

	// DDS_HEADER structure
	DDSFormat_Header_DDS_HEADER *pHeader1 = m_reader.structureHeap<DDSFormat_Header_DDS_HEADER>();

	// DDS_PIXELFORMAT structure
	DDSFormat_Header_DDS_PIXELFORMAT_Part1 *pHeader2 = m_reader.structureHeap<DDSFormat_Header_DDS_PIXELFORMAT_Part1>();

	if (pHeader2->m_uiFlags2 == 4/*DDPF_FOURCC*/ && string((char*)&pHeader2->m_ucFourCC, 4) == "DX10")
	{
		// DDS_HEADER_DXT10 structure
		DDSFormat_Header_DDS_HEADER_DXT10 *pHeader3 = m_reader.structureHeap<DDSFormat_Header_DDS_HEADER_DXT10>();
		delete pHeader3;
	}

	// DDS_HEADER structure continued
	DDSFormat_Header_DDS_PIXELFORMAT_Part2 *pHeader4 = m_reader.structureHeap<DDSFormat_Header_DDS_PIXELFORMAT_Part2>();

	// populate ImageData object
	// todo - this code only supports DXT1 images.
	m_image.m_uiFormat = COMPRESSED_RGB_DXT1;
	m_image.m_vecSize.x = pHeader1->m_uiWidth;
	m_image.m_vecSize.y = pHeader1->m_uiHeight;
	m_image.checkToAllocateData();
	m_reader.cstr((char*)m_image.m_pData, m_image.getDataSize());

	// clean up
	delete pHeader1;
	delete pHeader2;
	delete pHeader4;
}

void				DDSFormat::_serialize(void)
{
	bool bCompressedRasterData = true; // todo

	// file header
	uint32 uiMagicNumber = 0x20534444;
	m_writer.ui32(uiMagicNumber);

	// DDS_HEADER structure
	uint32
		uiSize = 124,
		uiFlags = 1 | 2 | 4 | 0x1000,
		uiPitchOrLinearSize,
		uiDepth = 0,
		uiMipMapCount = 1,
		uiReserved1 = 0
	;
	if (bCompressedRasterData)
	{
		uiFlags |= 0x80000;
		uiPitchOrLinearSize = m_image.getDataSize();
	}
	else
	{
		uiFlags |= 8;
		uiPitchOrLinearSize = m_image.m_vecSize.x * 4;
	}
	m_writer.ui32(uiSize);
	m_writer.ui32(uiFlags);
	m_writer.ui32(m_image.m_vecSize.y);
	m_writer.ui32(m_image.m_vecSize.x);
	m_writer.ui32(uiPitchOrLinearSize);
	m_writer.ui32(uiDepth);
	m_writer.ui32(uiMipMapCount);
	for (uint32 i = 0; i < 11; i++)
	{
		m_writer.ui32(uiReserved1);
	}

	// DDS_PIXELFORMAT structure
	uint32
		uiSize2 = 32,
		uiFlags2,
		uiRGBBitCount = 32,
		uiABitMask = 0xFF000000,
		uiBBitMask = 0x00FF0000,
		uiGBitMask = 0x0000FF00,
		uiRBitMask = 0x000000FF
	;
	if (bCompressedRasterData)
	{
		uiFlags2 = 0x4;
	}
	else
	{
		uiFlags2 = 0x40;
	}
	string
		strFourCC = "";
	if (bCompressedRasterData)
	{
		strFourCC = "DXT1";
	}
	else
	{
		strFourCC.append(4, '\0');
	}
	m_writer.ui32(uiSize2);
	m_writer.ui32(uiFlags2);
	m_writer.str(strFourCC);
	m_writer.ui32(uiRGBBitCount);
	m_writer.ui32(uiRBitMask);
	m_writer.ui32(uiGBitMask);
	m_writer.ui32(uiBBitMask);
	m_writer.ui32(uiABitMask);

	//if (uiFlags2 == DDPF_FOURCC && strFourCC == "DX10")
	if (uiFlags2 == 4 && strFourCC == "DX10")
	{
		// DDS_HEADER_DXT10 structure
		uint32 uiDXGIFormat = DXGI_FORMAT_R8G8B8A8_UINT;
		uint32 uiD3D10ResourceDimension = D3D10_RESOURCE_DIMENSION_TEXTURE2D;
		int32 iMiscFlag = 0;
		int32 iArraySize = 0;
		int32 iMiscFlag2 = 0;
		m_writer.ui32(uiDXGIFormat);
		m_writer.ui32(uiD3D10ResourceDimension); // D3D10_RESOURCE_DIMENSION - assume 4 bytes
		m_writer.ui32(iMiscFlag);
		m_writer.ui32(iArraySize);
		m_writer.ui32(iMiscFlag2);
	}

	// DDS_HEADER structure continued
	uint32
		uiCaps = 0x1000,
		uiCaps2 = 0,
		uiCaps3 = 0,
		uiCaps4 = 0,
		uiReserved2 = 0
	;
	m_writer.ui32(uiCaps);
	m_writer.ui32(uiCaps2);
	m_writer.ui32(uiCaps3);
	m_writer.ui32(uiCaps4);
	m_writer.ui32(uiReserved2);

	// data
	m_writer.cstr((char*)m_image.m_pData, m_image.getDataSize());
}