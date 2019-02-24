#include "includes.h"

bool Objects::mouse_cmenu(const POINT &_point)
{
	return _point.x < LVrt.right && _point.x > LVrt.left &&
		_point.y < LVrt.bottom && _point.y > LVrt.top;
}

Objects::Objects(HWND _hWnd, int _size) :
	hWnd(_hWnd), number_colum(5), buttons_amount(_size), hInst(GetModuleHandle(NULL))
{
	Button = new HWND[buttons_amount];
	_create_listview();
	_create_tree();
	_crete_objects();
	_create_hotkey();
	resize();
}

bool Objects::resize()
{
	if (!this)
		return false;

	HWND temp = NULL;
	GetClientRect(hWnd, &WindowRT);
	LVrt = { WindowRT.left + 300, WindowRT.top + 25, WindowRT.right, WindowRT.bottom };

	SetWindowPos(ListView, temp, LVrt.left, LVrt.top, LVrt.right, LVrt.bottom, NULL);
	SetWindowPos(Tree, temp, WindowRT.left, WindowRT.top + 25, WindowRT.left + 300, WindowRT.bottom, NULL);
	SetWindowPos(Edit, temp, WindowRT.left + 300, WindowRT.top, WindowRT.right - 355, 25, NULL);
	SetWindowPos(ComboBox, temp, WindowRT.right - 55, WindowRT.top, 55, 200, NULL);
	return true;
}

void Objects::_create_listview()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);
	RECT WindowRT;
	GetClientRect(hWnd, &WindowRT);

	ListView = CreateWindow(WC_LISTVIEW,
		NULL,
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | 
		LVS_EDITLABELS | LVS_SINGLESEL,
		WindowRT.left + 300, WindowRT.top + 110, WindowRT.right - 290, WindowRT.bottom - 120,
		hWnd, 
		(HMENU)ID_LISTVIEW, 
		hInst,
		NULL);

	ListView_SetExtendedListViewStyleEx(ListView, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	_set_listviw_colum();
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
	Edit = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_PATH_EDIT, hInst, NULL);
	ComboBox = CreateWindow("Combobox", NULL, WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | CB_SHOWDROPDOWN | WS_BORDER,
		0, 0, 0, 0, hWnd, (HMENU)ID_DISKLIST_CB, hInst, NULL);

	Button[0] = CreateWindow("Button", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
		0, 0, 50, 25, hWnd, (HMENU)ID_BACK_BUTTON, hInst, NULL);
	Button[1] = CreateWindow("Button", "->", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
		50, 0, 50, 25, hWnd, (HMENU)ID_NEXT_BUTTON, hInst, NULL);
}

void Objects::_create_hotkey()
{
	RegisterHotKey(hWnd, ID_COPY_HK, MOD_CONTROL, 0x43);
	RegisterHotKey(hWnd, ID_CUT_HK, MOD_CONTROL, 0x58);
	RegisterHotKey(hWnd, ID_PASTE_HK, MOD_CONTROL, 0x56);
	RegisterHotKey(hWnd, ID_REFRESH_HK, MOD_CONTROL, 0x52);
	RegisterHotKey(hWnd, ID_MINIM_HK, MOD_CONTROL, 0x57);
	RegisterHotKey(hWnd, ID_CREATE_FOLDER, MOD_CONTROL, 0x4E);
	RegisterHotKey(hWnd, ID_CREATE_TEXT_ITEM, MOD_CONTROL, 0x46);

	RegisterHotKey(hWnd, ID_BACK_HK, MOD_ALT, VK_LEFT);
	RegisterHotKey(hWnd, ID_NEXT_HK, MOD_ALT, VK_RIGHT);
}

void Objects::_set_listviw_colum()
{
	SmartStringLoad str;
	RECT rt;
	GetClientRect(ListView, &rt);
	int index = -1;

	LVCOLUMN lv;
	lv.mask = LVCF_TEXT | LVCF_WIDTH;
	lv.cx = (rt.right - rt.left) / number_colum; 
	lv.cchTextMax = 256;

	for (int i = 0; i < number_colum; i++) {
		lv.pszText = (LPSTR)str._get(Table_name + i);
		index = ListView_InsertColumn(ListView, i, &lv);
		if (index == -1) 
			break;
	}
}

Objects::~Objects()
{
	DestroyMenu(CMenu);
	DestroyMenu(Main_Menu);

	delete[] Button;
}
