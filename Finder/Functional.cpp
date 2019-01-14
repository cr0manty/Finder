#include "Functional.h"
#include <shlwapi.h>

void Functional::_init_main_menu()
{
	Main_Menu = CreateMenu();
	HMENU Menu = CreatePopupMenu();
	HMENU File_add = CreatePopupMenu();

	SmartStringLoad str;
	AppendMenu(Main_Menu, MFT_STRING | MF_POPUP, (UINT)File_add, str._set_and_get(Menu_file));
	{
		AppendMenu(File_add, MFT_STRING, ID_PROGRAM_CLOSE, str._set_and_get(Menu_file_exit));
	}
	AppendMenu(Main_Menu, MFT_STRING | MF_POPUP, (UINT)Menu, str._set_and_get(DialogAboutName));
	{
		AppendMenu(Menu, MFT_STRING | MF_POPUP, ID_PROGRAM_ABOUT, str._set_and_get(DialogAboutName));
	}
	SetMenu(hWnd, Main_Menu);
}

void Functional::_init_cmenu()
{
	UINT enabled = (manip) ? MF_ENABLED : MF_DISABLED;
	CMenu = CreatePopupMenu();
	HMENU AdditionalMenu = CreatePopupMenu();
	SmartStringLoad str;

	AppendMenu(CMenu, MFT_STRING, ID_OPEN_ITEM, str._set_and_get(MenuOpen));
	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING | MF_POPUP, (UINT)AdditionalMenu, str._set_and_get(MenuCreate));
	{
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_FOLDER, str._set_and_get(MenuCreateFile));
		AppendMenu(AdditionalMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(AdditionalMenu, MFT_STRING, ID_CREATE_TEXT_ITEM, str._set_and_get(MenuCreateTxt));
	}
	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_CUT_ITEM, str._set_and_get(MenuCut));
	AppendMenu(CMenu, MFT_STRING, ID_COPY_ITEM, str._set_and_get(MenuCopy));
	AppendMenu(CMenu, MFT_STRING | enabled, ID_PASTE_ITEM, str._set_and_get(MenuPaste));
	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_LINK_ITEM, str._set_and_get(MenuLink));
	AppendMenu(CMenu, MFT_STRING, ID_DELETE_ITEM, str._set_and_get(MenuDelete));
	AppendMenu(CMenu, MFT_STRING, ID_RENAME_ITEM, str._set_and_get(MenuRename));

	AppendMenu(CMenu, MFT_SEPARATOR, 0, NULL);
	AppendMenu(CMenu, MFT_STRING, ID_INFO_ITEM, str._set_and_get(MenuInfo));
}

void Functional::_init_tree()
{
	SmartStringLoad str(MyComputer_info);
	TV_INSERTSTRUCT _insert;

	_insert.item.mask = TVIF_TEXT | TVIF_PARAM;

	_insert.hParent = NULL;
	_insert.hInsertAfter = TVI_LAST;

	_insert.item.pszText = (char*)str._get();

	HTREEITEM MyComputer = TreeView_InsertItem(Tree, &_insert);
	HTREEITEM Disk;

	for (int i = 0; i < disks->disk_amount; ++i) {
		_insert.hParent = MyComputer;
		_insert.item.pszText = disks->_get_disk(i);
		Disk = TreeView_InsertItem(Tree, &_insert);
		tree_load(Disk, _getPath(Disk));
	}

	TreeView_Expand(Tree, MyComputer, TVE_EXPAND);
	TreeView_SelectItem(Tree, MyComputer);
}

void Functional::disk_list()
{
	char all_disks[512], *disk;
	int amount = GetLogicalDriveStrings(sizeof(all_disks), all_disks);

	disk = all_disks;
	disks = new Disk(amount / 4);

	while (*disk != 0) {
		SendMessage(ComboBox, CB_ADDSTRING, NULL, (LPARAM)disk);
		disks->add_disk(disk);
		disk += 4;
	}
	SendMessage(ComboBox, CB_SETCURSEL, NULL, (LPARAM)1);
}

bool Functional::_delete(const std::string &_delete)
{
	SmartFinder file;

	if (file.find(_delete)) {
		try {
			if (file.is_file())
				boost::filesystem::remove(_delete);
			else if (file.is_directory())
				boost::filesystem::remove_all(_delete);
			
		}
		catch (boost::filesystem::filesystem_error &_error) {
			SmartStringLoad str(ErrorDelete, 64);
			SmartStringLoad str_1(Error_info);

			MessageBox(NULL, str._get(), str_1._get(), MB_OK);
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
	SHELLEXECUTEINFO sei = { 0 };

	sei.cbSize = sizeof(sei);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = "open";
	sei.hwnd = hWnd;
	sei.nShow = SW_SHOWNORMAL;
	sei.lpFile = path.selected_file.c_str();
	sei.lpParameters = NULL;
	sei.lpDirectory = NULL;

	if (ShellExecuteEx(&sei))
		return true;
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
		SmartStringLoad str(Folder_info);

		_info[2] = str._get();
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

std::string Functional::_getPath(HTREEITEM _item)
{
	char *str = new char[100];
	TV_ITEM tv;

	tv.mask = TVIF_TEXT | TVIF_HANDLE;
	tv.hItem = _item;
	tv.pszText = str;
	tv.cchTextMax = 100;
	TreeView_GetItem(Tree, &tv);

	return (char*)tv.pszText;
}

std::string Functional::_get_full_path(HTREEITEM _selected)
{
	std::string temp_path = _getPath(_selected);
	
	HTREEITEM _selected_child = TreeView_GetParent(Tree, _selected);
	HTREEITEM root = TreeView_GetRoot(Tree);

	while (_selected_child != root && _selected_child) {
		temp_path.insert(0, _getPath(_selected_child) + "\\");
		_selected_child = TreeView_GetParent(Tree, _selected_child);
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
		else if (file.is_directory()) {
			temp = _name;
			temp += " (" + std::to_string(i++) + ")";
		}
	}

	return temp;
}

Functional::Functional(HWND _hWnd) :
	path("C:\\", path.main_path), Objects(_hWnd,2)
{
	disk_list();
	update_listview();
	_init_tree();
	_init_main_menu();
}

bool Functional::try_paste()
{
	if (!manip)
		return false;

	std::string temp = manip.file;
	if (manip.path == path.main_path) {
		temp = same_name(manip.file);
	}

	SmartFinder check_file(manip.file);
	try {
		if (check_file.is_file())
			boost::filesystem::copy_file(manip.file, temp, boost::filesystem::copy_option::fail_if_exists);
		else if (check_file.is_directory())
			boost::filesystem::copy_directory(manip.file, path.main_path + temp.substr(temp.rfind('\\', temp.size() - 1)));
	}
	catch (...) {
		SmartStringLoad str(ErrorPaste);
		SmartStringLoad str_1(Error_info);

		MessageBox(NULL, str._get(), str_1._get(), MB_OK);
		return false;
	}

	if (manip.aDelete) {
		if (!_delete(manip.file))
			return false;
	}

	return true;
}

void Functional::update_listview()
{
	std::string buffer;
	SmartFinder file;

	SetWindowText(Edit, path.main_path.c_str());
	ListView_DeleteAllItems(ListView);

	if (file.find(path.main_path + "*")) {
		do {
			if (file.hidden()) {
				_add_lw_item(make_file_info(file._get()));
			}
		} while (file.next());
	}

	item_count = ListView_GetItemCount(ListView);
}

Functional::~Functional()
{
	delete disks;
}

void Functional::tree_load(HTREEITEM _item, const std::string &_path)
{
	SmartFinder file;

	if (!file.find(_path + "*"))
		return;

	TV_INSERTSTRUCT _insert;
	_insert.hParent = _item;
	_insert.item.mask = TVIF_TEXT;

	do {
		if (file.is_directory() && file.hidden()) {
			_insert.item.pszText = file._get().cFileName;
			TreeView_InsertItem(Tree, &_insert);
		}
	} while (file.next());
}
