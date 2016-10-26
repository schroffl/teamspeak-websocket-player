#ifdef _WIN32
#include <Windows.h>
#endif

#include "config.h"
#include <iostream>
#include <sstream>

void Config::loadFile(const char *filePath) {
	this->filePath = (char *) filePath;

	std::ifstream fileStream(filePath);

	for (std::string line; getline(fileStream, line);) {
		int splitAt = line.find('=');

		std::string key = line.substr(0, splitAt);
		int value = std::stoi(line.substr(splitAt + 1));

		set(key, value);
	}

	fileStream.close();
}

void Config::saveFile() {
	std::ofstream fileStream(this->filePath);

	for (auto const &entry : options)
		fileStream << entry.first << "=" << entry.second << std::endl;

	fileStream.close();
}

bool Config::has(std::string key) {
	return options.find(key) != options.end();
}

int Config::get(std::string key, int defaultValue) {
	if (!has(key))
		return defaultValue;

	return options[key];
}

void Config::set(std::string key, int value) {
	options[key] = value;
}