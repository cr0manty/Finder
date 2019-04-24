#pragma once

class SmartStringLoad
{
	char * m_Data;
	HINSTANCE hInst;
public:
	SmartStringLoad();
	char *_get(int, int = 32);

	~SmartStringLoad();
};

