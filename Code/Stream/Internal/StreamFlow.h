#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "EStreamDeviceType.h"

class mx::StreamFlow
{
private:
	EStreamDeviceType		m_uiDeviceType;

public:
	StreamFlow(EStreamDeviceType uiDeviceType);

	virtual void			prepare(std::string& strFilePath, bool bCheckForErrors) = 0;
	virtual void			prepare(char* pBuffer) = 0;

	virtual bool			isOpen() = 0;
	virtual void			open() = 0;
	virtual void			close() = 0;

	virtual uint64			writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite) = 0;
	virtual uint64			readOnce(uint8* pBuffer, uint64 uiByteCountToRead) = 0;

	virtual void			writeAll(const uint8* pBuffer, uint64 uiByteCountToWrite) = 0;
	virtual void			readAll(uint8* pBuffer, uint64 uiByteCountToRead) = 0;

	virtual void			seek(uint64 uiIndex) = 0;
	virtual uint64			size() = 0;

	virtual void			flush() = 0;
};