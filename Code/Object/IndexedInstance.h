#pragma once

#include "mx.h"
#include <vector>

template<class Class>
class mx::IndexedInstance
{
public:
	static Class*						get(uint32 uiInstanceIndex = 0);
	static std::vector<Class*>&			gets(void) { return m_vecInstances; }

private:
	static std::vector<Class*>			m_vecInstances;
};

template <class Class>
std::vector<Class*>						mx::IndexedInstance<Class>::m_vecInstances;

template <class Class>
Class*									mx::IndexedInstance<Class>::get(uint32 uiInstanceIndex)
{
	if (uiInstanceIndex >= m_vecInstances.size())
	{
		m_vecInstances.resize(uiInstanceIndex + 1);
		m_vecInstances[uiInstanceIndex] = new Class;
	}
	return m_vecInstances[uiInstanceIndex];
}