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
	disk_name = new char*[disk_amount];
}

void Disk::add_disk(char *_name)
{
	static int index = 0;

	if (index > disk_amount)
		return;

	disk_name[index] = new char[4];
	strcpy(disk_name[index++], _name);
}

char * Disk::_get_disk(int _index) const
{
	return disk_name[_index];
}

Disk::~Disk()
{
	for (int i = 0; i < disk_amount; i++)
		delete[] disk_name[i];

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