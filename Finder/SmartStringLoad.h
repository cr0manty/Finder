#pragma once

class SmartStringLoad
{
	char * string;
	HINSTANCE hInst;
public:
	SmartStringLoad();
	char *_get(int, int = 32);

	~SmartStringLoad();
};

