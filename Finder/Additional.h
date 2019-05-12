#pragma once
#include <string>

class Disk 
{
	char **disk;
public:
	const int disk_amount;

	Disk(int);

	char * _get_disk(int) const;
	void add_disk(char*);

	~Disk();
};

class FileInfo
{
	bool is_file;
	std::string temp;
	WIN32_FIND_DATA file;

	void change_time();
	void create_time();
public:
	FileInfo(WIN32_FIND_DATA);

	char * _get_header(int);
	char * _get_info(int);
};

struct Path 
{
	int selected_index;
	int item_amount;
	std::string main_path;
	std::string next_path;
	std::string	selected_file;

	Path(const std::string &);
	operator bool();
};

struct Manip 
{
	bool aDelete;
	std::string file;
	std::string path;

	Manip(const std::string &, const std::string &, bool);
	operator bool();
	void clear();
};

class Shrek
{
	PAINTSTRUCT ps;
	HANDLE hBitmap;
	HANDLE hOldBitmap;
	BITMAP Bitmap;
	HDC hCompatibleDC;
	RECT rt;
	HWND *hDlg;
	HDC hdc;
public:
	Shrek(HWND &, RECT);
	void make_picture(int);
	~Shrek();
};