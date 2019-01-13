#pragma once

class SmartStringLoad
{
	char * string_1;
	char * string_2;

public:
	SmartStringLoad();

	SmartStringLoad(int);
	SmartStringLoad(int, int);
	SmartStringLoad(int, int, int);

	char * _get_1() const;
	char * _get_2() const;

	char *_set_and_get(int, int = 32);

	bool new_element_1(int, int = 32);
	bool new_element_2(int, int = 32);
	
	~SmartStringLoad();
};

