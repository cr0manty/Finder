#include "Finder.h"
#include <fstream>
#include <ShlObj.h>

Finder::Finder(HWND _hWnd) :
	Functional(_hWnd)
{
}

void Finder::create_txt()
{
	SmartStringLoad str;
	std::string temp = same_name(path->main_path + str._get(DefaultTextFile) + ".txt");
	try {
		std::ofstream file(temp);
		file.close();
	}
	catch (...) {
		return;
	}
	path->selected_file = temp;
	update_listview();
	path->selected_index = path->item_amount - 1;
	start_rename();
}

void Finder::create_folder()
{
	SmartStringLoad str;
	std::string temp = same_name(path->main_path + str._get(DefaultFolder));

	try {
		boost::filesystem::create_directory(temp);
	}
	catch (...) {
		return;
	}

	path->selected_file = temp;
	update_listview();
	path->selected_index = path->item_amount - 1;
	start_rename();
}

void Finder::start_rename()
{
	temp_edit = ListView_EditLabel(ListView, path->selected_index);
}

void Finder::end_rename()
{
	if (!temp_edit)
		return;

	char * temp = new char[256];
	if (!GetWindowText(temp_edit, temp, 255)) {
		SmartStringLoad str, str_1;
		MessageBox(NULL, str._get(ErrorEmptyName), str_1._get(Error_info), MB_OK);
		goto end;
	}
	try {
		boost::filesystem::rename(path->selected_file, path->main_path + temp);
	}
	catch (...) {
		goto end;
	}
	update_listview();
end:
	delete[] temp;
}

void Finder::open()
{
	if (!path)
		return;

	SmartFinder file;

	if (file.find(path->selected_file)) {
		if (file.is_file()) {
			open_proc();
		}
		else {
			path->main_path = path->selected_file + '\\';
			update_listview();
		}
	}
}

void Finder::create_link()
{
	std::string type = " (1).lnk";
	IShellLink  *psl;
	HRESULT hres;
	SmartFinder file;
	CoInitialize(NULL);

	hres = CoCreateInstance(CLSID_ShellLink, NULL,
		CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(hres))
	{
		int amount = 2, i;
		IPersistFile  *ppf;
		psl->SetPath(path->selected_file.c_str());
		i = path->selected_file.rfind('.');

		if (i > path->selected_file.size() - 5 && i != std::string::npos) {
			path->selected_file.erase(i);
		}

		while (file.find(path->selected_file + type)) {
				type = " (" + std::to_string(amount++) + ").lnk";
		}

		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(hres)) {
			wchar_t  *wsz = new wchar_t[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, (path->selected_file + type).c_str(), -1, wsz, MAX_PATH);
			hres = ppf->Save(wsz, true);
			ppf->Release();
			delete[] wsz;
		}
		psl->Release();
		CoUninitialize();
		update_listview();
	}
}

void Finder::show_back()
{
	if (path->main_path.size() <= 4)
		return;

	int index = path->main_path.size() - 3;
	while (path->main_path[index--] != '\\');
	path->next_path = path->main_path;
	path->main_path.erase(index + 1);
	path->main_path += '\\';
	update_listview();
}

void Finder::show_next()
{
	if (path->next_path != path->main_path) {
		path->main_path = path->next_path;
		update_listview();
	}
}

void Finder::select_item()
{
	char *temp = new char[128];
	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 127);

	if (index == -1)
		goto end;

	path->selected_index = index;
	path->selected_file = path->main_path + temp;
end:
	delete[] temp;
}

bool Finder::context_menu(__int64 _cmd)
{
	select_item();
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	if (!mouse_cmenu(point))
		return false;

	char *temp = new char[128];
	_init_cmenu();
	TrackPopupMenu(CMenu, TPM_RIGHTBUTTON |
		TPM_TOPALIGN |
		TPM_LEFTALIGN,
		LOWORD(_cmd), HIWORD(_cmd), 0, hWnd, NULL);

	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 127);

	path->selected_file = path->main_path + temp;
	delete[] temp;

	DestroyMenu(CMenu);
	return true;
}

void Finder::file_manip(bool _cut)
{
	if (path) {
		if (manip)
			delete manip;
		manip = new Manip(path->selected_file, path->main_path, _cut);
	}
}

bool Finder::fix_size(__int64 _cmd)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)_cmd;
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 200;

	return true;
}

void Finder::disk_change(__int64 _cmd)
{
	if (HIWORD(_cmd) != CBN_SELENDOK)
		return;

	char *temp = new char[6];
	GetDlgItemText(hWnd, ID_DISKLIST_CB, temp, 5);
	path->main_path = temp;
	delete[] temp;

	update_listview();
}

void Finder::delete_item()
{
	if (!path)
		return;
	SmartStringLoad str, str_1;

	if (MessageBox(NULL, str._get(DeleteFileInfo, 64),
		str_1._get(DeleteFileHeader), MB_ICONQUESTION | MB_YESNO) == IDYES) {
		if (_delete(path->selected_file)) {
			update_listview();
		}
	}
}

void Finder::make_paste()
{
	if (try_paste()) {
		update_listview();
		manip->clear();
	}
}

void Finder::show_info()
{
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_DLG_INFO), hWnd, (DLGPROC)DlgInfo, (LPARAM)this);
}

void Finder::make_refresh()
{
	update_listview();
}

void Finder::show_about()
{
	DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(ID_ABOUT), hWnd, (DLGPROC)DlgAbout);
}

void Finder::tree_to_list()
{
	if (!this)
		return;
	
	HTREEITEM _selected = TreeView_GetSelection(Tree); 
	TreeView_Expand(Tree, _selected, TVE_EXPAND);
	path->selected_file = _get_full_path(_selected);

	SmartStringLoad str;
	if (path->selected_file == str._get(MyComputer_info))
		return;

	SmartFinder file(path->selected_file);
	if (!file.is_file()) {
		if (!TreeView_GetChild(Tree, _selected)) {
			path->selected_file += '\\';
		}
		path->main_path = path->selected_file;
		update_listview();
	}
	else {
		open_proc();
	}
}

void Finder::tree_show(__int64 _cmd)
{
	if (!this)
		return;

	HTREEITEM myComputer = TreeView_GetRoot(Tree);
	HTREEITEM _selected = LPNMTREEVIEW((NMHDR*)_cmd)->itemNew.hItem;

	if (_selected == myComputer) 
		return;

	HTREEITEM _selected_child = TreeView_GetChild(Tree, _selected); 
	if (_selected_child) {
		do {
			if(!TreeView_GetChild(Tree, _selected_child)) {
				tree_load(_selected_child, _get_full_path(_selected_child) + "\\*");
			}
		} while (_selected_child = TreeView_GetNextSibling(Tree, _selected_child));
	}
}

bool Finder::command_switch(__int64 _cmd)
{
	switch (_cmd)
	{
	case ID_COPY_ITEM:
		file_manip(Copy);
		return true;

	case ID_CUT_ITEM:
		file_manip(Paste);
		return true;

	case ID_OPEN_ITEM:
		open();
		return true;

	case ID_LINK_ITEM:
		create_link();
		return true;

	case ID_PASTE_ITEM:
		make_paste();
		return true;

	case ID_RENAME_ITEM:
		start_rename();
		return true;

	case ID_CREATE_FOLDER:
		create_folder();
		return true;

	case ID_CREATE_TEXT_ITEM:
		create_txt();
		return true;

	case ID_INFO_ITEM:
		show_info();
		return true;

	case ID_DELETE_ITEM:
		delete_item();
		return true;

	case ID_BACK_BUTTON:
		show_back();
		return true;

	case ID_NEXT_BUTTON:
		show_next();
		return true;

	case ID_DISKLIST_CB:
		disk_change(_cmd);
		return true;

	case ID_PROGRAM_ABOUT:
		show_about();
		return true;

	case ID_PROGRAM_CLOSE:
		PostQuitMessage(0);
		return true;
	}
	return false;
}

bool Finder::hotkey_switch(__int64 _cmd)
{
	switch (_cmd)
	{
	case ID_REFRESH_HK:
		make_refresh();
		return true;

	case ID_COPY_HK:
		file_manip(Copy);
		return true;

	case ID_CUT_HK:
		file_manip(Paste);
		return true;

	case ID_PASTE_HK:
		make_paste();
		return true;

	case ID_MINIM_HK:
		minimize_window();
		return true;

	case ID_BACK_HK:
		show_back();
		return true;

	case ID_NEXT_HK:
		show_next();
		return true;

	case ID_CREATE_FOLDER:
		create_folder();
		return true;

	case ID_CREATE_TEXT_ITEM:
		create_txt();
		return true;
	}
	return false;
}

bool Finder::notify_switch(__int64 _cmd)
{
	switch (LPNMHDR(_cmd)->code)
	{
	case NM_DBLCLK:
		open();
		return true;

	case NM_CLICK:
		select_item();
		return true;

	case LVN_ENDLABELEDIT:
		end_rename();
		return true;

	case TVN_SELCHANGED:
		tree_to_list();
		return true;

	case TVN_ITEMEXPANDED:
		tree_show(_cmd);
		return true;
	}
	return false;
}

bool Finder::key_down_switch(__int64 _cmd)
{
	switch (_cmd)
	{
	case VK_DELETE:
		delete_item();
		return true;

	case VK_F5:
		make_refresh();
		return true;
	}
	return false;
}

bool Finder::dlgInfo_proc(HWND _hDlg, __int64 wParam)
{
	HWND *Object, *Object_info;
	SmartFinder file;
	SmartStringLoad str;
	SetWindowText(_hDlg, str._get(DialogAboutName));

	if (file.find(path->selected_file) && path->selected_file.size() > 3) {
		info = new FileInfo(file._get());
	}
	else {
		EndDialog(_hDlg, LOWORD(wParam));
		return false;
	}
	Object = new HWND[6];
	Object_info = new HWND[6];

	for (int i = 0; i < 5; i++) {
		Object[i] = GetDlgItem(_hDlg, ID_NOBJECT_1 + i);
		Object_info[i] = GetDlgItem(_hDlg, ID_OBJECT_1 + i);
		SendMessage(Object[i], WM_SETTEXT, (WPARAM)255, (LPARAM)info->_get_info(i));
		SendMessage(Object_info[i], WM_SETTEXT, (WPARAM)255, (LPARAM)info->_get_header(i));
	}
	Object[5] = GetDlgItem(_hDlg, ID_NOBJECT_6);
	Object_info[5] = GetDlgItem(_hDlg, ID_OBJECT_6);
	SendMessage(Object[5], WM_SETTEXT, (WPARAM)255, (LPARAM)path->main_path.c_str());
	SendMessage(Object_info[5], WM_SETTEXT, (WPARAM)255, (LPARAM)info->_get_header(5));

	delete info;
	delete[] Object;
	delete[] Object_info;
	return false;
}

void Finder::minimize_window()
{
	ShowWindowAsync(hWnd, SW_MINIMIZE);
}

Finder::~Finder()
{
	PostQuitMessage(0);
}
