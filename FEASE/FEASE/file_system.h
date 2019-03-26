#pragma once
#include <string>
#include <sys/stat.h>
#include <sstream>
#include <vector>

#define FPATH(X) pathTo(#X).c_str()

static const std::string pathTo(std::string path)
{
	const std::string root = "C:\\Users\\bachp2\\Documents\\IFEM\\FEASE\\FEASE";

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
		new_path.append("\\");
		new_path.append(i);
	}
	return new_path;
}

inline bool checkIfFileExist(const char *name) {
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}