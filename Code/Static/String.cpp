#include "String.h"
#include <algorithm>
#include <ctime>

using namespace std;
using namespace glm;
using namespace mx;

uint64_t mx::pack754(float80 f, unsigned bits, unsigned expbits)
{
	float80 fnorm;
	int shift;
	long long sign, exp, significand;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

	if (f == 0.0) return 0; // get this special case out of the way

	// check sign and begin normalization
	if (f < 0) { sign = 1; fnorm = -f; }
	else { sign = 0; fnorm = f; }

	// get the normalized form of f and track the exponent
	shift = 0;
	while (fnorm >= 2.0) { fnorm /= 2.0; shift++; }
	while (fnorm < 1.0) { fnorm *= 2.0; shift--; }
	fnorm = fnorm - 1.0;

	// calculate the binary form (non-float) of the significand data
	significand = (long long) (fnorm * ((1LL << significandbits) + 0.5f));

	// get the biased exponent
	exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

	// return the final answer
	return (sign << (bits - 1)) | (exp << (bits - expbits - 1)) | significand;
}

float80 mx::unpack754(uint64_t i, unsigned bits, unsigned expbits)
{
	float80 result;
	long long shift;
	unsigned bias;
	unsigned significandbits = bits - expbits - 1; // -1 for sign bit

	if (i == 0) return 0.0;

	// pull the significand
	result = (float80) (i&((1LL << significandbits) - 1)); // mask
	result /= (1LL << significandbits); // convert back to float
	result += 1.0f; // add the one back on

	// deal with the exponent
	bias = (1 << (expbits - 1)) - 1;
	shift = ((i >> significandbits)&((1LL << expbits) - 1)) - bias;
	while (shift > 0) { result *= 2.0; shift--; }
	while (shift < 0) { result /= 2.0; shift++; }

	// sign it
	result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

	return result;
}




vector<string>		String::split(string& strString, string& strDelimiter, bool bAllowBlankTokens)
{
	if (strString.length() > 0 && strString.substr(strString.length() - 1, 1) == strDelimiter)
	{
		strString = strString.substr(0, strString.length() - 1);
	}

	vector<string> vecTokens;
	string strToken;
	uint32
		uiSeek = 0,
		uiFoundPosition = strString.find(strDelimiter, uiSeek);
	while (uiFoundPosition != string::npos)
	{
		strToken = strString.substr(uiSeek, uiFoundPosition - uiSeek);
		if (strToken != "" || bAllowBlankTokens)
		{
			vecTokens.push_back(strToken);
		}
		uiSeek = uiFoundPosition + strDelimiter.length();
		uiFoundPosition = strString.find(strDelimiter, uiSeek);
	}
	if (uiSeek != strString.length())
	{
		strToken = strString.substr(uiSeek, string::npos);
		if (strToken != "" || bAllowBlankTokens)
		{
			vecTokens.push_back(strToken);
		}
	}
	return vecTokens;
}

string				String::join(vector<string>& vecTokens, string& strDelimiter)
{
	if (vecTokens.size() == 0)
	{
		return "";
	}
	else if (vecTokens.size() == 1)
	{
		return vecTokens[0];
	}

	string strText;
	for (uint32 i = 0; i < vecTokens.size() - 1; i++)
	{
		strText += vecTokens[i] + strDelimiter;
	}
	strText += vecTokens[vecTokens.size() - 1];
	return strText;
}

string				String::join(deque<string>& deqTokens, string& strDelimiter)
{
	if (deqTokens.size() == 0)
	{
		return "";
	}

	string strText;
	for (uint32 i = 0; i < deqTokens.size() - 1; i++)
	{
		strText += deqTokens[i] + strDelimiter;
	}
	strText += deqTokens[deqTokens.size() - 1];
	return strText;
}

string				String::replace(string& strString, string& strFind, string& strReplace, bool bCaseSensitive)
{
	string
		strString2 = strString;
	int32
		iFindLength = strFind.length(),
		iReplaceLength = strReplace.length(),
		iReplaceLengthMinusFindLength = strReplace.length() - strFind.length(),
		iPos;
	if(bCaseSensitive)
	{
		iPos = strString2.find(strFind);
		while (iPos != string::npos)
		{
			strString2.replace(iPos, iFindLength, strReplace);
			iPos = strString2.find(strFind, iPos + iReplaceLength + iReplaceLengthMinusFindLength);
		}
		return strString2;
	}
	else
	{
		string
			strStringUpper = upper(strString2),
			strFindUpper = upper(strFind);
		iPos = strStringUpper.find(strFindUpper);
		while (iPos != string::npos)
		{
			strString2.replace(iPos, iFindLength, strReplace);
			strStringUpper.replace(iPos, iFindLength, strReplace);
			iPos = strStringUpper.find(strFindUpper, iPos + iReplaceLength + iReplaceLengthMinusFindLength);
		}
		return strString2;
	}
}

string				String::remove(string& strString, string& strRemoveText, bool bCaseSensitive)
{
	return replace(strString, strRemoveText, g_strBlankString, bCaseSensitive);
}

string				String::packUint64(uint64 uiULongLong, bool bBigEndian)
{
	return packUint32((uiULongLong >> 32) & 0xFFFFFFFF, bBigEndian) + packUint32(uiULongLong & 0xFFFFFFFF, bBigEndian);
}

string				String::packUint32(uint32 uiULong, bool bBigEndian)
{
	char szULong[4];
	if (bBigEndian)
	{
		szULong[0] = (uint8)floor(uiULong / 16777216);
		szULong[1] = (uint8)floor(uiULong / 65536);
		szULong[2] = (uint8)floor(uiULong / 256);
		szULong[3] = (uint8)(uiULong % 256);
	}
	else
	{
		szULong[3] = (uint8)floor(uiULong / 16777216);
		szULong[2] = (uint8)floor(uiULong / 65536);
		szULong[1] = (uint8)floor(uiULong / 256);
		szULong[0] = (uint8)(uiULong % 256);
	}
	string strULong(szULong, 4);
	return strULong;
}

string				String::packUint16(uint16 uiUShort, bool bBigEndian)
{
	uint8 szUShort[2];
	if (bBigEndian)
	{
		szUShort[0] = (uint8)floor(uiUShort / 256);
		szUShort[1] = (uint8)(uiUShort % 256);
	}
	else
	{
		szUShort[1] = (uint8)floor(uiUShort / 256);
		szUShort[0] = (uint8)(uiUShort % 256);
	}
	string strUShort((char*)szUShort, 2);
	return strUShort;
}

string				String::packUint8(uint8 ucUChar)
{
	char szUChar[1];
	szUChar[0] = (uint8)ucUChar;
	string strUChar(szUChar, 1);
	return strUChar;
}

string				String::packInt64(int64 iLongLong, bool bBigEndian)
{
	return packInt32((iLongLong >> 32) & 0xFFFFFFFF, bBigEndian) + packInt32(iLongLong & 0xFFFFFFFF, bBigEndian);
}

string				String::packInt32(int32 iLong, bool bBigEndian)
{
	char szLong[4];
	if (bBigEndian)
	{
		szLong[0] = (iLong >> 24) & 0xFF;
		szLong[1] = (iLong >> 16) & 0xFF;
		szLong[2] = (iLong >> 8) & 0xFF;
		szLong[3] = iLong & 0xFF;
	}
	else
	{
		szLong[3] = (iLong >> 24) & 0xFF;
		szLong[2] = (iLong >> 16) & 0xFF;
		szLong[1] = (iLong >> 8) & 0xFF;
		szLong[0] = iLong & 0xFF;
	}
	string strLong(szLong, 4);
	return strLong;
}

string				String::packInt16(int16 iShort, bool bBigEndian)
{
	char szLong[2];
	if (bBigEndian)
	{
		szLong[0] = (iShort >> 8) & 0xFF;
		szLong[1] = iShort & 0xFF;
	}
	else
	{
		szLong[1] = (iShort >> 8) & 0xFF;
		szLong[0] = iShort & 0xFF;
	}
	string strLong(szLong, 4);
	return strLong;
}

string				String::packInt8(int8 iChar)
{
	char szChar[1];
	szChar[0] = (int8)iChar;
	string strChar(szChar, 1);
	return strChar;
}

string				String::packFloat32(float32 fValue, bool bBigEndian)
{
	return packUint32((uint32)pack754_32(fValue), bBigEndian);
}

string				String::packFloat64(float64 fValue, bool bBigEndian)
{
	return packUint64((uint64) pack754_64(fValue), bBigEndian);
}

string				String::packVector2D(vec2& vecVector, bool bBigEndian)
{
	return String::packFloat32(vecVector.x, bBigEndian)
		 + String::packFloat32(vecVector.y, bBigEndian);
}

string				String::packVector3D(vec3& vecVector, bool bBigEndian)
{
	return String::packFloat32(vecVector.x, bBigEndian)
		 + String::packFloat32(vecVector.y, bBigEndian)
		 + String::packFloat32(vecVector.z, bBigEndian);
}

string				String::packVector4D(vec4& vecVector, bool bBigEndian)
{
	return String::packFloat32(vecVector.x, bBigEndian)
		+ String::packFloat32(vecVector.y, bBigEndian)
		+ String::packFloat32(vecVector.z, bBigEndian)
		+ String::packFloat32(vecVector.w, bBigEndian);
}

uint32		String::unpackUint32(string& strData, bool bBigEndian)
{
	uint32 uiInt;
	if (bBigEndian)
	{
		uiInt =
			(((uint8)strData.c_str()[0]) * 16777216) +
			(((uint8)strData.c_str()[1]) * 65536) +
			(((uint8)strData.c_str()[2]) * 256) +
			((uint8)strData.c_str()[3]);
	}
	else
	{
		uiInt =
			(((uint8)strData.c_str()[3]) * 16777216) +
			(((uint8)strData.c_str()[2]) * 65536) +
			(((uint8)strData.c_str()[1]) * 256) +
			((uint8)strData.c_str()[0]);
	}
	return uiInt;
}

uint16		String::unpackUint16(string& strData, bool bBigEndian)
{
	uint16 uiInt;
	if (bBigEndian)
	{
		uiInt =
			(((uint8)strData.c_str()[0]) * 256) +
			((uint8)strData.c_str()[1]);
	}
	else
	{
		uiInt =
			(((uint8)strData.c_str()[1]) * 256) +
			((uint8)strData.c_str()[0]);
	}
	return uiInt;
}

uint8		String::unpackUint8(string& strData)
{
	return (uint8)strData.c_str()[0];
}

int32			String::unpackInt32(string& strData, bool bBigEndian)
{
	int32 iInt;
	if (bBigEndian)
	{
		iInt =
			(((int8)strData.c_str()[0]) << 24) |
			(((int8)strData.c_str()[1]) << 16) |
			(((int8)strData.c_str()[2]) << 8) |
			((int8)strData.c_str()[3]);
	}
	else
	{
		iInt =
			(((int8)strData.c_str()[3]) << 24) |
			(((int8)strData.c_str()[2]) << 16) |
			(((int8)strData.c_str()[1]) << 8) |
			((int8)strData.c_str()[0]);
	}
	return iInt;
}

int16		String::unpackInt16(string& strData, bool bBigEndian)
{
	int16 iInt;
	if (bBigEndian)
	{
		iInt =
			(((int8)strData.c_str()[0]) << 8) |
			((int8)strData.c_str()[1]);
	}
	else
	{
		iInt =
			(((int8)strData.c_str()[1]) << 8) |
			((int8)strData.c_str()[0]);
	}
	return iInt;
}

int8			String::unpackInt8(string& strData)
{
	return (int8)strData.c_str()[0];
}

float32				String::unpackFloat32(string& strData, bool bBigEndian)
{
	return (float32)unpack754_32(unpackUint32(strData, bBigEndian));
}

vec2			String::unpackVector2D(string& strData, bool bBigEndian)
{
	string strStrings[2] = {
		strData.substr(0, 4),
		strData.substr(4, 4)
	};
	return vec2(
		unpackFloat32(strStrings[0], bBigEndian),
		unpackFloat32(strStrings[1], bBigEndian)
	);
}

vec3			String::unpackVector3D(string& strData, bool bBigEndian)
{
	string strStrings[3] = {
		strData.substr(0, 4),
		strData.substr(4, 4),
		strData.substr(8, 4)
	};
	return vec3(
		unpackFloat32(strStrings[0], bBigEndian),
		unpackFloat32(strStrings[1], bBigEndian),
		unpackFloat32(strStrings[2], bBigEndian)
	);
}

vec4		String::unpackVector4D(string& strData, bool bBigEndian)
{
	string strStrings[4] = {
		strData.substr(0, 4),
		strData.substr(4, 4),
		strData.substr(8, 4),
		strData.substr(12, 4)
	};
	return vec4(
		unpackFloat32(strStrings[0], bBigEndian),
		unpackFloat32(strStrings[1], bBigEndian),
		unpackFloat32(strStrings[2], bBigEndian),
		unpackFloat32(strStrings[3], bBigEndian)
	);
}

string				String::toString(int iNumber)
{
	char szString[21];
	_itoa_s(iNumber, szString, 10);
	return string(szString);
}

string				String::toString(unsigned long uiNumber)
{
	char szString[21];
	_itoa_s(uiNumber, szString, 10);
	return string(szString);
}

string				String::toString(float32 fNumber)
{
	char szString[21];
	sprintf_s(szString, "%g", fNumber);
	return string(szString);
}

string				String::toStringExtendedFloat(float32 fNumber)
{
	char szString[21];
	sprintf_s(szString, "%f", fNumber);
	return string(szString);
}

string				String::toStringHex(uint32 uiNumber)
{
	char szString[21];
	sprintf_s(szString, "%X", uiNumber);
	return string(szString);
}

int					String::toNumber(string& strText)
{
	return atoi(strText.c_str());
}

uint32				String::toUint32(string& strString)
{
	return atoi(strString.c_str());
}

uint16				String::toUint16(string& strString)
{
	return (uint16) atoi(strString.c_str());
}

uint8				String::toUint8(string& strString)
{
	return (uint8) atoi(strString.c_str());
}

int32				String::toInt32(string& strString)
{
	return atoi(strString.c_str());
}

int16				String::toInt16(string& strString)
{
	return (int16) atoi(strString.c_str());
}

int8				String::toInt8(string& strString)
{
	return (int8) atoi(strString.c_str());
}

float32				String::toFloat32(string& strString)
{
	return (float32) atof(strString.c_str());
}

string				String::trim(string& strString, string& strTrimCharacters)
{
	string strString2 = ltrim(strString, strTrimCharacters);
	return rtrim(strString2, strTrimCharacters);
}

string				String::ltrim(string& strString, string& strTrimCharacters)
{
	if (strTrimCharacters == "")
	{
		uint32 iStart = 0;
		for (uint32 i = 0; i < strString.length(); i++)
		{
			if (strString.at(i) <= 32 || strString.at(i) >= 127)
			{
				iStart++;
			}
			else
			{
				break;
			}
		}
		return strString.substr(iStart);
	}
	else
	{
		uint32 iStart = 0;
		for (uint32 i = 0; i < strString.length(); i++)
		{
			if (strString.substr(i, strTrimCharacters.length()) == strTrimCharacters)
			{
				iStart += strTrimCharacters.length();
				i += strTrimCharacters.length() - 1;
			}
			else
			{
				break;
			}
		}
		return strString.substr(iStart);
	}
}

string				String::rtrim(string& strString, string& strTrimCharacters)
{
	if (strTrimCharacters == "")
	{
		uint32 iEnd = strString.length() - 1;
		for (int32 i = iEnd; i >= 0; i--)
		{
			if (strString.at(i) <= 32 || strString.at(i) >= 127)
			{
				iEnd--;
			}
			else
			{
				break;
			}
		}
		return strString.substr(0, iEnd + 1);
	}
	else
	{
		uint32 iEnd = strString.length() - strTrimCharacters.length();
		for (int32 i = iEnd; i >= 0; i--)
		{
			if (strString.substr(i, strTrimCharacters.length()) == strTrimCharacters)
			{
				iEnd -= strTrimCharacters.length();
				i -= strTrimCharacters.length() - 1;
			}
			else
			{
				break;
			}
		}
		return strString.substr(0, iEnd + 1);
	}
}

string				String::rtrimFromLeft(string& strString)
{
	for (uint32 i = 0; i < strString.length(); i++)
	{
		if (strString.at(i) < 32 || strString.at(i) >= 127)
		{
			return strString.substr(0, i);
		}
	}
	return strString;
}

string				String::zeroPad(string& strData, uint32 uiPadLength)
{
	uint32 uiOldLength = strData.size();
	strData.resize(uiPadLength);
	for (uint32 i = uiOldLength; i < uiPadLength; i++)
	{
		strData[i] = '\0';
	}
	return strData;
}

string				String::zeroPad(uint32 uiPadLength)
{
	string strData = "";
	strData.resize(uiPadLength);
	for (uint32 i = 0; i < uiPadLength; i++)
	{
		strData[i] = '\0';
	}
	return strData;
}

string				String::upper(string& strString)
{
	string strString2 = strString;
	transform(strString2.begin(), strString2.end(), strString2.begin(), ::toupper);
	return strString2;
}

string				String::lower(string& strString)
{
	string strString2 = strString;
	transform(strString2.begin(), strString2.end(), strString2.begin(), ::tolower);
	return strString2;
}

string				String::titleCase(string& strString)
{
	string strSpace = " ";
	vector<string> vecTokens = split(strString, strSpace);
	for (uint32 i = 0; i < vecTokens.size(); i++)
	{
		string strFirstChar = strString.substr(0, 1);
		string strRemainingChars = strString.substr(1);
		vecTokens[i] = upper(strFirstChar) + lower(strRemainingChars);
	}
	return join(vecTokens, strSpace);
}

string				String::getTimestampText(uint32 uiTimestamp)
{
	time_t uiTime;
	if (uiTimestamp == 0)
	{
		uiTime = time(0);
	}
	else
	{
		uiTime = uiTimestamp;
	}
	tm t;
	localtime_s(&t, &uiTime);

	char *pStr = new char[1024];
	strftime(pStr, 1024, "%d %B %Y %H:%M:%S", &t);
	string strTimeString(pStr);
	delete[] pStr;
	return strTimeString;
}

string				String::getDateTextForFolder(void)
{
	time_t uiTime = time(0);
	tm t;
	localtime_s(&t, &uiTime);

	char *pStr = new char[1024];
	strftime(pStr, 1024, "%d %B %Y", &t);
	string strTimeString(pStr);
	delete[] pStr;
	return strTimeString;
}

string				String::escapeMenuText(string& strText)
{
	string
		strAmp1 = "&",
		strAmp2 = "&&";
	return replace(strText, strAmp1, strAmp2);
}

string				String::removeComments(string& strText, vector<string>& vecCommentCharacters)
{
	string strTextWithoutComment = strText;
	for (string& strCommentCharacters : vecCommentCharacters)
	{
		size_t uiCommentIndex = strTextWithoutComment.find_first_of(strCommentCharacters, 0);
		if (uiCommentIndex != string::npos)
		{
			strTextWithoutComment = strTextWithoutComment.substr(0, uiCommentIndex);
		}
	}
	return strTextWithoutComment;
}

char*				String::copy(string& str)
{
	char *pData = (char*)malloc(str.length());
	memcpy(pData, str.c_str(), str.length());
	return pData;
}

bool				String::isIn(string& strText, string& strFind, bool bCaseSensitive, uint32 uiStartIndex)
{
	if(bCaseSensitive)
	{
		return strText.find(strFind, uiStartIndex) != string::npos;
	}
	else
	{
		return upper(strText).find(upper(strFind), uiStartIndex) != string::npos;
	}
}

uint32				String::getHitCount(string& strText, string& strFind, bool bCaseSensitive, uint32 uiStartIndex)
{
	uint32
		uiHitCount = 0,
		uiFindLength = strFind.length(),
		uiTextLength = strText.length();
	string
		strText2,
		strFind2;
	if (!bCaseSensitive)
	{
		strText2 = upper(strText);
		strFind2 = upper(strFind);
	}
	for(uint32 uiMatchIndex = 0; uiMatchIndex != string::npos && uiStartIndex < uiTextLength; uiStartIndex = uiMatchIndex + uiFindLength)
	{
		if (bCaseSensitive)
		{
			uiMatchIndex = strText.find(strFind, uiStartIndex);
		}
		else
		{
			uiMatchIndex = strText2.find(strFind2, uiStartIndex);
		}

		if (uiMatchIndex != string::npos)
		{
			uiHitCount++;
		}
	}
	return uiHitCount;
}

string				String::readStringUntilZero(string& strData)
{
	uint32 uiLength = strlen(strData.c_str());
	if (uiLength == 0)
	{
		return "";
	}
	return strData.substr(0, uiLength);
}

string				String::readStringUntilSpace(string& strString)
{
	size_t iSeek = strString.find(' ', 0);
	if (iSeek == string::npos)
	{
		return strString;
	}
	else
	{
		return strString.substr(0, iSeek);
	}
}

string				String::stripNumbersFromRight(string& strText)
{
	uint32 iEnd = strText.length() - 1;
	for (int32 i = strText.length() - 1; i >= 0; i--)
	{
		if (strText.c_str()[i] >= 48 && strText.c_str()[i] <= 57)
		{
			iEnd--;
		}
		else
		{
			break;
		}
	}
	return strText.substr(0, iEnd + 1);
}

bool				String::isLengthInRange(string& strString, uint32 uiMinLength, uint32 uiMaxLength)
{
	return strString.length() >= uiMinLength && strString.length() <= uiMaxLength;
}

bool				String::isNumeric(string& strData, bool bStripCommas)
{
	// e.g. " -5,000.20 " returns true
	string strComma = ",";
	string strData1 = bStripCommas ? remove(strData, strComma) : strData;
	string strDot = ".";
	strData1 = remove(strData1, strDot);
	string strData2 = trim(strData1);
	if (strData2.length() < 1)
		return false;
	if (strData2.c_str()[0] == '-')
		strData2 = strData2.substr(1);
	for (uint32 i = 0, j = strData2.length(); i < j; i++)
	{
		uint8 ucChar = strData2.c_str()[i] & 0xFF;
		if (!(ucChar >= 48 && ucChar <= 57))
		{
			return false;
		}
	}
	return true;
}

bool				String::isAlphaNumericUnderscore(string& strData, bool bAllowDigitAtStart)
{
	for (uint32 i = 0, j = strData.length(); i < j; i++)
	{
		uint8 ucChar = strData.c_str()[i] & 0xFF;
		if ((bAllowDigitAtStart && (ucChar >= 48 && ucChar <= 57)) || (ucChar >= 65 && ucChar <= 90) || (ucChar >= 97 && ucChar <= 122) || ucChar == 95)
		{
		}
		else
		{
			return false;
		}
	}
	return true;
}

void				String::setClipboardText(string& strText)
{
	size_t iTextLength = strText.length() + 1;
	HGLOBAL glob = GlobalAlloc(GMEM_FIXED, iTextLength);
	memcpy(glob, strText.c_str(), iTextLength);

	if (OpenClipboard(NULL) != 0)
	{
		if (EmptyClipboard() != FALSE)
		{
			SetClipboardData(CF_TEXT, glob);
		}
		CloseClipboard();
	}
}

string				String::getClipboardText(void)
{
	// Try opening the clipboard
	if (!OpenClipboard(nullptr))
	{
		return "";
	}

	// Get handle of clipboard object for ANSI text
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Lock the handle to get the actual text pointer
	char *pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr)
	{
		CloseClipboard();
		return "";
	}

	// Save text in a string class instance
	string strText(pszText);
	
	// Release the lock
	GlobalUnlock(hData);
	
	// Release the clipboard
	CloseClipboard();

	return strText;
}

string				String::capLength(string& strData, uint32 uiMaxLength)
{
	if (strData.length() > uiMaxLength)
	{
		return strData.substr(0, uiMaxLength);
	}
	return strData;
}

uint32		String::getCurrentEpochTime(void)
{
	string strEpochTime = (char*)time(NULL);
	return String::toNumber(strEpochTime);
}

string				String::mergeStrings(string& strString1, string& strString, uint32 uiCapLength)
{
	if ((strString1.length() + strString.length()) <= uiCapLength)
	{
		return strString1 + strString;
	}

	if (strString1.length() > strString.length())
	{
		if (uiCapLength > strString.length())
		{
			strString1 = strString1.substr(0, uiCapLength - strString.length());
		}
	}
	else if (strString1.length() < strString.length())
	{
		if (uiCapLength > strString1.length())
		{
			strString = strString.substr(0, uiCapLength - strString1.length());
		}
	}

	uint32 uiLength = uiCapLength / 2;
	return strString1.substr(0, uiLength) + strString.substr(0, uiLength + ((uiCapLength % 2) == 1 ? 1 : 0));
}

bool				String::doesStartWith(string& strText, string& strStartWith, bool bMatchCase)
{
	if (strText.length() < strStartWith.length())
	{
		return false;
	}

	string strStartWith2 = strText.substr(0, strStartWith.length());
	if (!bMatchCase)
	{
		strStartWith = String::upper(strStartWith);
		strStartWith2 = String::upper(strStartWith2);
	}
	if (strStartWith2 == strStartWith)
	{
		return true;
	}
	
	return false;
}

bool				String::isPositiveInteger(string& strText)
{
	for (uint32 i = 0, j = strText.length(); i < j; i++)
	{
		uint8 ucChar = strText.c_str()[i] & 0xFF;
		if (ucChar >= 48 && ucChar <= 57)
		{
		}
		else
		{
			return false;
		}
	}
	return true;
}

string				String::addNumberGrouping(string& strNumber)
{
	if (strNumber.length() == 0)
	{
		return "";
	}

	uint32
		uiDigitOffset = 0,
		uiOldDigitsLength = strNumber.length();
	char
		uiFirstChar = strNumber.c_str()[0];

	if (uiFirstChar == '-' || uiFirstChar == '+')
	{
		uiDigitOffset++;
		uiOldDigitsLength--;
	}

	string strNewDigits = "";
	uiDigitOffset += uiOldDigitsLength % 3;
	if (uiDigitOffset != 0)
	{
		strNewDigits = strNumber.substr(0, uiDigitOffset);
	}
	for (uint32 i = 0, j = uiOldDigitsLength / 3; i < j; i++)
	{
		if (i != 0 || uiDigitOffset != 0)
		{
			strNewDigits += ",";
		}
		strNewDigits += strNumber.substr(uiDigitOffset, 3);
		uiDigitOffset += 3;
	}
	return strNewDigits;
}

string				String::fixEOLs(string& strData, string& strEOLCharsToUse)
{
	string
		strEOL_Windows = "\r\n",
		strEOL_Linux = "\n",
		strEOL_OldMac = "\r",
		str;
	if (strEOLCharsToUse == strEOL_Linux)
	{
		str = String::replace(strData, strEOL_Windows, strEOL_Linux);
		return String::replace(str, strEOL_OldMac, strEOL_Linux);
	}
	else if (strEOLCharsToUse == strEOL_OldMac)
	{
		str = String::replace(strData, strEOL_Windows, strEOL_OldMac);
		return String::replace(str, strEOL_Linux, strEOL_OldMac);
	}
	else if (strEOLCharsToUse == strEOL_Windows)
	{
		str = String::replace(strData, strEOL_Windows, strEOL_Linux);
		str = String::replace(str, strEOL_OldMac, strEOL_Linux);
		return String::replace(str, strEOL_Linux, strEOL_Windows);
	}
	return strData;
}

uint32				String::swapEndian(uint32 uiValue)
{
	return
		(uiValue >> 24) & 0xFF |
		(((uiValue >> 16) & 0xFF) << 8) |
		(((uiValue >> 8) & 0xFF) << 16) |
		((uiValue & 0xFF) << 24)
	;
}

bool				String::isAsciiCharacterDisplayable(uint8 uiCharCode, bool bIncludeSpace, bool bIncludeTab)
{
	return (uiCharCode > 32 && uiCharCode < 127)
		|| (bIncludeSpace && uiCharCode == 32)
		|| (bIncludeTab && uiCharCode == 9);
}

string				String::createCharString(uint8 uiCharCode)
{
	return string(1, uiCharCode);
}

string				String::getTextInSameCase(string& strText, string& strTextCase)
{
	string strOutString = "";
	for (uint32 i = 0, j = strText.length(); i < j; i++)
	{
		uint8 ucInChar = strTextCase.c_str()[i];
		string strOutChar = string(1, strText.c_str()[i]);
		strOutString += ucInChar > 64 && ucInChar < 91 ? upper(strOutChar) : lower(strOutChar);
	}
	return strOutString;
}

string				String::extractQuotedData(string& strData, uint32 uiQuoteIndex)
{
	uint32
		uiActiveQuoteIndex = 0,
		uiDataCharIndex = 0,
		uiQuoteStartCharIndex,
		uiQuoteEndCharIndex;
	do
	{
		uiQuoteStartCharIndex = strData.find("\"", uiDataCharIndex);
		uiQuoteEndCharIndex = strData.find("\"", uiQuoteStartCharIndex + 1);
		if (uiActiveQuoteIndex == uiQuoteIndex)
		{
			return strData.substr(uiQuoteStartCharIndex + 1, ((uiQuoteEndCharIndex - uiQuoteStartCharIndex) + 1) - 2);
		}
		uiActiveQuoteIndex++;
		uiDataCharIndex = uiQuoteEndCharIndex + 1;
	} while (uiActiveQuoteIndex <= uiQuoteIndex && uiDataCharIndex < strData.length());
	return "";
}

string				String::collectAsString(string& strData, string& strCharactersToCollect)
{
	string strCollectedCharacters = "";
	for (uint32 uiIndex = 0, uiCount = strData.length(); uiIndex < uiCount; uiIndex++)
	{
		char iChar = strData.c_str()[uiIndex];
		if (strCharactersToCollect.find(iChar) != string::npos)
		{
			strCollectedCharacters += string(1, iChar);
		}
	}
	return strCollectedCharacters;
}

string				String::getToken(string& strData, string& strToken, uint32 uiTokenIndex)
{
	vector<string> vecTokens = split(strData, strToken);
	if (uiTokenIndex >= vecTokens.size())
	{
		return "";
	}
	return vecTokens[uiTokenIndex];
}

bool				String::isStringChars(string& strData)
{
	const char *pData = strData.c_str();
	char cChar;
	for (uint32 i = 0, j = strData.length(); i < j; i++)
	{
		cChar = pData[i];
		if (!((cChar > 64 && cChar < 91) || (cChar > 96 && cChar < 123)))
		{
			return false;
		}
	}
	return true;
}

bool				String::isUintBetween(string& strData, uint32 uiMin, uint32 uiMax)
{
	if (!isPositiveInteger(strData))
	{
		return false;
	}

	uint32 uiNumber = toUint32(strData);
	return uiNumber >= uiMin && uiNumber <= uiMax;
}

string				String::reverse(string& strValue)
{
	string strValue2 = "";
	for (int32 i = strValue.length() - 1; i >= 0; i--)
	{
		strValue2.append(1, strValue[i]);
	}
	return strValue2;
}

uvec2				String::getVisibleTextTokenRange(string& strText, uint32 uiCharXPosition)
{
	uint32
		uiCharStartXPosition = uiCharXPosition,
		uiCharEndXPosition = uiCharXPosition,
		uiPosition;

	uiPosition = uiCharXPosition;
	while (uiPosition > 0 && String::isAsciiCharacterDisplayable(strText.substr(uiPosition - 1, 1).c_str()[0], false, false))
	{
		uiPosition--;
	}
	uiCharStartXPosition = uiPosition;

	uiPosition = uiCharXPosition;
	while ((uiPosition + 1) <= strText.length() && String::isAsciiCharacterDisplayable(strText.substr(uiPosition + 1, 1).c_str()[0], false, false))
	{
		uiPosition++;
	}
	uiCharEndXPosition = uiPosition;

	return uvec2(uiCharStartXPosition, uiCharEndXPosition);
}

vec2				String::getVec2FromTokens(vector<string>& vecTokens, uint32 uiStartIndex)
{
	return vec2(String::toFloat32(vecTokens[uiStartIndex]), String::toFloat32(vecTokens[uiStartIndex + 1]));
}

vec3				String::getVec3FromTokens(vector<string>& vecTokens, uint32 uiStartIndex)
{
	return vec3(String::toFloat32(vecTokens[uiStartIndex]), String::toFloat32(vecTokens[uiStartIndex + 1]), String::toFloat32(vecTokens[uiStartIndex + 2]));
}