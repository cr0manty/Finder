#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"

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
}

void Disk::add_disk(char *_name)
{
	static int index = 0;

	if (index > disk_amount)
		return;

	disk[index] = new char[4];
	strcpy(disk[index++], _name);
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

void FileInfo::create_time()
{
	SYSTEMTIME sys_time;

	FileTimeToSystemTime(&file.ftCreationTime, &sys_time);
	char *buffer = new char[32];

	sprintf(buffer,
		"%02d.%02d.%d %02d:%02d:%02d",
		sys_time.wDay,
		sys_time.wMonth,
		sys_time.wYear,
		sys_time.wHour,
		sys_time.wMinute,
		sys_time.wSecond);

	temp = buffer;
	delete[] buffer;
}

void FileInfo::change_time()
{
	SYSTEMTIME sys_time;

	FileTimeToSystemTime(&file.ftLastWriteTime, &sys_time);
	char *buffer = new char[32];

	sprintf(buffer,
		"%02d.%02d.%d %02d:%02d:%02d",
		sys_time.wDay,
		sys_time.wMonth,
		sys_time.wYear,
		sys_time.wHour,
		sys_time.wMinute,
		sys_time.wSecond);

	temp = buffer;
	delete[] buffer;

}

FileInfo::FileInfo(WIN32_FIND_DATA _file) :
	file(_file)
{
	is_file = file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

const char * FileInfo::_get_info(int _switch)
{
	SmartStringLoad str;

	switch(_switch)
	{
	case 0:
		return file.cFileName;
		
	case 1:
		change_time();
		break;

	case 2:
		if (is_file) {
			temp = file.cFileName;
			temp = temp.substr(temp.rfind('.') + 1, temp.size() - 1).c_str();
		}
		else {
			temp = str._set_and_get(Folder_info);
		}
		break;

	case 3:
		if (is_file) {
			temp = std::to_string((file.nFileSizeHigh * MAXDWORD) + file.nFileSizeLow) + " ��";
			break;
		}
		else {
			return " ";
		}

	case 4:
		create_time();
		break;

	default:
		return " ";
	}
	return temp.c_str();
}

const char * FileInfo::_get_header(int _switch)
{
	SmartStringLoad str;
	
	switch (_switch)
	{
	case 0:
		temp = str._set_and_get(Table_name);
		break;

	case 1:
		temp = str._set_and_get(Table_date_change);
		break;

	case 2:
		temp = str._set_and_get(Table_type);
		break;

	case 3:
		temp =  str._set_and_get(Table_size);
		break;

	case 4:
		temp = str._set_and_get(Table_date_create);
		break;

	case 5:
		temp = str._set_and_get(PathInfo);
		break;

	default:
		return " ";
	}
	return temp.c_str();
}
