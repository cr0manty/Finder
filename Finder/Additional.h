#pragma once
#include "Objects.h"

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

