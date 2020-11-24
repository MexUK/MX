#include "Writer.h"
#include "Static/String.h" // temp - for pack754_32/64/80
#include "EEndian.h"

using namespace std;
using namespace mx;

Writer::Writer(Stream& stream) :
	ReaderOrWriter(stream)
{
}

// int
void					Writer::ui8(uint8 uiValue)
{
	m_stream.writeAll(&uiValue, 1);
}

void					Writer::ui16(uint16 uiValue)
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui16le(uiValue);
	else
		return ui16be(uiValue);
}

void					Writer::ui16le(uint16 uiValue)
{
	uint8 ui[2];
	ui[0] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_OCTET_COMBINATION_COUNT;
	m_stream.writeAll(ui, 2);
}

void					Writer::ui16be(uint16 uiValue)
{
	uint8 ui[2];
	ui[1] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[0] = uiValue / MX_OCTET_COMBINATION_COUNT;
	m_stream.writeAll(ui, 2);
}

void					Writer::ui24(uint32 uiValue)
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui24le(uiValue);
	else
		return ui24be(uiValue);
}

void					Writer::ui24le(uint32 uiValue)
{
	uint8 ui[3];
	ui[0] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[2] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 3);
}

void					Writer::ui24be(uint32 uiValue)
{
	uint8 ui[3];
	ui[2] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[0] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 3);
}

void					Writer::ui32(uint32 uiValue)
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui32le(uiValue);
	else
		return ui32be(uiValue);
}

void					Writer::ui32le(uint32 uiValue)
{
	uint8 ui[4];
	ui[0] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[2] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	ui[3] = uiValue / MX_THREE_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 4);
}

void					Writer::ui32be(uint32 uiValue)
{
	uint8 ui[4];
	ui[3] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[2] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	ui[0] = uiValue / MX_THREE_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 4);
}

void					Writer::ui64(uint64 uiValue)
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui64le(uiValue);
	else
		return ui64be(uiValue);
}

void					Writer::ui64le(uint64 uiValue)
{
	uint8 ui[8];
	ui[0] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[1] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[2] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	ui[3] = uiValue / MX_THREE_OCTETS_COMBINATION_COUNT;
	ui[4] = uiValue / MX_FOUR_OCTETS_COMBINATION_COUNT;
	ui[5] = uiValue / MX_FIVE_OCTETS_COMBINATION_COUNT;
	ui[6] = uiValue / MX_SIX_OCTETS_COMBINATION_COUNT;
	ui[7] = uiValue / MX_SEVEN_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 8);
}

void					Writer::ui64be(uint64 uiValue)
{
	uint8 ui[8];
	ui[7] = uiValue % MX_OCTET_COMBINATION_COUNT;
	ui[6] = uiValue / MX_OCTET_COMBINATION_COUNT;
	ui[5] = uiValue / MX_TWO_OCTETS_COMBINATION_COUNT;
	ui[4] = uiValue / MX_THREE_OCTETS_COMBINATION_COUNT;
	ui[3] = uiValue / MX_FOUR_OCTETS_COMBINATION_COUNT;
	ui[2] = uiValue / MX_FIVE_OCTETS_COMBINATION_COUNT;
	ui[1] = uiValue / MX_SIX_OCTETS_COMBINATION_COUNT;
	ui[0] = uiValue / MX_SEVEN_OCTETS_COMBINATION_COUNT;
	m_stream.writeAll(ui, 8);
}

// float
void					Writer::f32(float32 fValue)
{
	ui32le(MX_THIRD_PARTY_pack754_32(fValue));
}

void					Writer::f64(float64 fValue)
{
	ui64le(MX_THIRD_PARTY_pack754_64(fValue));
}

void					Writer::f80(float80 fValue)
{
	// todo
	//ui80le(MX_THIRD_PARTY_pack754_80(fValue));
}

// string
void					Writer::cstr(char* pData, uint64 uiLength)
{
	m_stream.writeAll((uint8*)pData, uiLength);
}

void					Writer::str(string& strData)
{
	m_stream.writeAll((uint8*)strData.c_str(), strData.length());
}

void					Writer::mstr(string& strData)
{
	ui64le(strData.length());
	m_stream.writeAll((uint8*)strData.c_str(), strData.length());
}

// vector
void					Writer::vec2(glm::vec2& vec)
{
	cstr((char*)&vec.x, sizeof(glm::vec2));
}

void					Writer::vec3(glm::vec3& vec)
{
	cstr((char*)&vec.x, sizeof(glm::vec3));
}

// matrix
void					Writer::mat4(glm::mat4& mat)
{
	cstr((char*)&mat[0].x, sizeof(glm::mat4));
}