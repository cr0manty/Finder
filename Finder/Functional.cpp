#include "Functional.h"

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

void Functional::_init_tree()
{
	TV_INSERTSTRUCT tvInsert;
	tvInsert.hParent = nullptr;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsert.item.pszText = (LPSTR)"Desktop";
	tvInsert.item.lParam = (LPARAM)("Desktop");
	HTREEITEM hDesktop = TreeView_InsertItem(Tree, &tvInsert);

	tvInsert.hParent = hDesktop;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.pszText = (LPSTR)"My Computer";
	tvInsert.item.lParam = (LPARAM)("MyComputer");
	HTREEITEM hMyComputer = TreeView_InsertItem(Tree, &tvInsert);

	for (int i = 0; i < disks->disk_amount; i++) {
		tvInsert.hParent = hMyComputer;
		tvInsert.item.pszText = disks->_get_disk(i);
		tvInsert.item.lParam = (LPARAM)disks->_get_disk(i);
		HTREEITEM hDrive = TreeView_InsertItem(Tree, &tvInsert);
		TreeView_Expand(Tree, hMyComputer, TVE_EXPAND);
		TreeView_SelectItem(Tree, hMyComputer);
	}

}

void Functional::disk_list()
{
	char arr[1024], *b;

	GetLogicalDriveStrings(sizeof(arr), arr);
	b = arr;
	disks = new Disk(strlen(arr) / 3);

	while (*b != 0) {
		SendMessage(ComboBox, CB_ADDSTRING, NULL, (LPARAM)b);
		disks->add_disk(b);
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
	return ShellExecute(NULL, "open", path.selected_file.c_str(), NULL, NULL, SW_SHOWNORMAL);
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

std::string Functional::file_name(const std::string &_path)
{
	return _path.substr(_path.rfind('\\') + 1, _path.size() -1);
}

Functional::Functional(HWND _hWnd) :
	path("C:\\", path.main_path), Objects(_hWnd,4)
{
	disk_list();
	_init_tree();
	update_listview();
}

bool Functional::make_paste()
{
	if (!manip)
		return false;
	std::string temp = file_name(manip.file);

	if (manip.path == path.main_path) {
		int i = 1;
		SmartFinder file;
		std::string new_temp = temp;

		while (file.find(path.main_path + new_temp)) {
			if (file.is_file()) {
				new_temp = temp;
				new_temp.insert(new_temp.rfind('.'), " (" + std::to_string(i++) + ")");
			}
			else if (file.is_directory()) {
				new_temp = temp;
				new_temp += " (" + std::to_string(i++) + ")";
			}

		}
		temp = new_temp;
	}

	SmartFinder check_file(manip.file);
	try {
		if (check_file.is_file())
			boost::filesystem::copy_file(manip.file, path.main_path + temp, boost::filesystem::copy_option::fail_if_exists);
		else if (check_file.is_directory())
			boost::filesystem::copy_directory(manip.file, path.main_path + temp);
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

	item_count = ListView_GetItemCount(ListView);
}

bool Functional::start_rename(int _index)
{
	temp_edit = ListView_EditLabel(ListView, _index);
	
	return temp_edit;
}

bool Functional::end_rename(int)
{
	if (!temp_edit) {
		MessageBox(NULL, local_ru::ErrorEmptyName, local_ru::Error, MB_OK);
		return false;
	}
	char *temp = new char[200];

	delete[] temp;
}

Functional::~Functional()
{
	delete disks;
}
