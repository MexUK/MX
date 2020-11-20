#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "ReaderOrWriter.h"

class mx::Writer : public mx::ReaderOrWriter
{
public:
	Writer(mx::Stream& stream);
	
	void					ui8(uint8 uiValue);
	void					ui16(uint16 uiValue);
	void					ui16le(uint16 uiValue);
	void					ui16be(uint16 uiValue);
	void					ui24(uint32 uiValue);
	void					ui24le(uint32 uiValue);
	void					ui24be(uint32 uiValue);
	void					ui32(uint32 uiValue);
	void					ui32le(uint32 uiValue);
	void					ui32be(uint32 uiValue);
	void					ui64(uint64 uiValue);
	void					ui64le(uint64 uiValue);
	void					ui64be(uint64 uiValue);
	
	void					f32(float32 fValue);
	void					f64(float64 fValue);
	void					f80(float80 fValue);
	
	void					cstr(char* pData, uint64 uiLength);
	void					str(std::string& strData);
	void					mstr(std::string& strData);

	void					vec2(glm::vec2& vec);
	void					vec3(glm::vec3& vec);

	void					mat4(glm::mat4& mat);
};