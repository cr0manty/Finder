#include "Finder.h"
#include <fstream>
#include <ShlObj.h>

Finder::Finder(HWND _hWnd) :
	Functional(_hWnd), current_lang(Menu_change_lang_ru)
{
}

Path Finder::_get_path() const
{
	return path;
}

void Finder::create_txt()
{
	SmartFinder find;
	SmartStringLoad str(DefaultTextFile);

	try {
		std::ofstream file(same_name(path.main_path + str._get_1() + ".txt"));
		file.close();
	}

	catch (...) {
		return;
	}

	update_listview();
}

void Finder::create_folder()
{
	SmartFinder file;
	SmartStringLoad str(DefaultFolder);

	try {
		boost::filesystem::create_directory(same_name(path.main_path + str._get_1()));
	}
	catch (...) {
		return;
	}

	update_listview();
}

void Finder::rename()
{
	start_rename(path.selected_index);
}

void Finder::open()
{
	if (!path)
		return;
	SmartFinder file;
	if (file.find(path.selected_file)) {
		if (file.is_file()) {
			open_proc();
		}
		else if (file.is_directory()) {
			path.main_path = path.selected_file + '\\';
			update_listview();
		}
	}
}

void Finder::create_link()
{
	std::string type = " (1).lnk";
	IShellLink  *psl;
	int amount = 2;
	HRESULT hres;
	SmartFinder file;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL,
		CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (SUCCEEDED(hres))
	{
		int i;
		IPersistFile  *ppf;
		psl->SetPath(path.selected_file.c_str());
		i = path.selected_file.rfind('.');

		if (i > path.selected_file.size() - 5 && i != std::string::npos)
			path.selected_file.erase(i);
			
		while (file.find(path.selected_file + type)) {
				type = " (" + std::to_string(amount++) + ").lnk";
			}

		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (SUCCEEDED(hres)) {
			wchar_t  wsz[MAX_PATH];
			MultiByteToWideChar(CP_ACP, 0, (path.selected_file + type).c_str(), -1, wsz, MAX_PATH);
			hres = ppf->Save(wsz, true);
			ppf->Release();
		}
		psl->Release();
		CoUninitialize();
		update_listview();
	}
}

void Finder::show_back()
{
	if (path.main_path.size() <= 4)
		return;

	int index = path.main_path.size() - 3;
	while (path.main_path[index--] != '\\');
	path.next_path = path.main_path;
	path.main_path.erase(index + 1);
	path.main_path += '\\';
	update_listview();
}

void Finder::show_next()
{
	if (path.next_path != path.main_path) {
		path.main_path = path.next_path;
		update_listview();
	}
}

void Finder::select_item()
{
	char *temp = new char[200];
	temp[0] = 0;
	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 200);

	if (index == -1)
		goto end;

	path.selected_index = index;
	path.selected_file = path.main_path + temp;
end:
	delete[] temp;
}

void Finder::context_menu(LPARAM lParam)
{
	select_item();
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	if (!mouse_cmenu(point))
		return;

	char *temp = new char[200];
	_init_cmenu();
	RECT rt;
	GetClientRect(ListView, &rt);
	TrackPopupMenu(CMenu, TPM_RIGHTBUTTON |
		TPM_TOPALIGN |
		TPM_LEFTALIGN,
		LOWORD(lParam), HIWORD(lParam), 0, hWnd, &LVrt);

	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 200);

	path.selected_file = path.main_path + temp;
	delete[] temp;

	DestroyMenu(CMenu);
}

void Finder::file_manip(bool _cut)
{
	if (path) {
		manip = Manip(path.selected_file, path.main_path, _cut);
	}
}

void Finder::fix_size(LPARAM lParam)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 200;
}

void Finder::disk_change(WPARAM wParam)
{
	if (HIWORD(wParam) != CBN_SELENDOK)
		return;

	char *temp = new char[6];
	GetDlgItemText(hWnd, ID_DISKLIST_CB, temp, 5);
	path.main_path = temp;
	update_listview();

	delete[] temp;
}

void Finder::delete_item()
{
	if (!path)
		return;
	SmartStringLoad str(DeleteFileInfo, DeleteFileHeader, 64);

	if (MessageBox(NULL, str._get_1(),
		str._get_2(), MB_ICONQUESTION | MB_YESNO) == IDYES) {
		_delete(path.selected_file);
		update_listview();
	}
}

void Finder::make_paste()
{
	if (try_paste()) {
		update_listview();
		manip.clear();
	}
}

void Finder::show_info()
{
	DialogBoxParam(hInst, MAKEINTRESOURCE(ID_DLG_INFO), hWnd, (DLGPROC)DlgInfo, (LPARAM)this);
}

void Finder::make_refresh()
{
	update_listview();
}

void Finder::show_about()
{
	DialogBox(hInst, MAKEINTRESOURCE(ID_ABOUT), hWnd, (DLGPROC)DlgAbout);
}

void Finder::tree_to_list()
{
	if (!this)
		return;
	
	HTREEITEM _selected = TreeView_GetSelection(Tree); 
	TreeView_Expand(Tree, _selected, TVE_EXPAND);

	std::string temp = _get_full_path(_selected);
	SmartFinder file(temp);
	path.selected_file = temp;
	
	if (!file.is_file()) {
		if (!TreeView_GetChild(Tree, _selected))
			temp += '\\';
		path.main_path = temp;
		update_listview();
	}
	else
		open_proc();
}

void Finder::tree_show(LPARAM lParam)
{
	if (!this)
		return;

	HTREEITEM myComputer = TreeView_GetRoot(Tree);
	NMHDR* notifyMess = (NMHDR*)lParam;
	LPNMTREEVIEW _tree = (LPNMTREEVIEW)notifyMess; 
	HTREEITEM _selected = _tree->itemNew.hItem;

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

void Finder::minimize_window()
{
	ShowWindowAsync(hWnd, SW_MINIMIZE);
}

void Finder::exit()
{
	PostQuitMessage(0);
}

void Finder::change_lang(int _lang)
{
	EnableMenuItem(Main_Menu, current_lang, MF_ENABLED);
	current_lang = _lang;
	EnableMenuItem(Main_Menu, _lang, MF_DISABLED);
}
