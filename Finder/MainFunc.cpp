#include "Finder.h"

__int64 __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

	case WM_KEYDOWN:
		return main->key_down_switch(wParam);

	case WM_GETMINMAXINFO:
		return main->fix_size(lParam);

	case WM_SIZE:
		return main->resize();

	case WM_DESTROY:
		delete main;
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool __stdcall DlgInfo(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Finder *main;

	switch (msg)
	{
	case WM_INITDIALOG:
		main = (Finder*)lParam;
		main->dlgInfo_proc(hDlg, wParam);
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

	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, str._get(DialogAboutName));
		SendMessage(GetDlgItem(hDlg, ID_ABOUT_STATIC), WM_SETTEXT, (WPARAM)1024, (LPARAM)str._get(Copyright, 1024));
		return true;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return true;
		}
		break;
	}
	return false;
}
