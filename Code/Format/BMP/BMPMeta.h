#pragma once

#include "mx.h"
#include "Format/BMP/EBMPFileType.h"
#include "Format/BMP/EBMPDIBHeader.h"
#include "Format/BMP/EBMPDIBHeaderType.h"
#include "Format/BMP/EBMPCompressionType.h"

/*
 * Note:
 * This structure is raw data from the stream (e.g. file),
 * except for properties which are null'd by MX, advised to be ignored by the specification under certain conditions.
*/

class mx::BMPMeta
{
public:
	// All versions
	EBMPFileType m_uiFileType;
	EBMPDIBHeader m_uiDIBHeader;

	// OS2.1X / WinV1
	uint16 m_uiColourPlaneCount;
	uint16 m_uiBitPerPixelCount;

	// OS2.2X / Some WinV2
	EBMPCompressionType m_uiCompressionType;
	uint32 m_uiDataSize;
	uint32 m_uiXResolution;
	uint32 m_uiYResolution;
	uint32 m_uiTableEntryCount;
	uint32 m_uiTableImportantEntryCount;
	uint16 m_uiUnitType;
	uint16 m_uiReserved1;
	uint16 m_uiRecordingAlgorithm;
	uint16 m_uiHalftoningAlgorithm;
	uint32 m_uiHalftoningAlgorithmSize1;
	uint32 m_uiHalftoningAlgorithmSize2;
	uint32 m_uiColourEncoding;
	uint32 m_uiApplicationIdentifier;

	// WinV2
	int32 m_iXPelsPerMeter;
	int32 m_iYPelsPerMeter;
	uint32 m_uiRedMask;
	uint32 m_uiGreenMask;
	uint32 m_uiBlueMask;

	// WinV3
	uint32 m_uiAlphaMask;

	// WinV4
	uint32 m_uiCSType;
	tagICEXYZTRIPLE m_endpoints;
	uint32 m_uiGammaRed;
	uint32 m_uiGammaGreen;
	uint32 m_uiGammaBlue;

	// WinV5
	uint32 m_uiIntent;
	uint32 m_uiProfileDataOffset;
	uint32 m_uiProfileDataSize;
	uint32 m_uiReserved2;

public:
	BMPMeta();

	EBMPDIBHeaderType				getDIBHeaderType();
	uint32							getDIBHeaderSize();
	uint32							getWinBMPVersion();
};