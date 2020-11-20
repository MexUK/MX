#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <set>

class mx::StdVector
{
public:
	static std::vector<std::string>					combineVectors(std::vector<std::string>& vecVector1, std::vector<std::string>& vecVector2);
	template <class T>
	static std::vector<T>							combineVectors(std::vector<T>& vecVector1, std::vector<T>& vecVector2);
	template <class T>
	static void										addToVector(std::vector<T>& vecVector1, std::vector<T>& vecVector2);
	template <class T>
	static std::deque<T>							convertVectorToDeque(std::vector<T>& vecVector);
	static std::unordered_map<std::string, bool>	convertVectorToUnorderedMap(std::vector<std::string>& vecVector);
	static std::vector<std::string>					lower(std::vector<std::string>& vecVector);
	static std::vector<std::string>					upper(std::vector<std::string>& vecVector);
	template <class T>
	static std::vector<T>							removeDuplicates(std::vector<T>& vecVector);
	template <class T>
	static uint32									findKey(std::vector<T>& vecVector, T value);
	template <class T>
	static uint32									findKey(std::deque<T>& deqDeque, T value);
	static void										addUniqueEntry(std::vector<std::string>& vecVector, std::string& strValue);
	static std::vector<std::string>					getUniqueEntries(std::vector<std::string>& vecVector1, std::vector<std::string>& vecVector2);
	static bool										isIn(std::vector<std::string>& vecVector1, std::string& strLookFor);
	static void										sortAZCaseInsensitive(std::vector<std::string>& vecVector);
	template <typename T>
	static std::vector<std::string>					mapSorted(std::vector<T>& vecVector, std::string(*fpCallback)(T));
	template <typename T1, typename T2>
	static std::vector<T1>							getKeysWithMatchingEntries(std::unordered_map<T1, std::vector<T2>>& umapIMGModelNames, std::set<T2>& stModelNames);
	static std::vector<std::vector<std::string>>	swap2D(std::vector<std::vector<std::string>>& vecVector2D);
};

template <class T>
static std::vector<T>								mx::StdVector::combineVectors(std::vector<T>& vecVector1, std::vector<T>& vecVector2)
{
	uint32 i = 0;
	std::vector<T> vecValues;
	vecValues.resize(vecVector1.size() + vecVector2.size());
	for (T& value : vecVector1)
	{
		vecValues[i++] = value;
	}
	for (T& value : vecVector2)
	{
		vecValues[i++] = value;
	}
	return vecValues;
}

template <class T>
void												mx::StdVector::addToVector(std::vector<T>& vecVector1, std::vector<T>& vecVector2)
{
	uint32 uiKey = vecVector1.size();
	vecVector1.resize(uiKey + vecVector2.size());
	for (T& strValue : vecVector2)
	{
		vecVector1[uiKey] = strValue;
		uiKey++;
	}
}

template <class T>
std::deque<T>										mx::StdVector::convertVectorToDeque(std::vector<T>& vecVector)
{
	uint32 uiEntryCount = vecVector.size();
	std::deque<T> deqValues;
	deqValues.resize(uiEntryCount);
	for(uint32 i = 0; i < uiEntryCount; i++)
	{
		deqValues[i] = vecVector[i];
	}
	return deqValues;
}

template <typename T>
std::vector<std::string>							mx::StdVector::mapSorted(std::vector<T>& vecVector, std::string(*fpCallback)(T))
{
	std::vector<std::string> vecOutVector;
	vecOutVector.resize(vecVector.size());
	uint32 i = 0;
	for (T uiValue : vecVector)
	{
		vecOutVector[i++] = fpCallback(uiValue);
	}
	sortAZCaseInsensitive(vecOutVector);
	return vecOutVector;
}

template <typename T1, typename T2>
std::vector<T1>										mx::StdVector::getKeysWithMatchingEntries(std::unordered_map<T1, std::vector<T2>>& umapIMGModelNames, std::set<T2>& stModelNames)
{
	std::vector<T1> vecItems;
	for (const auto& it : umapIMGModelNames)
	{
		for (T2& entry : it.second)
		{
			if (stModelNames.find(entry) != stModelNames.end())
			{
				vecItems.push_back(it.first);
			}
		}
	}
	return vecItems;
}

template <typename T>
std::vector<T>										mx::StdVector::removeDuplicates(std::vector<T>& vecVector)
{
	std::vector<T> vecNewVector;
	for (T& value : vecVector)
	{
		if (std::find(vecNewVector.begin(), vecNewVector.end(), value) == vecNewVector.end())
		{
			vecNewVector.push_back(value);
		}
	}
	return vecNewVector;
}

template <typename T>
uint32												mx::StdVector::findKey(std::vector<T>& vecVector, T value)
{
	for (uint32 i = 0; i < vecVector.size(); i++)
	{
		if (vecVector[i] == value)
		{
			return i;
		}
	}
	return -1;
}

template <typename T>
uint32												mx::StdVector::findKey(std::deque<T>& deqDeque, T value)
{
	for (uint32 i = 0; i < deqDeque.size(); i++)
	{
		if (deqDeque[i] == value)
		{
			return i;
		}
	}
	return -1;
}