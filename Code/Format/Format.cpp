#include "Format.h"
#include "Stream/Errors/StreamException.h"

using namespace std;
using namespace mx;

//Stream g_defaultStream;

Format::Format(Stream& stream, bool bFormatIsBinary, EEndian uiEndian) :
	m_reader(Reader(stream)),
	m_writer(Writer(stream))
{
}

/*
Format::Format(string& strFilePath, bool bFormatIsBinary, mx::EEndian uiEndian) :
	m_reader(Reader(g_defaultStream)),
	m_writer(Writer(g_defaultStream))
{
	g_defaultStream.setDeviceType(STREAM_DEVICE_TYPE_FILE);
	g_defaultStream.open(strFilePath);
}
*/

Format::~Format()
{
	if (m_reader.m_stream.isOpen())
		m_reader.m_stream.close();
}

// serialization
bool			Format::unserialize()
{
	try
	{
		m_reader.m_stream.open();
		_unserialize();
		m_reader.m_stream.close();
	}
	catch (StreamException& e)
	{
		m_uiError = e.m_uiStreamErrorId;

		if (m_reader.m_stream.isOpen())
			m_reader.m_stream.close();

		return false;
	}
	return true;
}

bool			Format::serialize()
{
	try
	{
		m_writer.m_stream.open();
		_serialize();
		m_writer.m_stream.close();
	}
	catch (StreamException& e)
	{
		m_uiError = e.m_uiStreamErrorId;

		if (m_writer.m_stream.isOpen())
			m_writer.m_stream.close();

		return false;
	}
	return true;
}