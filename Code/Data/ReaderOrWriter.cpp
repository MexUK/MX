#include "ReaderOrWriter.h"

using namespace mx;

ReaderOrWriter::ReaderOrWriter(Stream& stream) :
	m_stream(stream),
	m_uiEndian(LITTLE_ENDIAN)
{
}

// seek
void					ReaderOrWriter::seek(uint64 uiIndex)
{
	m_stream.seek(uiIndex);
}