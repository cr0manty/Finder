#include "includes.h"

SmartFinder::SmartFinder() :
	Find(NULL)
{
}

SmartFinder::SmartFinder(const std::string &_path) :
	Find(NULL)
{
	find(_path);
}

bool SmartFinder::find(const std::string &_name)
{
	Find = FindFirstFile(_name.c_str(), &file);
	return Find != INVALID_HANDLE_VALUE;
}

bool SmartFinder::next()
{
	return FindNextFile(Find, &file);
}

bool SmartFinder::is_file() const
{
	return file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

bool SmartFinder::is_nHidden() const
{
	return !(file.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) &&
		!(file.dwFileAttributes &  FILE_ATTRIBUTE_REPARSE_POINT) &&
		lstrcmp(file.cFileName, "..") && lstrcmp(file.cFileName, ".");
}

WIN32_FIND_DATA SmartFinder::_get() const
{
	return file;
}

SmartFinder::~SmartFinder()
{
	if(Find)
		FindClose(Find);
}
