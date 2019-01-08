#pragma once
#include "SmartFinder.h"
#include <boost/filesystem.hpp>

class Functional : public Objects
{
	std::string file_name(const std::string &);
protected:
	int item_count;
	HWND temp_edit;
	Disk *disks;
	Path path;
	Manip manip;

	Functional(HWND);

	void _init_menu();
	void _init_tree();
	void disk_list();
	bool _delete(const std::string &);
	bool _add_lw_item(const std::string *);
	bool open_proc();
	bool make_paste();
	void update_listview();
	bool start_rename(int);
	bool end_rename(int);
public:
	std::string* make_file_info(const WIN32_FIND_DATA &) const;
	~Functional();
};
