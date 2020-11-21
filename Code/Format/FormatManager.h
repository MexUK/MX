#pragma once

#include "mx.h"
#include <string>

template<class T>
class mx::FormatManager
{
public:
	static T*					unserializeMemory(std::string& strData);
	static T*					unserializeFile(std::string& strFilePath);

	static T*					unserializeManyMemory(std::vector<std::string>& vecData);
	static T*					unserializeManyFile(std::vector<std::string>& vecFilePaths);
};

template<class T>
T*			mx::FormatManager<T>::unserializeMemory(std::string& strData)
{
	T *pFormat = new T(strData, false);
	pFormat->unserialize();
	return pFormat;
}

template<class T>
T*			mx::FormatManager<T>::unserializeFile(std::string& strFilePath)
{
	T *pFormat = new T(strFilePath);
	pFormat->unserialize();
	return pFormat;
}

template<class T>
T*			mx::FormatManager<T>::unserializeManyMemory(std::vector<std::string>& vecData)
{
	T
		*pFormat = new T[vecData.size()],
		*pFormat2 = pFormat;
	for (std::string& strData : vecData)
	{
		pFormat2->setData(strData);
		pFormat2->unserialize();
		pFormat2++;
	}
	return pFormat;
}

template<class T>
T*			mx::FormatManager<T>::unserializeManyFile(std::vector<std::string>& vecFilePaths)
{
	T
		*pFormat = new T[vecFilePaths.size()],
		*pFormat2 = pFormat;
	for (std::string& strFilePath : vecFilePaths)
	{
		pFormat2->setFilePath(strFilePath);
		pFormat2->unserialize();
		pFormat2++;
	}
	return pFormat;
}