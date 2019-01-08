#include "Finder.h"

LRESULT __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Finder *main;
	LPNMHDR lpnmHdr;

	switch (message)
	{
	case WM_CREATE:
		main = new Finder(hWnd);
		break;

	case WM_SIZE:
		main->resize_objects();
		break;

	case WM_NOTIFY:
		lpnmHdr = (LPNMHDR)lParam;
		switch (lpnmHdr->code)
		{
		case NM_DBLCLK:
			main->open();
			break;

		case NM_CLICK:
			main->select_item();
			break;

		case TVN_SELCHANGED:
			main->tree_select();
			break;
		}
		break;

	case WM_CONTEXTMENU:
		main->select_item();
		main->context_menu(lParam);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_COPY_ITEM:
			main->file_manip(false);
			break;

		case ID_CUT_ITEM:
			main->file_manip(true);
			break;

		case ID_OPEN_ITEM:
			main->open();
			break;

		case ID_LINK_ITEM:
			main->create_link();
			break;

		case ID_REFRESH_BTN:
			main->refresh();
			break;

		case ID_PASTE_ITEM:
			main->paste();
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
			main->back_button();
			break;

		case ID_NEXT_BUTTON:
			main->next_button();
			break;

		case ID_DISKLIST_CB:
			main->disk_change(wParam);
			break;

		case ID_PROGRAM_ABOUT:
			main->show_about();
			break;

		case ID_PROGRAM_CLOSE:
			PostQuitMessage(0);
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

BOOL __stdcall DlgInfo(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND Object[6];
	HWND Object_info[6];

	WIN32_FIND_DATA file;
	std::string *temp;
	Finder *main;
	HANDLE hFind;

	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, local_ru::DialogInfoName);

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
			SendMessage(Object_info[i], WM_SETTEXT, (WPARAM)255, (LPARAM)local_ru::header[i].c_str());
		}
		SendMessage(Object[5], WM_SETTEXT, (WPARAM)255, (LPARAM)main->_get_path().main_path.c_str());
		SendMessage(Object_info[5], WM_SETTEXT, (WPARAM)255, (LPARAM)local_ru::PathInfo);


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

BOOL __stdcall DlgAbout(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND obj;
	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg, local_ru::DialogAboutName);

		obj = GetDlgItem(hDlg, ID_ABOUT_STATIC);
		SendMessage(obj, WM_SETTEXT, (WPARAM)1024, (LPARAM)local_ru::Copyright);
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

ATOM MyRegisterClass(HINSTANCE hInstance)
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance;
	hWnd = CreateWindow(wnd_class,
		local_ru::window_name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		LoadMenu(hInst,MAKEINTRESOURCE(ID_MAIN_MENU)),
		hInstance,
		NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
