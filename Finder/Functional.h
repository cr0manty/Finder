#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SmartFinder.h"
#include "Local.h"
#include "resource.h"
#include <commctrl.h>

#pragma comment(lib, "ComCtl32.Lib")

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

	struct Manip {
		std::string file;
		std::string path;
		bool aDelete;

		Manip();
		Manip(const std::string &,const std::string &, bool);
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


