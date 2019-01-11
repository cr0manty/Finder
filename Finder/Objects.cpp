#include "Objects.h"

Objects::Objects(HWND _hWnd, int _size) :
	hWnd(_hWnd), number_colum(5), buttons_amount(_size)
{
	Button = new HWND[buttons_amount];

	_create_listview();
	_create_tree();
	_crete_objects();
}

void Objects::resize() const
{
	RECT WindowRT;
	HWND temp = NULL;

	GetClientRect(hWnd, &WindowRT);
	SetWindowPos(ListView, temp, WindowRT.left + 300, WindowRT.top + 25, WindowRT.right - 300, WindowRT.bottom, NULL);
	SetWindowPos(Tree, temp, WindowRT.left, WindowRT.top + 25, WindowRT.left + 300, WindowRT.bottom, NULL);
	SetWindowPos(Edit, temp, WindowRT.left + 300, WindowRT.top, WindowRT.right - 380, 25, NULL);
	SetWindowPos(Button[2], temp, WindowRT.right - 80, WindowRT.top, 25, 25, NULL);
	SetWindowPos(ComboBox, temp, WindowRT.right - 55, WindowRT.top, 55, 200, NULL);
}

Objects::~Objects()
{
	DestroyMenu(Menu);
	DestroyWindow(Edit);
	DestroyWindow(Tree);
	DestroyWindow(ListView);
	DestroyWindow(ComboBox);
	DestroyWindow(hWnd);

	for (int i = 0; i < buttons_amount; i++)
		DestroyWindow(Button[i]);
	delete[] Button;
}

void Objects::_create_listview()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	RECT WindowRT;
	GetClientRect(hWnd, &WindowRT);

	ListView = CreateWindow(WC_LISTVIEW, NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS,
		WindowRT.left + 300, WindowRT.top + 110, WindowRT.right - 290, WindowRT.bottom - 120,
		hWnd, (HMENU)ID_LISTVIEW, GetModuleHandle(NULL), NULL);

	ListView_SetExtendedListViewStyleEx(ListView, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	set_listviw_colum();
	ShowWindow(ListView, SW_SHOWDEFAULT);
}

void Objects::_create_tree()
{
	InitCommonControls();

	Tree = CreateWindowEx(0,
		WC_TREEVIEW,
		TEXT("Tree View"),
		WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | WS_VSCROLL | WS_TABSTOP |
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		0, 0, 0, 0,
		hWnd,
		(HMENU)ID_TREEVIEW,
		hInst,
		NULL);

	ShowWindow(Tree, SW_SHOWDEFAULT);
}

void Objects::_crete_objects()
{
	Edit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_PATH_EDIT, hInst, NULL);
	ComboBox = CreateWindow("combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_DISKLIST_CB, NULL, NULL);

	Button[0] = CreateWindow("button", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
		0, 0, 50, 25, hWnd, (HMENU)ID_BACK_BUTTON, hInst, NULL);
	Button[1] = CreateWindow("button", "->", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
		50, 0, 50, 25, hWnd, (HMENU)ID_NEXT_BUTTON, hInst, NULL);
	Button[2] = CreateWindow("button", "Refresh", WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_REFRESH_BTN, NULL, NULL);
	/*HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1),
		IMAGE_BITMAP, 0, 0, 1);
	SendMessage(Button[2], BM_SETIMAGE, IMAGE_BITMAP, LPARAM(hBitmap));*/
}

void Objects::set_listviw_colum()
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
}
