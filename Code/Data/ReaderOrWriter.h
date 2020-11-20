#pragma once

#include "mx.h"
#include "Stream/Stream.h"
#include "EEndian.h"

#define OCTET_COMBINATION_COUNT			256
#define TWO_OCTETS_COMBINATION_COUNT	65536
#define THREE_OCTETS_COMBINATION_COUNT	16777216
#define FOUR_OCTETS_COMBINATION_COUNT	4294967296
#define FIVE_OCTETS_COMBINATION_COUNT	1099511627776
#define SIX_OCTETS_COMBINATION_COUNT	281474976710656
#define SEVEN_OCTETS_COMBINATION_COUNT	72057594037927936

class mx::ReaderOrWriter
{
public:
	mx::Stream&				m_stream;
	EEndian					m_uiEndian;

public:
	ReaderOrWriter(mx::Stream& stream);

	void					seek(uint64 uiIndex);
};