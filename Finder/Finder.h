#pragma once
#include "Functional.h"


class Finder : public Functional
{
	int current_lang;
public:
	Finder(HWND);
	Path _get_path() const;

	void create_txt();
	void create_folder();
	void rename();

	void open();
	void create_link();
	void show_back();
	void show_next();
	void select_item();
	void context_menu(LPARAM);
	void file_manip(bool);
	void fix_size(LPARAM);
	void disk_change(WPARAM);
	void delete_item();
	void make_paste();
	void show_info();
	void make_refresh();
	void show_about();
	void tree_to_list();
	void tree_show(LPARAM);
	void minimize_window();
	void exit();
	void change_lang(int);
};