#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <chrono>
#include <iostream>
#include <string>
#include <filesystem>
#include <format>
#include <fstream>

class Logging
{
public:
	Logging();

	std::string getFileName() const;
	void log(const std::string input) const;

private:
	const std::string m_directory = "./logs/";
	std::string m_filePath;

	void beginLogging(const std::string& now) const;
	void createDirectory() const;
};

#endif