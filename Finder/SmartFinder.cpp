#include "SmartFinder.h"

SmartFinder::SmartFinder()
{
}

bool SmartFinder::find(const std::string &_name)
{
	Find = FindFirstFile(_name.c_str(), &file);
	return Find;
}

bool SmartFinder::next()
{
	return FindNextFile(Find, &file);
}

WIN32_FIND_DATA SmartFinder::_get() const
{
	return file;
}


SmartFinder::~SmartFinder()
{
	FindClose(Find);
}
