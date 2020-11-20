#pragma once

#include "mx.h"
#include "Types.h"
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

template <class T>
class mx::VectorPool
{
public:
	// entry existence
	void									addEntry(T entry);
	void									addEntryAtPosition(T entry, uint32 uiPosition);
	virtual void							removeEntry(T entry, bool bDeleteEntryIfPointer = true);
	virtual void							removeAllEntries(bool bDeleteEntriesIfPointers = true);
	bool									isEntryAtIndex(uint32 uiEntryIndex);
	uint32									getEntryCount(void);
	uint32									getNextEntryIndex(void);

	void									setEntries(std::vector<T>& vecEntries) { m_vecEntries = vecEntries; }
	std::vector<T>&							getEntries(void) { return m_vecEntries; }

	// entry set/get
	void									setEntryByIndex(uint32 uiEntryIndex, T entry);
	T										getEntryByIndex(uint32 uiEntryIndex);

	void									moveEntryToPosition(T entry, uint32 uiPosition); // todo - rename to setIndexByEntry
	uint32									getIndexByEntry(T entry);

	T										getFirstEntry(void);
	T										getLastEntry(void);

	void									swapEntries(T entry1, T entry2);

private:
	// memory deallocation
	void									deletePotentialPointerEntry(T pEntry, std::true_type);
	void									deletePotentialPointerEntry(T entry, std::false_type);

protected:
	// entry storage
	std::vector<T>							m_vecEntries;
};




template <class T>
bool				mx::VectorPool<T>::isEntryAtIndex(uint32 uiEntryIndex)
{
	return uiEntryIndex < m_vecEntries.size();
}

template <class T>
uint32				mx::VectorPool<T>::getEntryCount(void)
{
	return m_vecEntries.size();
}

template <class T>
T					mx::VectorPool<T>::getEntryByIndex(uint32 uiEntryIndex)
{
	if (isEntryAtIndex(uiEntryIndex))
	{
		return m_vecEntries[uiEntryIndex];
	}
	return nullptr;
}

template <class T>
void				mx::VectorPool<T>::removeAllEntries(bool bDeleteEntriesIfPointers)
{
	if (bDeleteEntriesIfPointers)
	{
		for (T entry : m_vecEntries)
		{
			deletePotentialPointerEntry(entry, std::is_pointer<T>());
		}
	}
	m_vecEntries.clear();
}

template <class T>
void				mx::VectorPool<T>::addEntry(T entry)
{
	m_vecEntries.push_back(entry);
}

template <class T>
void				mx::VectorPool<T>::addEntryAtPosition(T entry, uint32 uiPosition)
{
	m_vecEntries.insert(m_vecEntries.begin() + uiPosition, entry);
}

template <class T>
void				mx::VectorPool<T>::removeEntry(T entry, bool bDeleteEntryIfPointer)
{
	const auto& it = std::find(m_vecEntries.begin(), m_vecEntries.end(), entry);
	m_vecEntries.erase(it);
	if (bDeleteEntryIfPointer)
	{
		deletePotentialPointerEntry(entry, std::is_pointer<T>());
	}
}

template <class T>
void				mx::VectorPool<T>::deletePotentialPointerEntry(T pEntry, std::true_type)
{
	delete pEntry;
	pEntry = nullptr;
}

template <class T>
void				mx::VectorPool<T>::deletePotentialPointerEntry(T entry, std::false_type)
{
}

template <class T>
uint32				mx::VectorPool<T>::getNextEntryIndex(void)
{
	return m_vecEntries.size();
}

template <class T>
T					mx::VectorPool<T>::getFirstEntry(void)
{
	return m_vecEntries[0];
}

template <class T>
T					mx::VectorPool<T>::getLastEntry(void)
{
	return m_vecEntries[m_vecEntries.size() - 1];
}

template <class T>
uint32				mx::VectorPool<T>::getIndexByEntry(T entry)
{
	for (uint32 i = 0, j = m_vecEntries.size(); i < j; i++)
	{
		if (m_vecEntries[i] == entry)
		{
			return i;
		}
	}
	return -1;
}

template <class T>
void				mx::VectorPool<T>::setEntryByIndex(uint32 uiEntryIndex, T entry)
{
	m_vecEntries[uiEntryIndex] = entry;
}

template <class T>
void				mx::VectorPool<T>::swapEntries(T entry1, T entry2)
{
	uint32 uiEntryIndex1 = getIndexByEntry(entry1);
	uint32 uiEntryIndex2 = getIndexByEntry(entry2);
	setEntryByIndex(uiEntryIndex1, entry2);
	setEntryByIndex(uiEntryIndex2, entry1);
}

template <class T>
void				mx::VectorPool<T>::moveEntryToPosition(T entry, uint32 uiPosition)
{
	m_vecEntries.erase(m_vecEntries.begin() + getIndexByEntry(entry));
	m_vecEntries.insert(m_vecEntries.begin() + uiPosition, entry);
}