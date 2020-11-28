#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Stream/Internal/StreamFlow.h"

class mx::MemoryStreamFlow : public mx::StreamFlow
{
private:
	char*					m_pBuffer;
	uint64					m_uiSeek;

public:
	MemoryStreamFlow();

	virtual void			prepare(std::string& strFilePath, bool bCheckForErrors) override;
	virtual void			prepare(char* pBuffer) override;

	virtual bool			isOpen() override;
	virtual void			open() override;
	virtual void			close() override;

	virtual uint64			writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite) override;
	virtual uint64			readOnce(uint8* pBuffer, uint64 uiByteCountToRead) override;

	virtual void			writeAll(const uint8* pBuffer, uint64 uiByteCountToWrite) override;
	virtual void			readAll(uint8* pBuffer, uint64 uiByteCountToRead) override;

	virtual void			seek(uint64 uiIndex) override;
	virtual uint64			size() override;

	virtual void			flush() override;
};