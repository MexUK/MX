#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>
#include <deque>
#include <atlstr.h>

#define MX_THIRD_PARTY_pack754_32(f) (mx::pack754((f), 32, 8))
#define MX_THIRD_PARTY_pack754_64(f) (mx::pack754((f), 64, 11))
#define MX_THIRD_PARTY_unpack754_32(i) (mx::unpack754((i), 32, 8))
#define MX_THIRD_PARTY_unpack754_64(i) (mx::unpack754((i), 64, 11))

class mx::String
{
public:
	// general string functionality
	static std::string						replace(std::string& strString, std::string& strFind, std::string& strReplace, bool bCaseSensitive = false);
	static std::string						remove(std::string& strString, std::string& strRemoveText, bool bCaseSensitive = false);
	static std::string						trim(std::string& strString, std::string& strTrimCharacters = mx::g_strBlankString);
	static std::string						ltrim(std::string& strString, std::string& strTrimCharacters = mx::g_strBlankString);
	static std::string						rtrim(std::string& strString, std::string& strTrimCharacters = mx::g_strBlankString);
	static std::string						rtrimFromLeft(std::string& strString);
	static std::string						zeroPad(std::string& strData, uint32 uiPadLength);
	static std::string						zeroPad(uint32 uiPadLength);
	static std::string						upper(std::string& strString);
	static std::string						lower(std::string& strString);
	static std::string						titleCase(std::string& strString);
	static std::string						getTextInSameCase(std::string& strText, std::string& strTextCase);
	static bool								isIn(std::string& strText, std::string& strFind, bool bCaseSensitive = false, uint32 uiStartIndex = 0);
	static uint32							getHitCount(std::string& strText, std::string& strFind, bool bCaseSensitive = false, uint32 uiStartIndex = 0);
	static std::string						readStringUntilZero(std::string& strData);
	static std::string						readStringUntilSpace(std::string& strData);
	static std::string						stripNumbersFromRight(std::string& strText);
	static std::string						capLength(std::string& strData, uint32 uiMaxLength);
	static std::string						mergeStrings(std::string& strString1, std::string& strString, uint32 uiCapLength);
	static bool								doesStartWith(std::string& strText, std::string& strStartWith, bool bMatchCase = true);
	static std::string						addNumberGrouping(std::string& strNumber);
	static std::string						fixEOLs(std::string& strData, std::string& strEOLCharsToUse);
	static uint32							swapEndian(uint32 uiValue);
	static bool								isAsciiCharacterDisplayable(uint8 uiCharCode, bool bIncludeSpace = true, bool bIncludeTab = true);
	static std::string						createCharString(uint8 uiCharCode);
	static std::string						reverse(std::string& strValue);

	// convert string and vector/deque
	static std::vector<std::string>			split(std::string& strString, std::string& strDelimiter, bool bAllowBlankTokens = false);
	static std::string						join(std::vector<std::string>& vecTokens, std::string& strDelimiter);
	static std::string						join(std::deque<std::string>& deqTokens, std::string& strDelimiter);

	// data packing
	static std::string						packUint64(uint64 uiULongLong, bool bBigEndian = true);
	static std::string						packUint32(uint32 uiULong, bool bBigEndian = true);
	static std::string						packUint16(uint16 usUShort, bool bBigEndian = true);
	static std::string						packUint8(uint8 ucUChar);
	static std::string						packInt64(int64 iLongLong, bool bBigEndian = true);
	static std::string						packInt32(int32 iLong, bool bBigEndian = true);
	static std::string						packInt16(int16 iShort, bool bBigEndian = true);
	static std::string						packInt8(int8 iChar);
	static std::string						packFloat32(float32 fValue, bool bBigEndian = true);
	static std::string						packFloat64(float64 fValue, bool bBigEndian = true);
	static std::string						packVector2D(glm::vec2& vecVector, bool bBigEndian = true);
	static std::string						packVector3D(glm::vec3& vecVector, bool bBigEndian = true);
	static std::string						packVector4D(glm::vec4& vecVector, bool bBigEndian = true);

	// data unpacking
	static uint32							unpackUint32(std::string& strData, bool bBigEndian = true);
	static uint16							unpackUint16(std::string& strData, bool bBigEndian = true);
	static uint8							unpackUint8(std::string& strData);
	static int32							unpackInt32(std::string& strData, bool bBigEndian = true);
	static int16							unpackInt16(std::string& strData, bool bBigEndian = true);
	static int8								unpackInt8(std::string& strData);
	static float32							unpackFloat32(std::string& strData, bool bBigEndian = true);
	static glm::vec2						unpackVector2D(std::string& strData, bool bBigEndian = true);
	static glm::vec3						unpackVector3D(std::string& strData, bool bBigEndian = true);
	static glm::vec4						unpackVector4D(std::string& strData, bool bBigEndian = true);

	// convert type to string
	static std::string						toString(int iNumber);
	inline static std::string				toString(uint32 uiNumber);
	static std::string						toString(unsigned long uiNumber);
	static std::string						toString(float32 fNumber);
	static std::string						toStringExtendedFloat(float32 fNumber);
	static std::string						toStringHex(uint32 uiNumber);

	// convert string to type
	static int								toNumber(std::string& strText);
	static uint32							toUint32(std::string& strString);
	static uint16							toUint16(std::string& strString);
	static uint8							toUint8(std::string& strString);
	static int32							toInt32(std::string& strString);
	static int16							toInt16(std::string& strString);
	static int8								toInt8(std::string& strString);
	static float32							toFloat32(std::string& strString);

	// type validation
	static bool								isLengthInRange(std::string& strString, uint32 uiMinLength, uint32 uiMaxLength);
	static bool								isNumeric(std::string& strData, bool bStripCommas = true);
	static bool								isAlphaNumericUnderscore(std::string& strData, bool bAllowDigitAtStart = true);
	static bool								isPositiveInteger(std::string& strText);
	static bool								isStringChars(std::string& strData);
	static bool								isUintBetween(std::string& strData, uint32 uiMin, uint32 uiMax);

	// time related
	static std::string						getTimestampText(uint32 uiTimestamp = 0);
	static std::string						getDateTextForFolder(void);
	static uint32							getCurrentEpochTime(void);

	// clipboard
	static void								setClipboardText(std::string& strText);
	static std::string						getClipboardText(void);

	// tokens
	static std::string						getToken(std::string& strData, std::string& strToken, uint32 uiTokenIndex);
	static glm::uvec2						getVisibleTextTokenRange(std::string& strText, uint32 uiCharXPosition);
	static glm::vec2						getVec2FromTokens(std::vector<std::string>& vecTokens, uint32 uiStartIndex);
	static glm::vec3						getVec3FromTokens(std::vector<std::string>& vecTokens, uint32 uiStartIndex);

	// parsing
	static std::string						extractQuotedData(std::string& strData, uint32 uiQuoteIndex = 0);
	static std::string						collectAsString(std::string& strData, std::string& strCharactersToCollect);

	// misc
	static std::string						escapeMenuText(std::string& strText);
	static std::string						removeComments(std::string& strText, std::vector<std::string>& vecCommentCharacters);

	// allocation/copy
	static char*							copy(std::string& str);

	// string/wstring conversion
	static std::wstring						convertStdStringToStdWString(std::string& str)
	{
		//setlocale(LC_ALL, "");
		std::wstring wc(str.length()+1, L'#');
		size_t uiCharsConvertedCount = 0;
		mbstowcs_s(&uiCharsConvertedCount, &wc[0], str.length()+1, str.c_str(), str.length());
		return wc;
	}

	static std::string						convertStdWStringToStdString(std::wstring& str)
	{
		int len;
		int slength = (int)str.length() + 1;
		len = WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, 0, 0, 0, 0);
		char* buf = new char[len];
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), slength, buf, len, 0, 0);
		std::string r(buf);
		delete[] buf;
		return r;
	}
};

// inline methods
std::string				mx::String::toString(uint32 uiNumber)
{
	char szString[21];
	_itoa_s(uiNumber, szString, 10);
	return std::string(szString);
}