#include "Reader.h"
#include "Static/String.h" // temp - for unpack754_32/64/80
#include "EEndian.h"

using namespace std;
using namespace mx;

Reader::Reader(Stream& stream) :
	ReaderOrWriter(stream)
{
}

// int
uint8					Reader::ui8()
{
	uint8 ui;
	m_stream.readAll(&ui, 1);
	return ui;
}

uint16					Reader::ui16()
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui16le();
	else
		return ui16be();
}

uint16					Reader::ui16le()
{
	uint8 ui[2];
	m_stream.readAll(ui, 2);
	return ui[0] + (ui[1] * MX_OCTET_COMBINATION_COUNT);
}

uint16					Reader::ui16be()
{
	uint8 ui[2];
	m_stream.readAll(ui, 2);
	return ui[1] + (ui[0] * MX_OCTET_COMBINATION_COUNT);
}

uint32					Reader::ui24()
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui24le();
	else
		return ui24be();
}

uint32					Reader::ui24le()
{
	uint8 ui[3];
	m_stream.readAll(ui, 3);
	return ui[0] + (ui[1] * MX_OCTET_COMBINATION_COUNT) + (ui[2] * MX_TWO_OCTETS_COMBINATION_COUNT);
}

uint32					Reader::ui24be()
{
	uint8 ui[3];
	m_stream.readAll(ui, 3);
	return ui[2] + (ui[1] * MX_OCTET_COMBINATION_COUNT) + (ui[0] * MX_TWO_OCTETS_COMBINATION_COUNT);
}

uint32					Reader::ui32()
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui32le();
	else
		return ui32be();
}

uint32					Reader::ui32le()
{
	uint8 ui[4];
	m_stream.readAll(ui, 4);
	return ui[0] + (ui[1] * MX_OCTET_COMBINATION_COUNT) + (ui[2] * MX_TWO_OCTETS_COMBINATION_COUNT) + (ui[3] * MX_THREE_OCTETS_COMBINATION_COUNT);
}

uint32					Reader::ui32be()
{
	uint8 ui[4];
	m_stream.readAll(ui, 4);
	return ui[3] + (ui[2] * MX_OCTET_COMBINATION_COUNT) + (ui[1] * MX_TWO_OCTETS_COMBINATION_COUNT) + (ui[0] * MX_THREE_OCTETS_COMBINATION_COUNT);
}

uint64					Reader::ui64()
{
	if (m_uiEndian == LITTLE_ENDIAN)
		return ui64le();
	else
		return ui64be();
}

uint64					Reader::ui64le()
{
	uint8 ui[8];
	m_stream.readAll(ui, 8);
	return ui[0] + (ui[1] * MX_OCTET_COMBINATION_COUNT) + (ui[2] * MX_TWO_OCTETS_COMBINATION_COUNT) + (ui[3] * MX_THREE_OCTETS_COMBINATION_COUNT)
		+ (ui[4] * MX_FOUR_OCTETS_COMBINATION_COUNT) + (ui[5] * MX_FIVE_OCTETS_COMBINATION_COUNT) + (ui[6] * MX_SIX_OCTETS_COMBINATION_COUNT) + (ui[7] * MX_SEVEN_OCTETS_COMBINATION_COUNT);
}

uint64					Reader::ui64be()
{
	uint8 ui[8];
	m_stream.readAll(ui, 8);
	return ui[7] + (ui[6] * MX_OCTET_COMBINATION_COUNT) + (ui[5] * MX_TWO_OCTETS_COMBINATION_COUNT) + (ui[4] * MX_THREE_OCTETS_COMBINATION_COUNT)
		+ (ui[3] * MX_FOUR_OCTETS_COMBINATION_COUNT) + (ui[2] * MX_FIVE_OCTETS_COMBINATION_COUNT) + (ui[1] * MX_SIX_OCTETS_COMBINATION_COUNT) + (ui[0] * MX_SEVEN_OCTETS_COMBINATION_COUNT);
}

// float
float32					Reader::f32()
{
	return MX_THIRD_PARTY_unpack754_32(ui32le());
}

float64					Reader::f64()
{
	return MX_THIRD_PARTY_unpack754_64(ui64le());
}

float80					Reader::f80()
{
	// todo
	//MX_THIRD_PARTY_unpack754_80(ui80le());
	return 0.0f;
}

// string
char*					Reader::cstrHeap(uint64 uiLength)
{
	uint8* pBuffer = new uint8[uiLength];
	m_stream.readAll(pBuffer, uiLength);
	return (char*)pBuffer;
}

void					Reader::cstr(char* pBuffer, uint64 uiLength)
{
	m_stream.readAll((uint8*)pBuffer, uiLength);
}

string					Reader::str(uint64 uiLength)
{
	string str;
	str.resize(uiLength);
	m_stream.readAll((uint8*)str.data(), uiLength);
	return str;
}

string					Reader::mstr()
{
	uint64 uiLength = ui64le();
	string str;
	str.resize(uiLength);
	m_stream.readAll((uint8*)str.data(), uiLength);
	return str;
}

// vector
glm::vec2				Reader::vec2()
{
	glm::vec2 vec;
	cstr((char*)&vec.x, sizeof(glm::vec2));
	return vec;
}

glm::vec3				Reader::vec3()
{
	glm::vec3 vec;
	cstr((char*)&vec.x, sizeof(glm::vec3));
	return vec;
}

// matrix
glm::mat4				Reader::mat4()
{
	glm::mat4 mat;
	cstr((char*)&mat[0].x, sizeof(glm::mat4));
	return mat;
}