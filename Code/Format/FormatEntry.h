#pragma once

#include "mx.h"
#include <string>

class mx::FormatEntry
{
public:
	virtual void						setEntryName(std::string& strEntryName) = 0;
	virtual std::string&				getEntryName(void) = 0;

	virtual void						replace(std::string& strFilePath) = 0;

	virtual uint32						getIndex(void) = 0;
	virtual std::string					getEntryExtension(void) = 0;
	virtual uint32						getEntryOffset(void) = 0;
	virtual uint32						getEntrySize(void) = 0;
	virtual uint32						getRawVersion(void) = 0;
	virtual std::string					getVersionText(void) = 0;

	virtual std::string					getEntryData(void) = 0;

	virtual mx::EFileType	getEntryType(void) = 0;
};