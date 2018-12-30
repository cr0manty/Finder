#include "Finder.h"

Finder::Finder(HWND _hWnd) :
	Functional(_hWnd)
{
}

Functional::Path Finder::_get_path() const
{
	return path;
}

bool Finder::create_txt()
{

	return false;
}

void Finder::open()
{
	if (!path)
		return;

	if (!is_file(path.selected_file)) {
		open_proc();
		return;
	}
	path.main_path = path.selected_file + '\\';
	update_listview();
}

void Finder::back_button()
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

void Finder::next_button()
{
	if (path.next_path != path.main_path) {
		path.main_path = path.next_path;
		update_listview();
	}
}

void Finder::select_item()
{
	char *temp = new char[200];
	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 200);
	path.selected_file = path.main_path + temp;
	delete[] temp;
}

void Finder::context_menu(LPARAM lParam)
{
	char *temp = new char[200];
	_init_menu();

	TrackPopupMenu(Menu, TPM_RIGHTBUTTON |
		TPM_TOPALIGN |
		TPM_LEFTALIGN,
		LOWORD(lParam),
		HIWORD(lParam), 0, hWnd, NULL);

	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 200);

	path.selected_file = path.main_path + temp;
	delete[] temp;

	DestroyMenu(Menu);
}

void Finder::file_manip(bool _cut)
{
	if (path) {
		manip = File_Manip(path.selected_file, _cut);
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

	if (MessageBox(hWnd, local_ru::DeleteFileInfo,
		local_ru::DeleteFileHeader, MB_ICONQUESTION | MB_YESNO) == IDYES) {
		delete_file(path.selected_file);
		update_listview();
	}
}

void Finder::paste()
{
	if (make_paste()) {
		update_listview();
		manip.clear();
	}
}

void Finder::resize_objects()
{
	RECT WindowRT;
	HWND temp = NULL;

	GetClientRect(hWnd, &WindowRT);
	SetWindowPos(ListView, temp, WindowRT.left + 300, WindowRT.top + 45, WindowRT.right - 310, WindowRT.bottom - 55, NULL);
	SetWindowPos(Tree, temp, WindowRT.left + 10, WindowRT.top + 45, WindowRT.left + 280, WindowRT.bottom - 55, NULL);
	SetWindowPos(Edit, temp, WindowRT.left + 300, WindowRT.top + 10, WindowRT.right - 365, 25, NULL);
	SetWindowPos(ComboBox, temp, WindowRT.right - 60, WindowRT.top + 10, 50, 200, NULL);
}

std::string * Finder::_get_file_info(const WIN32_FIND_DATA &_file) const
{
	return get_file_info(_file);
}

void Finder::show_info()
{
	DialogBoxParam(hInst, MAKEINTRESOURCE(ID_DLG_INFO), hWnd, (DLGPROC)DlgInfo, (LPARAM)this);

}
