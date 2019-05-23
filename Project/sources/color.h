#pragma once
#include <string>

//////////////////////////////
//COLOR STRUCT AND UTILS
//////////////////////////////
struct Color {
	float r, g, b;
	static Color White(){
		const auto c = hexCodeToRGB("#000000");
		return c;
	}
	static Color Red(){
		const auto c = hexCodeToRGB("#FF0000");
		return c;
	}
	static Color Orange(){
		const auto c = hexCodeToRGB("#FFA500");
		return c;
	}
	static Color Purple(){
		const auto c = hexCodeToRGB("#800080");
		return c;
	}
	static Color Blue(){
		const auto c = hexCodeToRGB("#000080");
		return c;
	}
	static Color Black(){
		const auto c = hexCodeToRGB("#000000");
		return c;
	}
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
