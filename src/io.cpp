#include "io.h"
#include <string>
#include <vector>
#include <iostream>

static std::vector<std::string> libraryPath;

FILE *bpray_file_open(const char *filename, const char *mode)
{
	for (std::string &path : libraryPath) {
		std::string fullname = path + "/" + filename;
		FILE* file = fopen(fullname.c_str(), mode);
		if (file)
			return file;
	}
	return NULL;
}

void bpray_library_path_add (const char *path)
{
	char *canonicalPath = realpath(path, NULL);
	if (canonicalPath) {
		libraryPath.push_back(canonicalPath);
	}
}

