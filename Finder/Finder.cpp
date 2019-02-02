#include "Finder.h"
#include <fstream>
#include <ShlObj.h>

Finder::Finder(HWND _hWnd) :
	Functional(_hWnd)
{
}

Path & Finder::_get_path() const
{
	return *path;
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
		goto error;
	}

	try {
		boost::filesystem::rename(path->selected_file, path->main_path + temp);
	}
	catch (...) {
		return;
	}
	update_listview();
error:
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

void Finder::context_menu(LPARAM lParam)
{
	select_item();
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hWnd, &point);

	if (!mouse_cmenu(point))
		return;

	char *temp = new char[128];
	_init_cmenu();
	TrackPopupMenu(CMenu, TPM_RIGHTBUTTON |
		TPM_TOPALIGN |
		TPM_LEFTALIGN,
		LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);

	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 127);

	path->selected_file = path->main_path + temp;
	delete[] temp;

	DestroyMenu(CMenu);
}

void Finder::file_manip(bool _cut)
{
	if (path) {
		if (manip)
			delete manip;

		manip = new Manip(path->selected_file, path->main_path, _cut);
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

Finder::~Finder()
{
}
