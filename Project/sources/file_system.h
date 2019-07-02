#pragma once
#include <string>
#include <sys/stat.h>
#include <sstream>
#include <cstdarg>
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <direct.h>

#define ICON_FOLDER pathTo("res/gui_icons/")

#define FPATH(X) pathTo(#X).c_str()
#define RES(X) pathTo("res/" #X).c_str()
#define SHAD(X) pathTo("res/shaders/" #X).c_str()

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

inline static bool checkIfFileExist(std::string name) {
	return checkIfFileExist(name.c_str());
}

inline static void writeFile(const char * path, const std::string& content = "")
{
	std::ofstream outfile (path);

	outfile << content << std::endl;

	outfile.close();
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

//code taken from here: https://stackoverflow.com/questions/69738/c-how-to-get-fprintf-results-as-a-stdstring-w-o-sprintf#69911
std::string vformat(const char *fmt, va_list ap);
inline static std::string str_format (const char *fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	std::string buf = vformat (fmt, ap);
	va_end (ap);
	return buf;
}

inline static std::string vformat (const char *fmt, va_list ap)
{
	// Allocate a buffer on the stack that's big enough for us almost
	// all the time.  Be prepared to allocate dynamically if it doesn't fit.
	size_t size = 1024;
	char stackbuf[1024];
	std::vector<char> dynamicbuf;
	char *buf = &stackbuf[0];
	va_list ap_copy;

	while (1) {
		// Try to vsnprintf into our buffer.
		va_copy(ap_copy, ap);
		int needed = vsnprintf (buf, size, fmt, ap);
		va_end(ap_copy);

		// NB. C99 (which modern Linux and OS X follow) says vsnprintf
		// failure returns the length it would have needed.  But older
		// glibc and current Windows return -1 for failure, i.e., not
		// telling us how much was needed.

		if (needed <= (int)size && needed >= 0) {
			// It fit fine so we're done.
			return std::string (buf, (size_t) needed);
		}

		// vsnprintf reported that it wanted to write more characters
		// than we allotted.  So try again using a dynamic buffer.  This
		// doesn't happen very often if we chose our initial size well.
		size = (needed > 0) ? (needed+1) : (size*2);
		dynamicbuf.resize (size);
		buf = &dynamicbuf[0];
	}
}
