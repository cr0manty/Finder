#pragma once

class SmartFinder
{
	HANDLE Find;
	WIN32_FIND_DATA file;

public:
	SmartFinder();
	SmartFinder(const std::string &);

	bool find(const std::string &);
	bool next();
	bool is_file() const;
	bool is_nHidden() const;

	WIN32_FIND_DATA _get() const;
	~SmartFinder();
};

