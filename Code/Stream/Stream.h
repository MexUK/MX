#pragma once

#include <string>
#include <fstream>

#include "mx.h"
#include "Internal/EStreamDeviceType.h"

class mx::Stream
{
public:
	StreamFlow*				m_pFlow;

public:
	Stream(EStreamDeviceType uiDeviceTypeIn = mx::STREAM_DEVICE_TYPE_UNKNOWN);
	Stream(std::string& strFilePath);
	~Stream();

	void					setDeviceType(EStreamDeviceType uiDeviceType);

	bool					isOpen();
	bool					open();
	void					close();

	void					writeOnce(const uint8* pBuffer, uint64 uiMaxLength);
	void					readOnce(uint8* pBuffer, uint64 uiMaxLength);

	void					writeAll(const uint8* pBuffer, uint64 uiLength);
	void					readAll(uint8* pBuffer, uint64 uiLength);

	void					seek(uint64 uiIndex);

	void					flush();

private:
	static StreamFlow*		createFlow(EStreamDeviceType uiDeviceType);
	static void				destroyFlow(StreamFlow*& pFlow);
};