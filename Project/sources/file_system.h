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
inline static const std::string pathTo(std::string path)
{
	using namespace std;
	//const std::string root = "C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\FEASE";
	string root = GetFulProjectlPath( "..\\" );
	//root = "C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\FEASE\\";

	vector<string> vect;
	stringstream ss(path);

	while (ss.good())
	{
		string substr;
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
		//printf( "Full path is: %s\n", full );
		return full;
	}
	printf( "Invalid path\n" );
	return nullptr;
}


inline static bool checkIfFileExist(const char *name) {
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}

inline static std::vector<uint8_t> readFile(const char* path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open())
		printf("Failed to open file\n");

	const auto size = file.tellg();
	file.seekg(0, std::ios::beg);
	auto bytes = std::vector<uint8_t>(size);
	file.read(reinterpret_cast<char*>(&bytes[0]), size);
	file.close();

	return bytes;
}