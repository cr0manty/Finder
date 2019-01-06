#include "Functional.h"

int Functional::set_listviw_colum()
{
	RECT rcl;
	GetClientRect(ListView, &rcl);
	int index = -1;

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = (rcl.right - rcl.left) / number_colum;
	lvc.cchTextMax = 1000;

	for (int i = 0; i < number_colum; i++)
	{
		lvc.pszText = (LPSTR)local_ru::header[i].c_str();
		index = ListView_InsertColumn(ListView, i, &lvc);
		if (index == -1) break;
	}

	return index;
}

void Functional::_init_menu()
{
	UINT enabled = (manip) ? MF_ENABLED : MF_DISABLED;
	Menu = CreatePopupMenu();
	HMENU AdditionalMenu = CreatePopupMenu();

	AppendMenu(Menu, MFT_STRING, ID_OPEN_ITEM, local_ru::MenuOpen);
	AppendMenu(Menu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(Menu, MFT_STRING | MF_POPUP, (UINT)AdditionalMenu, local_ru::MenuCreate);
	{
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_FOLDER, local_ru::MenuCreateFile);
		AppendMenu(AdditionalMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_TEXT_ITEM, local_ru::MenuCreateTxt);
	}
	AppendMenu(Menu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(Menu, MFT_STRING, ID_CUT_ITEM, local_ru::MenuCut);
	AppendMenu(Menu, MFT_STRING, ID_COPY_ITEM, local_ru::MenuCopy);
	AppendMenu(Menu, MFT_STRING | enabled, ID_PASTE_ITEM, local_ru::MenuPaste);
	AppendMenu(Menu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(Menu, MFT_STRING, ID_LINK_ITEM, local_ru::MenuLink);
	AppendMenu(Menu, MFT_STRING, ID_DELETE_ITEM, local_ru::MenuDelete);
	AppendMenu(Menu, MFT_STRING, ID_RENAME_ITEM, local_ru::MenuRename);

	AppendMenu(Menu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(Menu, MFT_STRING, ID_INFO_ITEM, local_ru::MenuInfo);
}

void Functional::_create_listview()
{
	const int colum_amount = 5;
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	RECT WindowRT;
	GetClientRect(hWnd, &WindowRT);

	ListView = CreateWindow(WC_LISTVIEW, NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
		WindowRT.left + 300, WindowRT.top + 110, WindowRT.right - 310, WindowRT.bottom - 120,
		hWnd, (HMENU)ID_LISTVIEW, GetModuleHandle(NULL), NULL);

	ListView_SetExtendedListViewStyleEx(ListView, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	set_listviw_colum();
	ShowWindow(ListView, SW_SHOWDEFAULT);
}

void Functional::_create_tree()
{
	InitCommonControls();

	Tree = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
		0, 0, 0, 0,
		hWnd,
		(HMENU)ID_TREEVIEW,
		hInst,
		NULL);

	ShowWindow(Tree, SW_SHOWDEFAULT);
}

void Functional::_crete_objects()
{
	Edit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_PATH_EDIT, hInst, NULL);
	ComboBox = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_DISKLIST_CB, NULL, NULL);

	Button = new HWND[3];
	Button[0] = CreateWindow("button", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		10, 10, 50, 20, hWnd, (HMENU)ID_BACK_BUTTON, hInst, NULL);
	Button[1] = CreateWindow("button", "->", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		60, 10, 50, 20, hWnd, (HMENU)ID_NEXT_BUTTON, hInst, NULL);
	Button[2] = CreateWindow("button", "Refresh", WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_BORDER,
		50, 50, 80, 25, hWnd, (HMENU)ID_REFRESH_BTN, NULL, NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1),
		IMAGE_BITMAP, 0, 0, 1);
	SendMessage(Button[2], BM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));
}

void Functional::disk_list()
{
	char arr[1024], *b;

	GetLogicalDriveStrings(sizeof(arr), arr);
	b = arr;

	while (*b != 0) {
		SendMessage(ComboBox, CB_ADDSTRING, NULL, (LPARAM)b);
		b += 4;
	}
	SendMessage(ComboBox, CB_SETCURSEL, NULL, (LPARAM)1);
	
}

bool Functional::_delete(const std::string &_delete)
{
	SmartFinder file;

	if (file.find(_delete)) {
		if (file.is_file())
			boost::filesystem::remove(_delete);
		else if (file.is_directory())
			boost::filesystem::remove_all(_delete);
		else {
			MessageBox(NULL, local_ru::ErrorDelete, local_ru::Error, MB_OK);
			return false;
		}
	}
	return true;
}

bool Functional::_add_lw_item(const std::string *_item)
{
	int iLastIndex = ListView_GetItemCount(ListView);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 1000;
	lvi.iItem = iLastIndex;
	lvi.pszText = (LPSTR)_item[0].c_str();
	lvi.iSubItem = 0;

	if (ListView_InsertItem(ListView, &lvi) == -1)
		return false;
	for (int i = 1; i < number_colum; i++)
		ListView_SetItemText(ListView, iLastIndex, i, (LPSTR)_item[i].c_str());

	return true;
}

bool Functional::open_proc()
{
	SHELLEXECUTEINFOA _shell;

	ZeroMemory(&_shell, sizeof(SHELLEXECUTEINFOA));
	_shell.cbSize = sizeof(SHELLEXECUTEINFOA);
	_shell.lpFile = path.selected_file.c_str();

	_shell.fMask = SEE_MASK_DEFAULT;

	if(ShellExecuteEx(&_shell))
		return true;

	return false;
}

std::string * Functional::make_file_info(const WIN32_FIND_DATA &file) const
{
	SYSTEMTIME sys_time;
	char *buffer;

	std::string *_info = new std::string[5];
	_info[0] = file.cFileName;

	FileTimeToSystemTime(&file.ftLastAccessTime, &sys_time);
	buffer = new char[256];

	sprintf(buffer,
		"%02d.%02d.%d %02d:%02d:%02d",
		sys_time.wDay,
		sys_time.wMonth,
		sys_time.wYear,
		sys_time.wHour,
		sys_time.wMinute,
		sys_time.wSecond);

	_info[1] = buffer;
	delete[] buffer;

	if (file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
		auto i = _info[0].rfind('.');
		for (++i; i < _info[0].size(); i++)
			_info[2] += _info[0][i];
		_info[3] = std::to_string((file.nFileSizeHigh * MAXDWORD) + file.nFileSizeLow) + " สม";

	}
	else {
		_info[2] = local_ru::Folder;
		_info[3] = " ";
	}

	FileTimeToSystemTime(&file.ftCreationTime, &sys_time);
	buffer = new char[256];

	sprintf(buffer,
		"%02d.%02d.%d %02d:%02d:%02d",
		sys_time.wDay,
		sys_time.wMonth,
		sys_time.wYear,
		sys_time.wHour,
		sys_time.wMinute,
		sys_time.wSecond);

	_info[4] = buffer;
	delete[] buffer;

	return _info;
}

Functional::Functional(HWND _hWnd) :
	path("C:\\", path.main_path), hWnd(_hWnd), number_colum(5)
{
	_create_listview();
	_create_tree();
	_crete_objects();
	disk_list();
	update_listview();
}

bool Functional::make_paste()
{
	if (!manip)
		return false;

	int i = 1;
	SmartFinder file;

	if (file.find(manip.file)) {
		do {
			if (file.is_file()) {
				int index = manip.file.rfind('.');
				manip.file.insert(index, " (" + std::to_string(i++) + ")");
			}
			else if (file.is_directory())
				manip.file = manip.file + " (" + std::to_string(i++) + ")";
		} while (file.next());
	}

	try {
		if (file.is_file())
			boost::filesystem::copy_file(manip.file, path.main_path, boost::filesystem::copy_option::fail_if_exists);
		else if (file.is_directory())
			boost::filesystem::copy_directory(manip.file, path.main_path);
	}
	catch (...) {
		MessageBox(NULL, local_ru::ErrorCopy, local_ru::Error, MB_OK);
		return false;
	}

	if (manip.aDelete) {
		if (!_delete(manip.file))
			return false;
	}

	return true;
}

Functional::Path::operator bool()
{
	return (selected_file != main_path && selected_file.size());
}

Functional::Path::Path(const std::string &_main, const std::string &_next) :
	main_path(_main), next_path(_next)
{
}

void Functional::update_listview()
{
	std::string buffer;
	SmartFinder file;

	SetWindowText(Edit, path.main_path.c_str());
	ListView_DeleteAllItems(ListView);
	buffer = path.main_path + "*.*";

	if (file.find(buffer)) {
		do {
			if (file.hidden()) {
				_add_lw_item(make_file_info(file._get()));
			}
		} while (file.next());
	}
}

Functional::~Functional()
{
	DestroyMenu(Menu);
	DestroyWindow(Edit);
	DestroyWindow(Button[0]);
	DestroyWindow(Button[1]);
	DestroyWindow(Tree);
	DestroyWindow(ListView);
	DestroyWindow(ComboBox);
	DestroyWindow(hWnd);

	delete[] Button;
}

Functional::Manip::Manip()
{
}

Functional::Manip::Manip(const std::string &_buffer, const std::string &_path, bool _cut) :
	file(_buffer), aDelete(_cut), path(_path)
{
}

Functional::Manip::operator bool()
{
	return file.size();
}

void Functional::Manip::clear()
{
	file.clear();
	aDelete = false;
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
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
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
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
