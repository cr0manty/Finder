#pragma once
#include "Functional.h"

class Finder : public Functional
{
public:
	Finder(HWND);
	Path & _get_path() const;

	void context_menu(LPARAM);
	void create_folder();
	void create_link();
	void create_txt();

	void delete_item();
	void disk_change(WPARAM);

	void end_rename();
	void exit();

	void file_manip(bool);
	void fix_size(LPARAM);

	void make_paste();
	void make_refresh();
	void minimize_window();

	void open();

	void select_item();
	void show_about();
	void show_back();
	void show_info();
	void show_next();
	void start_rename();

	void tree_to_list();
	void tree_show(LPARAM);

	~Finder();
};