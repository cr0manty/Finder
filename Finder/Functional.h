#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SmartFinder.h"
#include "Local.h"
#include "resource.h"
#include <commctrl.h>

#pragma comment(lib, "ComCtl32.Lib")

#define ID_CREATE_TEXT_ITEM 0x00002
#define ID_BACK_BUTTON 0x00003
#define ID_NEXT_BUTTON 0x00004
#define ID_PATH_EDIT 0x00005
#define ID_DISKLIST_CB 0x00006
#define ID_DELETE_ITEM 0x00007
#define ID_COPY_ITEM 0x00008
#define ID_CUT_ITEM 0x00009
#define ID_PASTE_ITEM 0x00010
#define ID_LISTVIEW 0x00011
#define ID_TREEVIEW 0x00012
#define ID_OPEN_ITEM 0x00013
#define ID_RENAME_ITEM 0x00014
#define ID_INFO_ITEM 0x00015
#define ID_LINK_ITEM 0x00016
#define ID_CREATE_FOLDER 0x00017

static HINSTANCE hInst;
static LPCTSTR wnd_class = "Finder";

ATOM MyRegisterClass(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgInfo(HWND, UINT, WPARAM, LPARAM);

class Functional
{
protected:
	HWND ListView;
	HWND Edit;
	HWND ComboBox;
	HWND *Button;
	HWND Tree;
	HMENU Menu;
	HWND hWnd;
	const int number_colum;

	struct Path {
		std::string main_path;
		std::string next_path;
		std::string	selected_file;
		Path(const std::string &, const std::string &);
		operator bool();
	} path;

	struct File_Manip {
		std::string buffer_file;
		bool delete_after_paste;

		File_Manip();
		File_Manip(const std::string &, bool);
		operator bool();
		void clear();
	} manip;


public:
	Functional(HWND);

	std::string file_name() const;
	bool find_same(const std::string &);
	int set_listviw_colum();
	void _init_menu();
	void _create_listview();
	void _create_tree();
	void _crete_objects();
	void disk_list();
	bool delete_file(const std::string &);
	bool _add_lw_item(const std::string *);
	bool is_file(const std::string &);
	bool open_proc();
	bool make_paste();
	void update_listview();

	std::string* make_file_info(const WIN32_FIND_DATA &) const;
	~Functional();
};


