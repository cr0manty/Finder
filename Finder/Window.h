#pragma once
#include "includes.h"

class Window
{
	HINSTANCE hInst;
	HWND hWnd;
	HACCEL haccel;
public:
	MSG msg;
	Window(HINSTANCE);
	void init_window(int);
};

