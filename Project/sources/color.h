#pragma once
#include <string>
#include "error.h"
//////////////////////////////
//COLOR STRUCT AND UTILS
//////////////////////////////
class Color;
Color hexCodeToRGB(std::string input);
struct Color {
	float r, g, b;
	Color color(std::string i){
		return hexCodeToRGB(i);
	}
	static Color White(){
		static auto c = hexCodeToRGB("#ffffff");
		return c;
	}
	static Color Grey(){
		static auto c = hexCodeToRGB("#C1C1C1");
		return c;
	}
	static Color Red(){
		static auto c = hexCodeToRGB("#FF0000");
		return c;
	}
	static Color Orange(){
		static auto c = hexCodeToRGB("#FFA500");
		return c;
	}
	static Color Purple(){
		static auto c = hexCodeToRGB("#800080");
		return c;
	}
	static Color Blue(){
		static auto c = hexCodeToRGB("#000080");
		return c;
	}
	static Color Black(){
		static auto c = hexCodeToRGB("#000000");
		return c;
	}
};

inline static Color hexCodeToRGB(std::string input) {
	if (input[0] == '#')
		input.erase(0, 1);
	else ERR("invalid color input");

	unsigned int value = stoul(input, nullptr, 16);

	Color color;
	color.r = ((value >> 16) & 0xff) / 255.0;
	color.g = ((value >> 8) & 0xff) / 255.0;
	color.b = ((value >> 0) & 0xff) / 255.0;
	return color;
}
