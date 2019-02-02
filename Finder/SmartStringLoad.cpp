#include "includes.h"

SmartStringLoad::SmartStringLoad() :
	hInst(GetModuleHandle(NULL))
{
}

char * SmartStringLoad::_get(int _id, int _size)
{
	if (string)
		delete[] string;

	string = new char[_size];
	LoadString(hInst, _id, string, _size - 1);

	return string;
}

SmartStringLoad::~SmartStringLoad()
{
	if (string)
		delete[] string;
}
