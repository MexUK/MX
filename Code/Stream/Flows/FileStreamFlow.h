#pragma once

#include <string>
#include <fstream>

#include "mx.h"
#include "Types.h"
#include "Stream/Internal/StreamFlow.h"

class mx::FileStreamFlow : public mx::StreamFlow
{
private:
	std::string				m_strFilePath;
	std::fstream			m_file;

public:
	FileStreamFlow();
	~FileStreamFlow();

	virtual void			prepare(std::string& strFilePath) override;
	virtual void			prepare(char* pBuffer) override;

	virtual bool			isOpen() override;
	virtual void			open() override;
	virtual void			close() override;

	virtual uint64			writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite) override;
	virtual uint64			readOnce(uint8* pBuffer, uint64 uiMaxLength) override;

	virtual void			writeAll(const uint8* pBuffer, uint64 uiByteCountToWrite) override;
	virtual void			readAll(uint8* pBuffer, uint64 uiLength) override;

	virtual void			seek(uint64 uiIndex) override;

	virtual void			flush() override;

private:
	void					ensureOpen(void);
};