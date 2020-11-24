#include "FileStreamFlow.h"
#include "Stream/Errors/StreamException.h"
#include "Static/Dir.h"
#include <iostream>

using namespace std;
using namespace mx;

FileStreamFlow::FileStreamFlow() :
	StreamFlow(STREAM_DEVICE_TYPE_FILE),
	m_bCheckForErrors(true)
{
}

FileStreamFlow::~FileStreamFlow()
{
	if (isOpen())
	{
		flush();
		close();
	}
}

// prepare
void			FileStreamFlow::prepare(string& strFilePath, bool bCheckForErrors)
{
	m_strFilePath = strFilePath;
	m_bCheckForErrors = bCheckForErrors;
}

void			FileStreamFlow::prepare(char* pBuffer)
{
	if(m_bCheckForErrors)
		throw new StreamException(STREAM_ERROR_INVALID_PREPARE_SIGNATURE_FOR_DEVICE_TYPE);
}

// open/close
bool			FileStreamFlow::isOpen()
{
	return m_file.is_open();
}

void			FileStreamFlow::open()
{
	if (m_bCheckForErrors)
	{
		if (m_file.is_open())
			throw new StreamException(STREAM_ERROR_FILE_ALREADY_OPEN);

		if (m_strFilePath.empty())
			throw new StreamException(STREAM_ERROR_EMPTY_FILE_PATH);
	}

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
		if (m_bCheckForErrors && !m_file.is_open())
			throw new StreamException(STREAM_ERROR_FILE_OPEN);

		// re-open file with correct flags
		m_file.close();
		if (m_bCheckForErrors && !m_file)
			throw new StreamException(STREAM_ERROR_FILE_CLOSE);

		m_file.open(m_strFilePath, uiOpenFlags);
		if (m_bCheckForErrors && !m_file.is_open())
			throw new StreamException(STREAM_ERROR_FILE_OPEN);
	}

	seek(0);
}

void			FileStreamFlow::close()
{
	ensureOpen();

	m_file.close();
	if (m_bCheckForErrors && !m_file)
		throw new StreamException(STREAM_ERROR_FILE_CLOSE);

	m_strFilePath = "";
}

// write/read data once
uint64			FileStreamFlow::writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite)
{
	ensureOpen();

	streampos posBeforeWrite = m_file.tellp();
	if (m_bCheckForErrors && (!m_file || posBeforeWrite == -1))
		throw new StreamException(STREAM_ERROR_FILE_GET_SEEK);
	m_file.write((char*)pBuffer, uiByteCountToWrite);
	if (m_bCheckForErrors)
	{
		if (!m_file)
			throw new StreamException(STREAM_ERROR_FILE_WRITE);
		m_file.flush();
		if (!m_file)
			throw new StreamException(STREAM_ERROR_FILE_WRITE);
	}
	streampos posAfterWrite = m_file.tellp();
	if (m_bCheckForErrors && (!m_file || posAfterWrite == -1))
		throw new StreamException(STREAM_ERROR_FILE_GET_SEEK);
	uint64 uiByteWrittenCount = posAfterWrite - posBeforeWrite;

	return uiByteWrittenCount;
}

uint64			FileStreamFlow::readOnce(uint8* pBuffer, uint64 uiMaxLength)
{
	ensureOpen();

	streampos posBeforeRead = m_file.tellg();
	if (m_bCheckForErrors && (!m_file || posBeforeRead == -1))
		throw new StreamException(STREAM_ERROR_FILE_GET_SEEK);
	m_file.read((char*)pBuffer, uiMaxLength);
	if (m_bCheckForErrors && !m_file)
		throw new StreamException(STREAM_ERROR_FILE_READ);
	streampos posAfterRead = m_file.tellg();
	if (m_bCheckForErrors && (!m_file || posAfterRead == -1))
		throw new StreamException(STREAM_ERROR_FILE_GET_SEEK);
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
	if (m_bCheckForErrors && !m_file)
		throw new StreamException(STREAM_ERROR_FILE_SET_SEEK);

	m_file.seekp(uiIndex);
	if (m_bCheckForErrors && !m_file)
		throw new StreamException(STREAM_ERROR_FILE_SET_SEEK);
}

// flush
void			FileStreamFlow::flush()
{
	ensureOpen();

	m_file.flush();
	if (m_bCheckForErrors && !m_file)
		throw new StreamException(STREAM_ERROR_FILE_FLUSH);
}

// ensure
void			FileStreamFlow::ensureOpen()
{
	if (m_bCheckForErrors && !m_file.is_open())
		throw new StreamException(STREAM_ERROR_FILE_NOT_OPEN);
}