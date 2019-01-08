#pragma once
#include <Windows.h>
#include <string>
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
BOOL CALLBACK DlgAbout(HWND, UINT, WPARAM, LPARAM);

class Disk 
{
	char **disk_name;
public:
	const int disk_amount;

	Disk(int);
	void add_disk(char*);
	char* _get_disk(int) const;
	~Disk();
};

struct Path 
{
	std::string main_path;
	std::string next_path;
	std::string	selected_file;
	int selected_index;

	Path(const std::string &, const std::string &);
	operator bool();
};

struct Manip 
{
	std::string file;
	std::string path;
	bool aDelete;

	Manip();
	Manip(const std::string &, const std::string &, bool);
	operator bool();
	void clear();
};

struct Objects
{
	HWND ListView;
	HWND Edit;
	HWND ComboBox;
	HWND *Button;
	HWND Tree;
	HWND Search;
	HMENU Menu;
	HWND hWnd;
	const int number_colum;

	Objects(HWND, int);

	void resize();
	~Objects();
private:
	void _create_listview();
	void _create_tree();
	void _crete_objects();
	void set_listviw_colum();
};