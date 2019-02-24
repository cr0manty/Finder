#pragma once
#include "includes.h"

class Window
{
	HINSTANCE hInst;
public:
	MSG msg;
	Window(HINSTANCE);
	void init_window(int);
};

