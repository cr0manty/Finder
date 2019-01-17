#pragma once
#include <string>

class Disk 
{
	char **disk;
public:
	const int disk_amount;

	Disk(int);
	void add_disk(char*);
	char* _get_disk(int) const;
	~Disk();
};

class FileInfo
{
	WIN32_FIND_DATA file;
	void create_time();
	void change_time();
	std::string temp;
	bool is_file;
public:
	FileInfo(WIN32_FIND_DATA);
	const char * _get_info(int);
	const char * _get_header(int);
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
