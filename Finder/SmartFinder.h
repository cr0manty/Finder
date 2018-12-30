#pragma once
#include <Windows.h>
#include <string>

class SmartFinder
{
	HANDLE Find;
	WIN32_FIND_DATA file;

public:
	SmartFinder();

	bool find(const std::string &);
	bool next();
	WIN32_FIND_DATA _get() const;
	~SmartFinder();
};

