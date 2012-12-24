%{
#include "lexer.h"
#include "ray.h"
#include "scene.h"
#include "io.h"
#include <vector>
%}

DIGIT                 [0-9]
DIGITS                {DIGIT}+
IDENT_START_CHAR      [a-zA-Z]
IDENT_CHAR            [0-9a-zA-Z_]
COMMENT               "/"\*([^*]|[\n]|(\*+([^*/]|[\n])))*\*+"/"
%option yylineno
%option prefix="bplex_"
%option outfile="lexer.cc"

%%

angle {
        return ANGLE;
}

camera {
        return CAMERA;
}

color {
        return COLOR;
}

faces {
	return FACES;
}

height|Height {
        return HEIGHT;
}

look_at|lookat {
	return LOOKAT;
}

location {
	return LOCATION;
}

mesh {
	return MESH;
}

vertices {
	return VERTICES;
}

width|Width {
        return WIDTH;
}


" "|"\n"|"\r\n"|"\t"  {
        return yylex ();
}

"//".*"\n" {
	return yylex ();
}

"," {
        return COMMA;
}

"{" {
	return LBRACE;
}

"}" {
	return RBRACE;
}

"<" {
	return LANGLE;
}

">" {
	return RANGLE;
}

{COMMENT} {
	return yylex ();
}

{IDENT_START_CHAR}{IDENT_CHAR}* {
        bplex_lval.string = strdup (yytext);
        return IDENTIFIER;
}

\".*\" {
        bplex_lval.string = strdup (yytext + 1);
        bplex_lval.string [strlen (bplex_lval.string) - 1] = '\0';
        return STRING;
}

-?{DIGITS} {
	bplex_lval.val = atoi(yytext);
	return NUM;
}

[-+]?{DIGITS}\.?{DIGITS}?([eE][-+]{DIGITS})? {
        bplex_lval.fval = atof (yytext);
        return FNUM;
}

#include {
	 return INCLUDE;
}

%%
struct lexer_file {
	struct yy_buffer_state *state;
	int lineno;
	std::string filename;
};

static std::vector<lexer_file> lexerFiles;

int bplex_wrap()
{
	auto &currFile = lexerFiles.back();
 	fclose (currFile.state->yy_input_file);
	yy_delete_buffer (currFile.state);
	lexerFiles.pop_back();

	if (lexerFiles.size()) {
		auto& currFile = lexerFiles.back();
		yy_switch_to_buffer(currFile.state);
	        yylineno = currFile.lineno;
		return 0;
	} else
		return 1;
}

const char* bplex_filename()
{
	auto &currFile = lexerFiles.back();
	return currFile.filename.c_str();
}

int bplex_file (const char *filename)
{
        FILE *fp = bpray_file_open(filename, "r");
	if (!fp)
		return 0;
 	else {
		yyin = fp;
		
		lexer_file lf;
		lf.lineno   = yylineno;
		lf.state    = yy_create_buffer (yyin, YY_BUF_SIZE);
		lf.filename = filename;
		lexerFiles.push_back(lf);
		
		yy_switch_to_buffer (lf.state);

		yylineno = 1;
		return 1;
	}
}

