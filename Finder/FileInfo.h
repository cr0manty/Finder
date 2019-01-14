#pragma once

class FileInfo
{
	WIN32_FIND_DATA file;
	std::string temp;
	void make_name();
	void make_create();
	void make_change();
	void make_type();
	void make_size();
public:
	FileInfo(WIN32_FIND_DATA);
	const char * info(int);
	const char * header(int);
};

