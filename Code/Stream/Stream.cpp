#include "Stream.h"
#include "Flows/FileStreamFlow.h"
#include "Flows/MemoryStreamFlow.h"
#include "Flows/DirectoryStreamFlow.h"
#include "Errors/StreamException.h"

using namespace std;
using namespace mx;

Stream::Stream(EStreamDeviceType uiDeviceType) :
	m_pFlow(nullptr)
{
	if (uiDeviceType != STREAM_DEVICE_TYPE_UNKNOWN)
		setDeviceType(uiDeviceType);
}

Stream::Stream(string& strFilePath) :
	m_pFlow(nullptr)
{
	setDeviceType(STREAM_DEVICE_TYPE_FILE);
	m_pFlow->prepare(strFilePath);
}

Stream::~Stream()
{
	// todo
	//if (m_pFlow)
	//	destroyFlow(m_pFlow);
}

// device type
void			Stream::setDeviceType(EStreamDeviceType uiDeviceType)
{
	if (m_pFlow)
		destroyFlow(m_pFlow);

	m_pFlow = createFlow(uiDeviceType);
}

// flow object
StreamFlow*		Stream::createFlow(EStreamDeviceType uiDeviceType)
{
	switch (uiDeviceType)
	{
	case STREAM_DEVICE_TYPE_FILE:		return new FileStreamFlow;
	case STREAM_DEVICE_TYPE_MEMORY:		return new MemoryStreamFlow;
	//case STREAM_DEVICE_TYPE_DIRECTORY:	return new DirectoryStreamFlow;
	default:							throw new StreamException(STREAM_ERROR_UNKNOWN_DEVICE_TYPE);
	}
}

void			Stream::destroyFlow(StreamFlow*& pFlow)
{
	if (pFlow->isOpen())
	{
		pFlow->flush();
		pFlow->close();
	}

	delete pFlow;
}

// open/close
bool			Stream::isOpen()
{
	return m_pFlow->isOpen();
}

bool			Stream::open()
{
	m_pFlow->open();
	return m_pFlow->isOpen();
}

void			Stream::close()
{
	m_pFlow->flush();
	m_pFlow->close();
}

// write/read
void			Stream::writeOnce(const uint8* pBuffer, uint64 uiMaxLength)
{
	m_pFlow->writeOnce(pBuffer, uiMaxLength);
}

void			Stream::readOnce(uint8* pBuffer, uint64 uiMaxLength)
{
	m_pFlow->readOnce(pBuffer, uiMaxLength);
}

// write/read all data
void			Stream::writeAll(const uint8* pBuffer, uint64 uiLength)
{
	m_pFlow->writeAll(pBuffer, uiLength);
}

void			Stream::readAll(uint8* pBuffer, uint64 uiLength)
{
	m_pFlow->readAll(pBuffer, uiLength);
}

// seek
void			Stream::seek(uint64 uiIndex)
{
	m_pFlow->seek(uiIndex);
}

// flush
void			Stream::flush()
{
	m_pFlow->flush();
}