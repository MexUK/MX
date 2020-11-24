#include "MemoryStreamFlow.h"
#include "Stream/Errors/StreamException.h"

using namespace std;
using namespace mx;

MemoryStreamFlow::MemoryStreamFlow() :
	StreamFlow(STREAM_DEVICE_TYPE_MEMORY),
	m_pBuffer(nullptr),
	m_uiSeek(0)
{
}

// prepare
void			MemoryStreamFlow::prepare(string& strFilePath)
{
	throw new StreamException(STREAM_ERROR_INVALID_PREPARE_SIGNATURE_FOR_DEVICE_TYPE);
}

void			MemoryStreamFlow::prepare(char *pBuffer)
{
	m_pBuffer = pBuffer;
}

// open/close
bool			MemoryStreamFlow::isOpen()
{
	return true;
}

void			MemoryStreamFlow::open()
{
}

void			MemoryStreamFlow::close()
{
}

// write/read data once
uint64			MemoryStreamFlow::writeOnce(const uint8* pBuffer, uint64 uiByteCountToWrite)
{
	memcpy(m_pBuffer + m_uiSeek, pBuffer, uiByteCountToWrite);
	m_uiSeek += uiByteCountToWrite;
	return uiByteCountToWrite;
}

uint64			MemoryStreamFlow::readOnce(uint8* pBuffer, uint64 uiByteCountToRead)
{
	memcpy(pBuffer, m_pBuffer + m_uiSeek, uiByteCountToRead);
	m_uiSeek += uiByteCountToRead;
	return uiByteCountToRead;
}

// write/read all data
void			MemoryStreamFlow::writeAll(const uint8* pBuffer, uint64 uiByteCountToWrite)
{
	writeOnce(pBuffer, uiByteCountToWrite);
}

void			MemoryStreamFlow::readAll(uint8* pBuffer, uint64 uiByteCountToRead)
{
	readOnce(pBuffer, uiByteCountToRead);
}

// seek
void			MemoryStreamFlow::seek(uint64 uiIndex)
{
	m_uiSeek = uiIndex;
}

// flush
void			MemoryStreamFlow::flush()
{
}