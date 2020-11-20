#pragma once

#include "mx.h"
#include "Format/Network/JSON/JSONValue.h"

class mx::JSONContainer : public mx::JSONValue
{
public:
	mx::JSONContainer*									m_pParentContainer;
};