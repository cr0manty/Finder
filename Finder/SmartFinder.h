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
	bool is_directory() const;
	bool hidden() const;

	WIN32_FIND_DATA _get() const;
	~SmartFinder();
};

