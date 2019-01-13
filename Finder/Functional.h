#pragma once
#include "includes.h"
#include <boost/filesystem.hpp>

class Functional : public Objects
{
protected:
	int item_count;
	HWND temp_edit;
	Disk *disks;
	Path path;
	Manip manip;

	Functional(HWND);

	void _init_cmenu();
	void _init_tree();
	void update_listview();
	void disk_list();
	void tree_load(HTREEITEM, const std::string &);
	void _init_main_menu();

	std::string _getPath(HTREEITEM);
	std::string _get_full_path(HTREEITEM);
	std::string same_name(const std::string &);

	bool _add_lw_item(const std::string *);
	bool open_proc();
	bool try_paste();
	bool _delete(const std::string &);
	bool start_rename(int);
	bool end_rename(int);
public:
	std::string* make_file_info(const WIN32_FIND_DATA &) const;
	~Functional();
};
