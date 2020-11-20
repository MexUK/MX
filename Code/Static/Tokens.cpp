#include "Tokens.h"

using namespace std;

// token count
uint32					Tokens::getTokenCount(string& strText, string& strDelimiter)
{
	uint32
		uiTextLength = strText.length(),
		uiDelimiterLength = strDelimiter.length(),
		uiMaxSearchStartIndex = uiTextLength - uiDelimiterLength,
		uiSearchStartIndex = 0,
		uiSearchMatchIndex = 0,
		uiTokenCount = 0;
	bool
		bNonDelimiterCharacterFound = false;

	while(uiSearchStartIndex <= uiMaxSearchStartIndex)
	{
		uiSearchMatchIndex = strText.find(strDelimiter, uiSearchStartIndex);
		if (uiSearchMatchIndex == string::npos)
		{
			break;
		}

		if (uiSearchMatchIndex != (uiSearchStartIndex + uiDelimiterLength))
		{
			bNonDelimiterCharacterFound = true;
		}

		if (bNonDelimiterCharacterFound)
		{
			uiTokenCount++;
		}

		uiSearchStartIndex = uiSearchMatchIndex + uiDelimiterLength;
	}

	if (uiSearchStartIndex != uiSearchMatchIndex)
	{
		uiTokenCount++;
	}

	return uiTokenCount;
}

// add/remove token
string					Tokens::addToken(string& strText, string& strToken, string& strDelimiter)
{
	if (strText == "")
	{
		return strToken;
	}
	else
	{
		return strText + strDelimiter + strToken;
	}
}

string					Tokens::addTokenRightShift(string& strText, string& strToken, string& strDelimiter)
{
	string str = removeToken(strText, 0, strDelimiter);
	return addToken(str, strToken, strDelimiter);
}

string					Tokens::addTokenRightShiftMax(string& strText, string& strToken, string& strDelimiter, uint32 uiMaxTokens)
{
	if (getTokenCount(strText, strDelimiter) == uiMaxTokens)
	{
		return addTokenRightShift(strText, strToken, strDelimiter);
	}
	else
	{
		return addToken(strText, strToken, strDelimiter);
	}
}

string					Tokens::removeToken(string& strText, uint32 uiTokenIndex, string& strDelimiter)
{
	uint32 uiDelimiterIndex = strText.find(strDelimiter, 0);
	if (uiDelimiterIndex == -1)
	{
		return "";
	}
	else
	{
		return strText.substr(uiDelimiterIndex + strDelimiter.length());
	}
}