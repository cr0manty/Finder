#pragma once
#include "Functional.h"

class Finder : private Functional
{
public:
	Finder(HWND);
	Functional::Path _get_path() const;

	void create_txt();
	void create_folder();
	void rename();

	void open();
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
	std::string* _get_file_info(const WIN32_FIND_DATA &) const;
	void show_info();
};