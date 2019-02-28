#pragma once
#include "Functional.h"

class Finder : public Functional
{
	void create_folder();
	void create_link();
	void create_txt();

	void delete_item();
	void disk_change(__int64);

	void rename(__int64);
	void file_manip(bool);

	void make_paste();

	void open();
	void select_item();
	void show_about();
	void show_back();
	void show_info();
	void show_next();

	void tree_to_list();
	void tree_show(__int64);
public:
	Finder(HWND);
	bool context_menu(__int64);
	bool fix_size(__int64);

	bool command_switch(__int64);
	bool hotkey_switch(__int64);
	bool notify_switch(__int64);
	bool dlgInfo_proc(HWND);
	~Finder();
};