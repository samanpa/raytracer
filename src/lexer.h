#ifndef BPRAY_LEXER_H
#define BPRAY_LEXER_H

#include "scene.h"

int bplex_file (const char *filename);

union bplex_val_t {
	int val;
	float fval;
	char* string;
};

extern bplex_val_t bplex_lval;
extern int bplex_lex();
extern int bplex_lineno;
extern const char* bplex_filename();

enum {
	NUM,
	FNUM,
	IDENTIFIER,
	STRING,

	WIDTH,
	HEIGHT,

	CAMERA,
	ANGLE,
	LOCATION,
	UP,
	LOOKAT,
	
	KMESH,
	MESH,
	VERTICES,
	NORMALS,
	FACES,
	MATERIAL,

	COLOR,
	INCLUDE,

	LBRACE,
	RBRACE,
	LANGLE,
	RANGLE,
	COMMA
};

#endif
