#include "includes.h"

SmartStringLoad::SmartStringLoad()
{
}

SmartStringLoad::SmartStringLoad(int _str) :
	string_1(new char[32])
{
	LoadString(hInst, _str, string_1, 32);
}

SmartStringLoad::SmartStringLoad(int _str, int _str_2) :
	string_1(new char[32]),	string_2(new char[32])
{
	LoadString(hInst, _str, string_1, 32);
	LoadString(hInst, _str_2, string_2, 32);

}

SmartStringLoad::SmartStringLoad(int _str, int _str_2, int _size) :
	string_1(new char[_size]), string_2(new char[_size])

{
	LoadString(hInst, _str, string_1, _size);
	LoadString(hInst, _str_2, string_2, _size);

}

char * SmartStringLoad::_get_1() const
{
	return string_1;
}

char * SmartStringLoad::_get_2() const
{
	if(string_2)
		return string_2;
}

bool SmartStringLoad::new_element_1(int _id, int _size)
{
	if (string_1)
		delete[] string_1;

	string_1 = new char[_size];
	return LoadString(hInst, _id, string_1, _size);
}

bool SmartStringLoad::new_element_2(int _id, int _size)
{
	if (string_2)
		delete[] string_2;

	string_2 = new char[_size];
	return LoadString(hInst, _id, string_2, _size);
}

char *SmartStringLoad::_set_and_get(int _id, int _size)
{
	if (new_element_1(_id, _size))
		return string_1;
}

SmartStringLoad::~SmartStringLoad()
{
	if (string_1)
		delete[] string_1;

	if (string_2)
		delete[] string_2;
}
