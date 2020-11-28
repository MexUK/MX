#pragma once

#include "mx.h"
#include "EFormatException.h"

class mx::FormatException
{
public:
	EFormatException m_uiFormatException;

public:
	FormatException(EFormatException uiFormatException) :
		m_uiFormatException(uiFormatException)
	{
	}
};