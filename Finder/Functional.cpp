#include "Functional.h"
#include <shlwapi.h>

void Functional::_init_main_menu()
{
	Main_Menu = CreateMenu();
	HMENU Menu = CreatePopupMenu();
	HMENU File_add = CreatePopupMenu();

	SmartStringLoad str;
	AppendMenu(Main_Menu, MFT_STRING | MF_POPUP, (UINT)File_add, str._get(Menu_file));
	{	
		AppendMenu(File_add, MFT_STRING, ID_PROGRAM_CLOSE, str._get(Menu_file_exit));
	}
	AppendMenu(Main_Menu, MFT_STRING | MF_POPUP, (UINT)Menu, str._get(DialogAboutName));
	{
		AppendMenu(Menu, MFT_STRING | MF_POPUP, ID_PROGRAM_ABOUT, str._get(DialogAboutName));
	}
	SetMenu(hWnd, Main_Menu);
}

void Functional::_init_cmenu()
{
	UINT enabled = (manip) ? MF_ENABLED : MF_DISABLED;
	CMenu = CreatePopupMenu();
	HMENU AdditionalMenu = CreatePopupMenu();
	SmartStringLoad str;

	AppendMenu(CMenu, MFT_STRING, ID_OPEN_ITEM, str._get(MenuOpen));
	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING | MF_POPUP, (UINT)AdditionalMenu, str._get(MenuCreate));
	{
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_FOLDER, str._get(MenuCreateFile));
		AppendMenu(AdditionalMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_TEXT_ITEM, str._get(MenuCreateTxt));
	}
	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_CUT_ITEM, str._get(MenuCut));
	AppendMenu(CMenu, MFT_STRING, ID_COPY_ITEM, str._get(MenuCopy));
	AppendMenu(CMenu, MFT_STRING | enabled, ID_PASTE_ITEM, str._get(MenuPaste));

	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_LINK_ITEM, str._get(MenuLink));
	AppendMenu(CMenu, MFT_STRING, ID_DELETE_ITEM, str._get(MenuDelete));
	AppendMenu(CMenu, MFT_STRING, ID_RENAME_ITEM, str._get(MenuRename));

	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_INFO_ITEM, str._get(MenuInfo));
}

void Functional::_init_tree()
{
	SmartStringLoad str;
	TV_INSERTSTRUCT _insert;
	_insert.item.mask = TVIF_TEXT | TVIF_PARAM;
	_insert.hParent = NULL;
	_insert.hInsertAfter = TVI_LAST;
	_insert.item.pszText = (char*)str._get(MyComputer_info);

	HTREEITEM MyComputer = TreeView_InsertItem(Tree, &_insert);
	HTREEITEM Disk;
	char *temp; 

	for (int i = 0; i < disks->disk_amount; i++) {
		temp = new char[64];
		_insert.hParent = MyComputer;
		_insert.item.pszText = disks->_get_disk(i);
		Disk = TreeView_InsertItem(Tree, &_insert);

		_getPath(Disk, temp);
		tree_load(Disk, temp);
		delete[] temp;
	}

	TreeView_Expand(Tree, MyComputer, TVE_EXPAND);
	TreeView_SelectItem(Tree, MyComputer);
}

void Functional::disk_list()
{
	char *all_disks =  new char[512], *disk;
	int amount = GetLogicalDriveStrings(511, all_disks);

	disk = all_disks;
	disks = new Disk(amount / 4);

	while (*disk != 0) {
		disks->add_disk(disk);
		SendMessage(ComboBox, CB_ADDSTRING, NULL, (LPARAM)disk);
		disk += 4;
	}
	SendMessage(ComboBox, CB_SETCURSEL, NULL, (LPARAM)1);
	delete[] all_disks;
}

bool Functional::_delete(const std::string &_delete)
{
	SmartFinder file;

	if (file.find(_delete)) {
		try {
			if (file.is_file()) {
				boost::filesystem::remove(_delete);
			}
			else {
				boost::filesystem::remove_all(_delete);
			}
		}
		catch (...) {
			SmartStringLoad str, str_1;
			MessageBox(NULL, str._get(ErrorDelete, 64), str_1._get(Error_info), MB_OK);
			return false;
		}
	}
	return true;
}

bool Functional::_add_lw_item(WIN32_FIND_DATA _file)
{
	info = new FileInfo(_file);

	int iLastIndex = ListView_GetItemCount(ListView);
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 1000;
	lvi.iItem = iLastIndex;
	lvi.pszText = (LPSTR)info->_get_info(0);
	lvi.iSubItem = 0;
	ListView_InsertItem(ListView, &lvi);

	for (int i = 1; i < number_colum; i++) {
		ListView_SetItemText(ListView, iLastIndex, i, (char*)info->_get_info(i));
	}
	delete info;
	return true;
}

bool Functional::open_proc()
{
	SHELLEXECUTEINFO sei;

	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = "open";
	sei.hwnd = hWnd;
	sei.nShow = SW_SHOWNORMAL;
	sei.lpFile = path->selected_file.c_str();
	sei.lpParameters = NULL;
	sei.lpDirectory = NULL;

	if (ShellExecuteEx(&sei))
		return true;
}

std::string Functional::_getPath(HTREEITEM _item, char* temp)
{
	TV_ITEM tv;

	tv.mask = TVIF_TEXT | TVIF_HANDLE;
	tv.hItem = _item;
	tv.pszText = temp;
	tv.cchTextMax = 63;
	TreeView_GetItem(Tree, &tv);

	return (char*)tv.pszText;
}

std::string Functional::_get_full_path(HTREEITEM _selected)
{
	char *temp;
	std::string temp_path;
	HTREEITEM root = TreeView_GetRoot(Tree);

	while (_selected != root && _selected) {
		temp = new char[64];
		temp_path.insert(0, (_getPath(_selected, temp) + '\\'));
		_selected = TreeView_GetParent(Tree, _selected);
		delete[] temp;
	}
	return temp_path;
}

std::string Functional::same_name(const std::string &_name)
{
	int i = 1;
	SmartFinder file;
	std::string temp = _name;

	while (file.find(temp)) {
		if (file.is_file()) {
			temp = _name;
			temp.insert(temp.rfind('.'), " (" + std::to_string(i++) + ")");
		}
		else {
			temp = _name;
			temp += " (" + std::to_string(i++) + ")";
		}
	}

	return temp;
}

Functional::Functional(HWND _hWnd) :
	Objects(_hWnd,2)
{
	disk_list();
	path = new Path(disks->_get_disk(0));
	update_listview();
	_init_tree();
	_init_main_menu();
}

bool Functional::try_paste()
{
	if (!manip)
		return false;

	std::string temp = manip->file;
	if (manip->path == path->main_path) {
		temp = same_name(manip->file);
	}

	try {
		SmartFinder check_file;
		temp = path->main_path + temp.substr(temp.rfind('\\', temp.size() - 1));
		if (check_file.find(temp)) {
			throw "File exist!";
		}
		check_file.find(manip->file);
		if (check_file.is_file()) {
			boost::filesystem::copy_file(manip->file, temp);
		}
		else {
			boost::filesystem::copy_directory(manip->file, temp);
		}
	}
	catch (...) {
		SmartStringLoad str, str_1;
		MessageBox(NULL, str._get(ErrorPaste), str_1._get(Error_info), MB_OK);
		return false;
	}

	if (manip->aDelete) {
		if (!_delete(manip->file))
			return false;
	}

	return true;
}

void Functional::update_listview()
{
	std::string buffer;
	SmartFinder file;

	SetWindowText(Edit, path->main_path.c_str());
	ListView_DeleteAllItems(ListView);

	if (file.find(path->main_path + "*")) {
		do {
			if (file.hidden()) {
				_add_lw_item(file._get());
			}
		} while (file.next());
	}

	path->item_amount = ListView_GetItemCount(ListView);
}

void Functional::tree_load(HTREEITEM _item, const std::string &_path)
{
	SmartFinder file;

	if (!file.find(_path + '*'))
		return;

	TV_INSERTSTRUCT _insert;
	_insert.hParent = _item;
	_insert.item.mask = TVIF_TEXT;

	do {
		if (!file.is_file() && file.hidden()) {
			_insert.item.pszText = file._get().cFileName;
			TreeView_InsertItem(Tree, &_insert);
		}
	} while (file.next());
}

Functional::~Functional()
{
	delete path;
	delete disks;
	if (manip)
		delete manip;
}
