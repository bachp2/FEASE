#include "config_parser.h"
#include <string>

inline static bool CheckLua(lua_State *L, int r){
	if(r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		printf("%s\n", errormsg.c_str());
		return false;
	}
	return true;
}

void ConfigParser::lua_lookupTable(lua_State *L, const char *name, const char *key){
	lua_getglobal(L, name);
	if(lua_istable(L, -1)){
		lua_pushstring(L, key);
		lua_gettable(L, -2);
		colorPallete[name] = hexCodeToRGB(lua_tostring(L, -1));
		printf("%s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	
}

ConfigParser::ConfigParser(const char* path)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if( CheckLua(L, luaL_dofile(L, path)) ){
		const char* table_vars[] = {
			"dot", "background", "grid", "text"
		};

		for(const std::string& t : table_vars){
			lua_lookupTable(L, t.c_str(), "color");
		}
	}
	lua_close(L);
}

Color ConfigParser::getColor(std::string n)
{	
	return colorPallete[n];
}

ConfigParser::~ConfigParser()
{
}
