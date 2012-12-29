#ifndef BPRAY_PARSER_H
#define BPRAY_PARSER_H

#include <stdexcept>
#include "lexer.h"
#include "scene.h"
#include <string>
#include <sstream>

class parse_error : public std::exception {
	int lineno;
	const char* filename;
	std::string msg;
public:
	parse_error(const std::string& msg) 
		: lineno (bplex_lineno)
		, filename(bplex_filename())
		, msg(msg) {}

	const char* what() {
		std::stringstream str;
		str << msg << " @" << filename << ":" << lineno;
		return strdup(str.str().c_str());
	}
};

bool parsePov (char *filename, scene& scene);

#endif
