#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Stream/Internal/StreamFlow.h"

class mx::MemoryStreamFlow : public mx::StreamFlow
{
private:
	char*					m_pBuffer;

public:
	MemoryStreamFlow();

	virtual void			prepare(std::string& strFilePath) override;
	virtual void			prepare(char* pBuffer) override;

	virtual bool			isOpen() override;
	virtual void			open() override;
	virtual void			close() override;

	virtual uint64			writeOnce(const uint8* pBuffer, uint64 uiMaxLength) override;
	virtual uint64			readOnce(uint8* pBuffer, uint64 uiMaxLength) override;

	virtual void			writeAll(const uint8* pBuffer, uint64 uiLength) override;
	virtual void			readAll(uint8* pBuffer, uint64 uiLength) override;

	virtual void			seek(uint64 uiIndex) override;

	virtual void			flush() override;
};