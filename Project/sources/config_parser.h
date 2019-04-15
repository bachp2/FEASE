#pragma once
#include <map>
#include "color.h"
extern "C"
{
#include <lua535\include\lua.h>
#include <lua535\include\lauxlib.h>
#include <lua535\include\lualib.h>
}

#ifdef _WIN32
#pragma comment(lib, "lua535/liblua53.a")
#endif
class ConfigParser
{
public:
	ConfigParser(const char *path);
	ConfigParser();
	~ConfigParser();

	Color getColor(std::string n);

private:
	std::map<std::string, Color> colorPallete;
	inline void lua_lookupTable(lua_State *L, const char *name, const char *key);
};

