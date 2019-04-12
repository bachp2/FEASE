#pragma once
#include "color.h"
#include <map>
#include <fstream>
#include <iostream>

struct ColorConfig {
	std::map<std::string, Color> pallete;
	inline void parseColorConfig(const char *file) {
		std::string key, hexvalue;
		std::ifstream myfile;
		myfile.open(file);

		while (myfile >> key >> hexvalue)
		{
			pallete[key] = hexCodeToRGB(hexvalue);
			//std::cout << key << " " << hexvalue << '\n';
		}
		myfile.close();
	}
};


