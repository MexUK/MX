#pragma once

#include <string>

#include "mx.h"
#include "Types.h"
#include "Data/EEndian.h"
#include "Stream/Stream.h"
#include "Stream/Errors/EStreamError.h"
#include "Data/Reader.h"
#include "Data/Writer.h"

class mx::Format
{
protected:
	mx::Reader		m_reader;
	mx::Writer		m_writer;
	EStreamError	m_uiError;

public:
	Format(mx::Stream& stream, bool bFormatIsBinary = true, mx::EEndian uiEndian = mx::LITTLE_ENDIAN);
	//Format(std::string& strFilePath, bool bFormatIsBinary = true, mx::EEndian uiEndian = mx::LITTLE_ENDIAN);
	~Format();

	EStreamError	getStreamError() { return m_uiError; }

	bool			unserialize();
	bool			serialize();

	template <class T>
	static void		unserializeFile(std::string& strFilePath, T& format)
	{
		Stream stream(strFilePath);
		format.unserialize();
	}

	template <class T>
	static T* unserializeFileHeap(std::string& strFilePath)
	{
		Stream stream(strFilePath);
		T* pFormat = new T(stream);
		pFormat->unserialize();
		return pFormat;
	}

protected:
	virtual void	_unserialize() = 0;
	virtual void	_serialize() = 0;
};