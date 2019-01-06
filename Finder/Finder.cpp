#include "Finder.h"

Finder::Finder(HWND _hWnd) :
	Functional(_hWnd)
{
}

Functional::Path Finder::_get_path() const
{
	return path;
}

void Finder::create_txt()
{
	int i = 2;
	std::string additional = ".txt";
	SmartFinder file;
	if (file.find(path.main_path + local_ru::DefaultTextFile + additional))
		do {
			additional = " (" + std::to_string(i++) + ").txt";
		} while (file.next());
	additional = path.main_path + local_ru::DefaultTextFile + additional;

	if (CreateFile(additional.c_str(), NULL, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL))
		update_listview();
}

void Finder::create_folder()
{
	int i = 2;
	std::string additional;
	SmartFinder file;

	if (file.find(path.main_path + local_ru::DefaultFolder + additional))
		do {
			additional = " (" + std::to_string(i++) + ")";
		} while (file.next());
	additional = path.main_path + local_ru::DefaultFolder + additional;

	if(CreateDirectory(additional.c_str(),NULL))
		update_listview();
}

void Finder::rename()
{
	char *temp = new char[200];
	HWND edit = ListView_GetEditControl(ListView);
	SendMessage(edit, EM_GETLINE, NULL, (LPARAM)temp);
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
	int amount = 1;
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
			
		if (file.find(path.selected_file + type)) {
			do {
				type = " (" + std::to_string(amount++) + ").lnk";
			} while (file.next());
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
	temp[0] = 0;
	int index = ListView_GetNextItem(ListView,
		-1, LVNI_ALL | LVNI_SELECTED);
	ListView_GetItemText(ListView, index, 0, temp, 200);

	if (!temp) 
		goto end;

	path.selected_file = path.main_path + temp;
	end:
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

	if (MessageBox(hWnd, local_ru::DeleteFileInfo,
		local_ru::DeleteFileHeader, MB_ICONQUESTION | MB_YESNO) == IDYES) {
		_delete(path.selected_file);
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
	SetWindowPos(Edit, temp, WindowRT.left + 300, WindowRT.top + 10, WindowRT.right - 395, 25, NULL);
	SetWindowPos(Button[2], temp, WindowRT.right - 95, WindowRT.top + 10, 30 , 25, NULL);
	SetWindowPos(ComboBox, temp, WindowRT.right - 60, WindowRT.top + 10, 50, 200, NULL);
}

void Finder::show_info()
{
	DialogBoxParam(hInst, MAKEINTRESOURCE(ID_DLG_INFO), hWnd, (DLGPROC)DlgInfo, (LPARAM)this);
}

void Finder::refresh()
{
	update_listview();
}
