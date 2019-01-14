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
	disk_amount(_amount), disk(new char*[_amount])
{
	disk = new char*[disk_amount];
}

void Disk::add_disk(char *_name)
{
	static int index = 0;

	if (index > disk_amount)
		return;

	disk[index] = new char[4];
	strcpy(disk[index], _name);
}

char * Disk::_get_disk(int _index) const
{
	return disk[_index];
}

Disk::~Disk()
{
	for (int i = 0; i < disk_amount; i++)
		delete[] disk[i];

	delete[] disk;
}

Path::operator bool()
{
	return (selected_file != main_path && selected_file.size());
}

Path::Path(const std::string &_main, const std::string &_next) :
	main_path(_main), next_path(_next), selected_index(-1)
{
}