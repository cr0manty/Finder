#include "Finder.h"

__int64 __stdcall MainFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Finder *main;

	switch (message)
	{
	case WM_CREATE:
		main = new Finder(hWnd);
		return true;

	case WM_NOTIFY:
		return main->notify_switch(lParam);

	case WM_CONTEXTMENU:
		return main->context_menu(lParam);

	case WM_COMMAND:
		return main->command_switch(wParam);

	case WM_HOTKEY:
		return main->hotkey_switch(wParam);

	case WM_GETMINMAXINFO:
		return main->fix_size(lParam);

	case WM_SIZE:
		return main->resize();

	case WM_DESTROY:
		delete main;
		return true;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return false;
}

bool __stdcall DlgInfo(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Finder *main;

	switch (msg)
	{
	case WM_INITDIALOG:
		main = reinterpret_cast<Finder*>(lParam);
		if(!main->dlgInfo_proc(hDlg))
			EndDialog(hDlg, LOWORD(wParam));
		return true;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BUTTON_DLG || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return true;
		}
		break;
	}
	return false;
}

bool __stdcall DlgAbout(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SmartStringLoad str;
	RECT rt;
	static int i = IDB_BITMAP1;
	static int timer = 111;
	static Shrek *shrek;

	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, str._get(DialogAboutName));
		SendMessage(GetDlgItem(hDlg, ID_ABOUT_STATIC), WM_SETTEXT, (WPARAM)1024, reinterpret_cast<__int64>(str._get(Copyright, 1024)));
		SetTimer(hDlg, timer, 70, NULL);
		GetClientRect(hDlg, &rt);
		shrek = new Shrek(hDlg, rt);
		return true;
		
	case WM_TIMER:
		i = i == IDB_BITMAP36 ? i = IDB_BITMAP1 : i + 1;
		InvalidateRect(hDlg, NULL, TRUE);
		break;

	case WM_PAINT:
		shrek->make_picture(i);
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			KillTimer(hDlg, timer);
			EndDialog(hDlg, LOWORD(wParam));
			delete shrek;
			return true;
		}
		return false;
	}
	return false;
}
