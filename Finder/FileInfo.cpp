#include "includes.h"

FileInfo::FileInfo(WIN32_FIND_DATA _file) :
	file(_file)
{
}

void FileInfo::make_name()
{
	int size = strlen(file.cFileName);
	temp = file.cFileName;
}

void FileInfo::make_create()
{
	SYSTEMTIME sys_time;
	FileTimeToSystemTime(&file.ftCreationTime, &sys_time);
	char *buffer = new char[64];

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

void FileInfo::make_change()
{
	SYSTEMTIME sys_time;
	FileTimeToSystemTime(&file.ftLastWriteTime, &sys_time);
	char *buffer = new char[64];

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

void FileInfo::make_type()
{
	if (file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
		make_name();
		temp = temp.substr(temp.find('.') + 1, temp.size() - 1);
	}
	else {
		SmartStringLoad str(Folder_info);
		temp = str._get();
	}
}

void FileInfo::make_size()
{
	temp = new char[32];

	if (file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
		temp = std::to_string((file.nFileSizeHigh * MAXDWORD) + file.nFileSizeLow) + " สม";
	else
		temp = " ";
}

const char * FileInfo::info(int _case)
{
	switch (_case)
	{
	case 0:
		make_name();
		break;

	case 1:
		make_change();
		break;

	case 2:
		make_type();
		break;

	case 3:
		make_size();
		break;

	case 4:
		make_create();
		break;

	default:
		temp = " ";
		break;
	}
	return temp.c_str();
}

const char * FileInfo::header(int _case)
{
	SmartStringLoad str;

	switch (_case)
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
		temp = str._set_and_get(Table_size);
		break;

	case 4:
		temp = str._set_and_get(Table_date_create);
		break;

	default:
		temp = " ";
		break;
	}
	return temp.c_str();
}
