#pragma once
#include "Functional.h"
#include <ShlObj.h>

class Finder : public Functional
{
public:
	Finder(HWND);
	Path _get_path() const;

	void create_txt();
	void create_folder();
	void rename();

	void open();
	void create_link();
	void back_button();
	void next_button();
	void select_item();
	void context_menu(LPARAM);
	void file_manip(bool);
	void fix_size(LPARAM);
	void disk_change(WPARAM);
	void delete_item();
	void paste();
	void resize_objects();
	void show_info();
	void refresh();
	void tree_select();
	void show_about();
};