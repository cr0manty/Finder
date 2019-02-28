#include "Window.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine,
	int nCmdShow)
{
	Window wnd(hInstance);
	wnd.init_window(nCmdShow);

	while (GetMessage(&wnd.msg, NULL, 0, 0)) {
		TranslateMessage(&wnd.msg);
		DispatchMessage(&wnd.msg);
	}

	return wnd.msg.wParam;
}