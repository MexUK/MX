#pragma once

#include "mx.h"
#include "EStreamError.h"

class mx::StreamException
{
public:
	EStreamError			m_uiStreamErrorId;

public:
	StreamException(EStreamError uiStreamErrorId);
};