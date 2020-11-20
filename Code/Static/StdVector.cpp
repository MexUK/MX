#include "StdVector.h"
#include "Static/String.h"
#include <algorithm>

using namespace std;
using namespace mx;

vector<string>		StdVector::combineVectors(vector<string>& vecVector1, vector<string>& vecVector2)
{
	vector<string> vecValues;
	for (string& strValue : vecVector1)
	{
		vecValues.push_back(strValue);
	}
	for (string& strValue : vecVector2)
	{
		vecValues.push_back(strValue);
	}
	return vecValues;
}

unordered_map<string, bool>		StdVector::convertVectorToUnorderedMap(vector<string>& vecVector)
{
	unordered_map<string, bool> umapValues;
	for (string& strValue : vecVector)
	{
		umapValues[strValue] = true;
	}
	return umapValues;
}

vector<string>		StdVector::lower(vector<string>& vecVector)
{
	vector<string> vecValues;
	vecValues.resize(vecVector.size());
	uint32 i = 0;
	for (string& strValue : vecVector)
	{
		vecValues[i++] = String::lower(strValue);
	}
	return vecValues;
}

vector<string>		StdVector::upper(vector<string>& vecVector)
{
	vector<string> vecValues;
	vecValues.resize(vecVector.size());
	uint32 i = 0;
	for (string& strValue : vecVector)
	{
		vecValues[i++] = String::upper(strValue);
	}
	return vecValues;
}

void				StdVector::addUniqueEntry(vector<string>& vecVector, string& strValue)
{
	const auto& it = find(vecVector.begin(), vecVector.end(), strValue);
	if (it == vecVector.end())
	{
		vecVector.push_back(strValue);
	}
}

vector<string>		StdVector::getUniqueEntries(vector<string>& vecVector1, vector<string>& vecVector2)
{
	vector<string>
		vecVector3 = upper(vecVector1),
		vecVector4 = upper(vecVector2),
		vecUniqueEntries;

	for (uint32 i = 0, j = vecVector3.size(); i < j; i++)
	{
		string& strEntry1 = vecVector1[i];

		if (find(vecVector4.begin(), vecVector4.end(), vecVector3[i]) == vecVector4.end())
		{
			vecUniqueEntries.push_back(strEntry1);
		}
	}

	return vecUniqueEntries;
}

bool				StdVector::isIn(vector<string>& vecVector, string& strLookFor)
{
	return find(vecVector.begin(), vecVector.end(), strLookFor) != vecVector.end();
}

bool				sortStdVectorAzCaseInsensitive(string& strString1, string& strString)
{
	return strcmp(strString1.c_str(), strString.c_str()) < 0;
}

void				StdVector::sortAZCaseInsensitive(vector<string>& vecVector)
{
	sort(vecVector.begin(), vecVector.end(), sortStdVectorAzCaseInsensitive);
}

vector<vector<string>>	StdVector::swap2D(vector<vector<string>>& vecVector2D)
{
	vector<vector<string>> vecOutVector;
	if (vecVector2D.size() == 0)
	{
		return vecOutVector;
	}
	vecOutVector.resize(vecVector2D[0].size());
	uint32 i = 0;
	for (string& strValue : vecVector2D[0])
	{
		vecOutVector[i++] = { strValue };
	}
	return vecOutVector;
}