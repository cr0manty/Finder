#include "includes.h"

SmartStringLoad::SmartStringLoad()
{
}

SmartStringLoad::SmartStringLoad(int _str) :
	string(new char[32])
{
	LoadString(hInst, _str, string, 32);
}

SmartStringLoad::SmartStringLoad(int _str, int _size) :
	string(new char[_size])
{
	LoadString(hInst, _str, string, _size);
}

char * SmartStringLoad::_get() const
{
	return string;
}

char * SmartStringLoad::_set_and_get(int _id, int _size)
{
	if (string)
		delete[] string;

	string = new char[_size];
	LoadString(hInst, _id, string, _size);

	return string;
}

SmartStringLoad::~SmartStringLoad()
{
	if (string)
		delete[] string;
}
