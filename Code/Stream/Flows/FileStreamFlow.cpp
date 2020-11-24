#include "FileStreamFlow.h"
#include "Stream/Errors/StreamException.h"
#include "Static/Dir.h"
#include <iostream>

using namespace std;
using namespace mx;

FileStreamFlow::FileStreamFlow() :
	StreamFlow(STREAM_DEVICE_TYPE_FILE)
{
}

FileStreamFlow::~FileStreamFlow()
{
	if (m_file.is_open())
	{
		flush();
		m_file.close();
	}
}

// prepare
void			FileStreamFlow::prepare(string& strFilePath)
{
	m_strFilePath = strFilePath;
}

void			FileStreamFlow::prepare(char* pBuffer)
{
	throw new StreamException(STREAM_ERROR_INVALID_PREPARE_SIGNATURE_FOR_DEVICE_TYPE);
}

// open/close
bool			FileStreamFlow::isOpen()
{
	return m_file.is_open();
}

void			FileStreamFlow::open()
{
	if (m_file.is_open())
		throw new StreamException(STREAM_ERROR_FILE_ALREADY_OPEN);

	if (m_strFilePath.empty())
		throw new StreamException(STREAM_ERROR_EMPTY_FILE_PATH);

	Dir::create(m_strFilePath);

	bool bBinaryMode = true;
	bool bAppendMode = false;
	bool bTruncateMode = false;

	uint32 uiOpenFlags = fstream::in | fstream::out;
	if (bBinaryMode)
		uiOpenFlags |= fstream::binary;
	if (bAppendMode)
		uiOpenFlags |= fstream::ate;
	if (bTruncateMode)
		uiOpenFlags |= fstream::trunc;

	m_file.open(m_strFilePath, uiOpenFlags);

	if (!m_file.is_open())
	{
		// create the file
		m_file.open(m_strFilePath, fstream::out | fstream::binary | fstream::trunc);
		if (m_file.is_open())
		{
			// re-open file with correct flags
			m_file.close();
			m_file.open(m_strFilePath, uiOpenFlags);
		}
	}

	seek(0);
}

void			FileStreamFlow::close()
{
	ensureOpen();

	m_file.close();
	m_strFilePath = "";
}

// write/read data once
uint64			FileStreamFlow::writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite)
{
	ensureOpen();

	streampos posBeforeWrite = m_file.tellp();
	m_file.write((char*)pBuffer, uiByteCountToWrite);
	streampos posAfterWrite = m_file.tellp();
	uint64 uiByteWrittenCount = posAfterWrite - posBeforeWrite;

	return uiByteWrittenCount;
}

uint64			FileStreamFlow::readOnce(uint8* pBuffer, uint64 uiMaxLength)
{
	ensureOpen();

	streampos posBeforeRead = m_file.tellg();
	m_file.read((char*)pBuffer, uiMaxLength);
	streampos posAfterRead = m_file.tellg();
	uint64 uiByteReadCount = posAfterRead - posBeforeRead;

	return uiByteReadCount;
}

// write/read all data
void			FileStreamFlow::writeAll(const uint8* pBuffer, uint64 uiByteCountToWrite)
{
	if (pBuffer == nullptr)
		return;

	uint64 uiByteCountWritten = 0;
	uint64 uiByteCountWrittenOnce = 0;
	uint64 uiRemainingByteCountToWrite = uiByteCountToWrite;
	do
	{
		uiByteCountWrittenOnce = writeOnce(pBuffer + uiByteCountWritten, uiRemainingByteCountToWrite);
		uiByteCountWritten += uiByteCountWrittenOnce;
		uiRemainingByteCountToWrite -= uiByteCountWrittenOnce;
	}
	while (uiByteCountWritten < uiByteCountToWrite);
}

void			FileStreamFlow::readAll(uint8* pBuffer, uint64 uiByteCountToRead)
{
	if (pBuffer == nullptr)
		return;

	uint64 uiByteCountRead = 0;
	uint64 uiByteCountReadOnce = 0;
	uint64 uiRemainingByteCountToRead = uiByteCountToRead;
	do
	{
		uiByteCountReadOnce = readOnce(pBuffer + uiByteCountRead, uiRemainingByteCountToRead);
		uiByteCountRead += uiByteCountReadOnce;
		uiRemainingByteCountToRead -= uiByteCountReadOnce;
	}
	while (uiByteCountRead < uiByteCountToRead);
}

// seek
void			FileStreamFlow::seek(uint64 uiIndex)
{
	ensureOpen();

	m_file.seekg(uiIndex);
	m_file.seekp(uiIndex);
}

// flush
void			FileStreamFlow::flush()
{
	ensureOpen();

	m_file.flush();
}

// ensure
void			FileStreamFlow::ensureOpen()
{
	if (!m_file.is_open())
		throw new StreamException(STREAM_ERROR_FILE_NOT_OPEN);
}