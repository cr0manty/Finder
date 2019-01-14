#pragma once

class SmartStringLoad
{
	char * string;
public:
	SmartStringLoad();
	SmartStringLoad(int);
	SmartStringLoad(int, int);

	char * _get() const;
	char *_set_and_get(int, int = 32);

	~SmartStringLoad();
};

