#define _CRT_SECURE_NO_WARNINGS
#include "Additional.h"

Manip::Manip()
{
}

Manip::Manip(const std::string &_buffer, const std::string &_path, bool _cut) :
	file(_buffer), aDelete(_cut), path(_path)
{
}

Manip::operator bool()
{
	return file.size();
}

void Manip::clear()
{
	file.clear();
	aDelete = false;
}

Disk::Disk(int _amount) :
	disk_amount(_amount)
{
	disk_path = new char*[disk_amount];
	disk_name = new char*[disk_amount];

}

void Disk::add_disk(char *_name)
{
	static int index = 0;

	if (index > disk_amount)
		return;

	disk_path[index] = new char[4];
	strcpy(disk_path[index], _name);

	disk_name[index] = new char[3];
	disk_name[index][0] = _name[0];
	disk_name[index][1] = _name[1];
	disk_name[index++][2] = '\0';
}

char * Disk::_get_disk(int _index) const
{
	return disk_path[_index];
}

char * Disk::_get_disk_name(int _index) const
{
	return disk_name[_index];
}

Disk::~Disk()
{
	for (int i = 0; i < disk_amount; i++) {
		delete[] disk_path[i];
		delete[] disk_name[i];
	}

	delete[] disk_path;
	delete[] disk_name;
}

Path::operator bool()
{
	return (selected_file != main_path && selected_file.size());
}

Path::Path(const std::string &_main, const std::string &_next) :
	main_path(_main), next_path(_next)
{
}