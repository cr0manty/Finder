#include "Finder.h"

__int64 __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Finder *main;

	switch (message)
	{
	case WM_CREATE:
		main = new Finder(hWnd);
		break;

	case WM_SIZE:
		main->resize();
		break;

	case WM_NOTIFY:
		switch (LPNMHDR(lParam)->code)
		{
		case NM_DBLCLK:
			main->open();
			break;

		case NM_CLICK:
			main->select_item();
			break;

		case TVN_SELCHANGED:
			main->tree_to_list();
			break;

		case TVN_ITEMEXPANDED: 
			main->tree_show(lParam);
			break;
		}
		break;

	case WM_CONTEXTMENU:
		main->context_menu(lParam);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_COPY_ITEM:
			main->file_manip(Copy);
			break;

		case ID_CUT_ITEM:
			main->file_manip(Paste);
			break;

		case ID_OPEN_ITEM:
			main->open();
			break;

		case ID_LINK_ITEM:
			main->create_link();
			break;

		case ID_REFRESH_BTN:
			main->make_refresh();
			break;

		case ID_PASTE_ITEM:
			main->make_paste();
			break;

		case ID_RENAME_ITEM:
			main->rename();
			break;

		case ID_CREATE_FOLDER:
			main->create_folder();
			break;

		case ID_CREATE_TEXT_ITEM:
			main->create_txt();
			break;

		case ID_INFO_ITEM:
			main->show_info();
			break;

		case ID_DELETE_ITEM:
			main->delete_item();
			break;

		case ID_BACK_BUTTON:
			main->show_back();
			break;

		case ID_NEXT_BUTTON:
			main->show_next();
			break;

		case ID_DISKLIST_CB:
			main->disk_change(wParam);
			break;

		case ID_PROGRAM_ABOUT:
			main->show_about();
			break;

		case ID_PROGRAM_CLOSE:
			main->exit();
			break;

		case Menu_change_lang_ru:
			main->change_lang(Menu_change_lang_ru);
			break;

		case Menu_change_lang_en:
			main->change_lang(Menu_change_lang_en);
			break;

		case Menu_change_lang_ua:
			main->change_lang(Menu_change_lang_ua);
			break;
		}
		break;

	case WM_HOTKEY:
		switch (wParam)
		{
		case ID_DELETE_HK:
			main->delete_item();
			break;

		case ID_COPY_HK:
			main->file_manip(Copy);
			break;

		case ID_CUT_HK:
			main->file_manip(Paste);
			break;

		case ID_PASTE_HK:
			main->make_paste();
			break;

		case ID_REFRESH_HK:
			main->make_refresh();
			break;

		case ID_MINIM_HK:
			main->minimize_window();
			break;

		case ID_BACK_HK:
			main->show_back();
			break;

		case ID_NEXT_HK:
			main->show_next();
			break;
		}
		break;

	case WM_GETMINMAXINFO:
		main->fix_size(lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool __stdcall DlgInfo(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND Object[6];
	HWND Object_info[6];

	WIN32_FIND_DATA file;
	std::string *temp;
	Finder *main;
	HANDLE hFind;
	std::string header[5] = { (char*)"Имя", (char*)"Дата изменения", (char*)"Тип", (char*)"Размер" , (char*)"Дата создания" };
	char *temp_local;

	switch (msg)
	{
	case WM_INITDIALOG:
		temp_local = new char[32];
		LoadString(hInst, DialogAboutName, temp_local, 32);
		SetWindowText(hDlg, temp_local);
		delete[] temp_local;

		main = (Finder*)lParam;
		if (hFind = FindFirstFile(main->_get_path().selected_file.c_str(), &file))
			temp = main->make_file_info(file);
		else return FALSE;

		Object[0] = GetDlgItem(hDlg, IDC_STATIC1);
		Object[2] = GetDlgItem(hDlg, IDC_STATIC2);
		Object[3] = GetDlgItem(hDlg, IDC_STATIC3);
		Object[4] = GetDlgItem(hDlg, IDC_STATIC4);
		Object[1] = GetDlgItem(hDlg, IDC_STATIC5);
		Object[5] = GetDlgItem(hDlg, IDC_EDIT2);

		Object_info[5] = GetDlgItem(hDlg, IDC_STATIC_STATIC1);
		Object_info[0] = GetDlgItem(hDlg, IDC_STATIC_STATIC2);
		Object_info[2] = GetDlgItem(hDlg, IDC_STATIC_STATIC3);
		Object_info[3] = GetDlgItem(hDlg, IDC_STATIC_STATIC4);
		Object_info[4] = GetDlgItem(hDlg, IDC_STATIC_STATIC5);
		Object_info[1] = GetDlgItem(hDlg, IDC_STATIC_STATIC6);


		for (int i = 0; i < 5; i++) {
			SendMessage(Object[i], WM_SETTEXT, (WPARAM)255, (LPARAM)temp[i].c_str());
			SendMessage(Object_info[i], WM_SETTEXT, (WPARAM)255, (LPARAM)header[i].c_str());
		}
		temp_local = new char[32];
		LoadString(hInst, PathInfo, temp_local, 32);

		SendMessage(Object[5], WM_SETTEXT, (WPARAM)255, (LPARAM)main->_get_path().main_path.c_str());
		SendMessage(Object_info[5], WM_SETTEXT, (WPARAM)255, (LPARAM)temp_local);
		delete[] temp_local;

		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_BUTTON_DLG || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
			
		break;
	}
	return FALSE;
}

bool __stdcall DlgAbout(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND obj;
	SmartStringLoad str;

	switch (msg)
	{
	case WM_INITDIALOG:
		str.new_element_1(DialogAboutName);
		str.new_element_2(Copyright, 1024);
		SetWindowText(hDlg, str._get_1());

		obj = GetDlgItem(hDlg, ID_ABOUT_STATIC);
		SendMessage(obj, WM_SETTEXT, (WPARAM)1024, (LPARAM)str._get_2());
		return TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

unsigned short MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(ID_FINDER_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = wnd_class;
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex);
}

bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	SmartStringLoad str(window_name);
	hInst = hInstance;
	hWnd = CreateWindow(wnd_class,
		str._get_1(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return false;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return true;
}
