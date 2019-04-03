#pragma once
#include <string>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>

#define FPATH(X) pathTo(#X).c_str()

//work only for Windows system
std::string GetFulProjectlPath(const char * partialPath);
static const std::string pathTo(std::string path)
{
	//const std::string root = "C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\FEASE";
	std::string root = GetFulProjectlPath( "..\\" );
	//root = "C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\FEASE\\";

	std::vector<std::string> vect;
	std::stringstream ss(path);

	while (ss.good())
	{
		std::string substr;
		getline(ss, substr, '/');
		vect.push_back(substr);
			
	}

	auto new_path = root;
	for (const auto i : vect)
	{
		new_path.append(i);
		new_path.append("\\");
	}
	new_path.pop_back(); // remove last \

	return new_path;
}

inline static std::string GetFulProjectlPath( const char * partialPath )
{
	char full[_MAX_PATH];
	if( _fullpath( full, partialPath, _MAX_PATH ) != NULL )
	{
		printf( "Full path is: %s\n", full );
		return full;
	}
	printf( "Invalid path\n" );
	return nullptr;
}


inline bool checkIfFileExist(const char *name) {
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}