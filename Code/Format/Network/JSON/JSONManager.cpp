#include "JSONManager.h"
#include "Format/Network/JSON/JSONObject.h"
#include "Format/Network/JSON/JSONArray.h"
#include "Format/Network/JSON/JSONBoolean.h"
#include "Format/Network/JSON/JSONNull.h"
#include "Format/Network/JSON/JSONNumber.h"
#include "Static/String.h"

using namespace std;
using namespace mx;

JSONManager::JSONManager(void)
{
	m_pEntryContainer = new JSONObject;
	m_pLatestContainer = m_pEntryContainer;
	m_bExpectingKeyOrObjectEnd = true;
	m_pLatestKey = nullptr;
}

JSONObject*	JSONManager::unserializeString(string& strJSONRef)
{
	string strJSON = String::trim(strJSONRef).substr(1, strJSONRef.length() - 2);

	unsigned char ucChar;
	unsigned long j = strJSON.length();
	JSONContainer *pValue;
	JSONString *pString;
	JSONNumber *pNumber;
	for (m_uiSeek = 0; m_uiSeek < j; )
	{
		ucChar = strJSON.c_str()[m_uiSeek];
		switch(ucChar)
		{
		case '{':
			pValue = new JSONObject;
			m_pLatestContainer->addValue(getLatestKey(), pValue);
			m_pLatestContainer = pValue;
			m_uiSeek++;
			m_bExpectingKeyOrObjectEnd = true;
		break;
		case '}':
			m_pLatestContainer = m_pLatestContainer->m_pParentContainer;
			m_uiSeek++;
			m_bExpectingKeyOrObjectEnd = false;
		break;
		case '[':
			pValue = new JSONArray;
			m_pLatestContainer->addValue(getLatestKey(), pValue);
			m_pLatestContainer = pValue;
			m_uiSeek++;
		break;
		case ']':
			m_pLatestContainer = m_pLatestContainer->m_pParentContainer;
			m_uiSeek++;
			m_bExpectingKeyOrObjectEnd = false;
		break;
		case '"':
			m_uiSeek++;
			pString = new JSONString;
			pString->m_strValue = parseString(strJSON);
			if (m_bExpectingKeyOrObjectEnd)
			{
				setLatestKey(pString);
				m_bExpectingKeyOrObjectEnd = false;
			}
			else
			{
				m_pLatestContainer->addValue(getLatestKey(), pString);
			}
		break;
		case ':':
			m_bExpectingKeyOrObjectEnd = false;
			m_uiSeek++;
			break;
		case ',':
			m_bExpectingKeyOrObjectEnd = true;
			m_uiSeek++;
			break;
		default:
			if(memcmp(strJSON.c_str() + m_uiSeek, "true", 4) == 0)
			{
				JSONBoolean *pBool = new JSONBoolean;
				pBool->m_bValue = true;
				m_pLatestContainer->addValue(getLatestKey(), pBool);
				m_uiSeek += 4;
			}
			else if (memcmp(strJSON.c_str() + m_uiSeek, "false", 5) == 0)
			{
				JSONBoolean *pBool = new JSONBoolean;
				pBool->m_bValue = false;
				m_pLatestContainer->addValue(getLatestKey(), pBool);
				m_uiSeek += 5;
			}
			else if (memcmp(strJSON.c_str() + m_uiSeek, "null", 4) == 0)
			{
				JSONNull *pNull = new JSONNull;
				m_pLatestContainer->addValue(getLatestKey(), pNull);
				m_uiSeek += 4;
			}
			else if(ucChar <= 32 || ucChar >= 127)
			{
				m_uiSeek++;
			}
			else
			{
				pNumber = new JSONNumber;
				pNumber->m_dValue = parseNumber(strJSON);
				m_pLatestContainer->addValue(getLatestKey(), pNumber);
			}
		break;
		}
	}
	return m_pEntryContainer;
}

string		JSONManager::parseString(string& strJSON)
{
	string strString;
	unsigned char ucChar;
	for (; m_uiSeek < strJSON.length(); m_uiSeek++)
	{
		ucChar = strJSON.c_str()[m_uiSeek];
		if (ucChar == '"' && strJSON.c_str()[m_uiSeek - 1] != '\\')
		{
			m_uiSeek++;
			break;
		}
		else if (ucChar == '\\')
		{
			if ((strJSON.length() - 1) > m_uiSeek)
			{
				switch (strJSON.c_str()[m_uiSeek + 1])
				{
				case '"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					strString += strJSON.c_str()[m_uiSeek + 1];
					break;
				}
				m_uiSeek += 2;
			}
			if ((strJSON.length() - 1) > (m_uiSeek + 4))
			{
				if (strJSON.c_str()[m_uiSeek + 1] == 'x')
				{
					char szNumber[5];
					memcpy(szNumber, (void*)strJSON.c_str()[m_uiSeek + 2], 4);
					szNumber[4] = '\0';
					strString += atoi(szNumber) % 256;
				}
			}
		}
		else
		{
			strString += ucChar;
		}
	}
	return strString;
}
double			JSONManager::parseNumber(string& strJSON)
{
	string strNumber;
	bool bBreakLoop = false;
	unsigned char ucChar;
	for (; m_uiSeek < strJSON.length(); m_uiSeek++)
	{
		ucChar = strJSON.c_str()[m_uiSeek];
		switch (ucChar)
		{
		case '.':
		case 'e':
		case 'E':
		case '-':
		case '+':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			strNumber += ucChar;
			break;
		default:
			bBreakLoop = true;
		}
		if (bBreakLoop)
		{
			m_uiSeek++;
			break;
		}
	}
	return atof(strNumber.c_str());
}