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
	return ui[0] + (ui[1] * OCTET_COMBINATION_COUNT);
}

uint16					Reader::ui16be()
{
	uint8 ui[2];
	m_stream.readAll(ui, 2);
	return ui[1] + (ui[0] * OCTET_COMBINATION_COUNT);
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
	return ui[0] + (ui[1] * OCTET_COMBINATION_COUNT) + (ui[2] * TWO_OCTETS_COMBINATION_COUNT);
}

uint32					Reader::ui24be()
{
	uint8 ui[3];
	m_stream.readAll(ui, 3);
	return ui[2] + (ui[1] * OCTET_COMBINATION_COUNT) + (ui[0] * TWO_OCTETS_COMBINATION_COUNT);
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
	return ui[0] + (ui[1] * OCTET_COMBINATION_COUNT) + (ui[2] * TWO_OCTETS_COMBINATION_COUNT) + (ui[3] * THREE_OCTETS_COMBINATION_COUNT);
}

uint32					Reader::ui32be()
{
	uint8 ui[4];
	m_stream.readAll(ui, 4);
	return ui[3] + (ui[2] * OCTET_COMBINATION_COUNT) + (ui[1] * TWO_OCTETS_COMBINATION_COUNT) + (ui[0] * THREE_OCTETS_COMBINATION_COUNT);
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
	return ui[0] + (ui[1] * OCTET_COMBINATION_COUNT) + (ui[2] * TWO_OCTETS_COMBINATION_COUNT) + (ui[3] * THREE_OCTETS_COMBINATION_COUNT)
		+ (ui[4] * FOUR_OCTETS_COMBINATION_COUNT) + (ui[5] * FIVE_OCTETS_COMBINATION_COUNT) + (ui[6] * SIX_OCTETS_COMBINATION_COUNT) + (ui[7] * SEVEN_OCTETS_COMBINATION_COUNT);
}

uint64					Reader::ui64be()
{
	uint8 ui[8];
	m_stream.readAll(ui, 8);
	return ui[7] + (ui[6] * OCTET_COMBINATION_COUNT) + (ui[5] * TWO_OCTETS_COMBINATION_COUNT) + (ui[4] * THREE_OCTETS_COMBINATION_COUNT)
		+ (ui[3] * FOUR_OCTETS_COMBINATION_COUNT) + (ui[2] * FIVE_OCTETS_COMBINATION_COUNT) + (ui[1] * SIX_OCTETS_COMBINATION_COUNT) + (ui[0] * SEVEN_OCTETS_COMBINATION_COUNT);
}

// float
float32					Reader::f32()
{
	return unpack754_32(ui32le());
}

float64					Reader::f64()
{
	return unpack754_64(ui64le());
}

float80					Reader::f80()
{
	//unpack754_80(ui80le());
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
	vec[0] = f32();
	vec[1] = f32();
	return vec;
}

glm::vec3				Reader::vec3()
{
	glm::vec3 vec;
	vec[0] = f32();
	vec[1] = f32();
	vec[2] = f32();
	return vec;
}

// matrix
glm::mat4				Reader::mat4()
{
	glm::mat4 mat;

	mat[0][0] = f32();
	mat[0][1] = f32();
	mat[0][2] = f32();
	mat[0][3] = f32();

	mat[1][0] = f32();
	mat[1][1] = f32();
	mat[1][2] = f32();
	mat[1][3] = f32();

	mat[2][0] = f32();
	mat[2][1] = f32();
	mat[2][2] = f32();
	mat[2][3] = f32();

	mat[3][0] = f32();
	mat[3][1] = f32();
	mat[3][2] = f32();
	mat[3][3] = f32();

	return mat;
}