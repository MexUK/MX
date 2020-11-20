#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "ReaderOrWriter.h"
#include "Stream/Stream.h"

class mx::Reader : public mx::ReaderOrWriter
{
public:
	Reader(mx::Stream& stream);

	uint8					ui8();
	uint16					ui16();
	uint16					ui16le();
	uint16					ui16be();
	uint32					ui24();
	uint32					ui24le();
	uint32					ui24be();
	uint32					ui32();
	uint32					ui32le();
	uint32					ui32be();
	uint64					ui64();
	uint64					ui64le();
	uint64					ui64be();
	
	float32					f32();
	float64					f64();
	float80					f80();
	
	char*					cstrHeap(uint64 uiLength);
	void					cstr(char* pBuffer, uint64 uiLength);
	std::string				str(uint64 uiLength);
	std::string				mstr();

	glm::vec2				vec2();
	glm::vec3				vec3();

	glm::mat4				mat4();

	template <class T>
	void structure(T& structure)
	{
		cstr((char*)&structure, sizeof(T));
	};

	template <class T>
	T* structureHeap()
	{
		return (T*) cstrHeap(sizeof(T));
	};
};