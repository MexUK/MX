#include "MemoryStreamFlow.h"
#include "Stream/Errors/StreamException.h"

using namespace std;
using namespace mx;

MemoryStreamFlow::MemoryStreamFlow() :
	StreamFlow(STREAM_DEVICE_TYPE_MEMORY),
	m_pBuffer(nullptr)
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
uint64			MemoryStreamFlow::writeOnce(const uint8* pBuffer, uint64 uiMaxLength)
{
	memcpy(m_pBuffer, pBuffer, uiMaxLength);
	return 0; // todo
}

uint64			MemoryStreamFlow::readOnce(uint8* pBuffer, uint64 uiMaxLength)
{
	memcpy(pBuffer, m_pBuffer, uiMaxLength);
	return 0; // todo
}

// write/read all data
void			MemoryStreamFlow::writeAll(const uint8* pBuffer, uint64 uiLength)
{
	memcpy(m_pBuffer, pBuffer, uiLength);
}

void			MemoryStreamFlow::readAll(uint8* pBuffer, uint64 uiLength)
{
	memcpy(pBuffer, m_pBuffer, uiLength);
}

// seek
void			MemoryStreamFlow::seek(uint64 uiLength)
{
}

// flush
void			MemoryStreamFlow::flush()
{
}