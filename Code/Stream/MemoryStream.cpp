#include "MemoryStream.h"
#include "Flows/FileStreamFlow.h"

using namespace mx;

MemoryStream::MemoryStream(char *pBuffer) :
	Stream(STREAM_DEVICE_TYPE_MEMORY)
{
	m_pFlow->prepare(pBuffer);
}