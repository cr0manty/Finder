#pragma once
#include "includes.h"
#include <boost/filesystem.hpp>

class Functional : public Objects
{
protected:
	HWND temp_edit;
	Disk *disks;
	Path *path;
	Manip *manip;
	FileInfo *info;

	Functional(HWND);

	void disk_list();
	void _init_cmenu();
	void _init_main_menu();
	void _init_tree();
	void tree_load(HTREEITEM, const std::string &);
	void update_listview();

	std::string _getPath(HTREEITEM, char *);
	std::string _get_full_path(HTREEITEM);
	std::string same_name(const std::string &);

	bool _add_lw_item(WIN32_FIND_DATA);
	bool _delete(const std::string &);
	bool open_proc();
	bool try_paste();
	~Functional();
};
