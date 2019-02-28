#include "Window.h"

Window::Window(HINSTANCE _hInst) :
	hInst(_hInst)
{
	SmartStringLoad str;
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = reinterpret_cast<WNDPROC>(MainFunc);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ID_FINDER_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = str._get(WND_class);
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);
}

void Window::init_window(int _cmd)
{
	SmartStringLoad str, str_1;

	hWnd = CreateWindow(str._get(WND_class),
		str_1._get(window_name),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInst,
		NULL);

	ShowWindow(hWnd, _cmd);
	UpdateWindow(hWnd);
}