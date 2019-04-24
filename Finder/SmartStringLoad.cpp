#include "includes.h"

SmartStringLoad::SmartStringLoad() :
	hInst(GetModuleHandle(NULL))
{
}

char * SmartStringLoad::_get(int _id, int _size)
{
	if (m_Data)
		delete[] m_Data;

	m_Data = new char[_size];
	LoadString(hInst, _id, m_Data, _size - 1);

	return m_Data;
}

SmartStringLoad::~SmartStringLoad()
{
	if (m_Data)
		delete[] m_Data;
}
