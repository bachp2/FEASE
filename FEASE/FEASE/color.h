#pragma once
#include <string>

//////////////////////////////
//COLOR STRUCT AND UTILS
//////////////////////////////
struct Color {
	float r, g, b;
};

inline static Color hexCodeToRGB(std::string input) {
	if (input[0] == '#')
		input.erase(0, 1);

	unsigned int value = stoul(input, nullptr, 16);

	Color color;
	color.r = ((value >> 16) & 0xff) / 255.0;
	color.g = ((value >> 8) & 0xff) / 255.0;
	color.b = ((value >> 0) & 0xff) / 255.0;
	return color;
}
