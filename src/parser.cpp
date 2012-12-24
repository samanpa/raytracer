#include "parser.h"
#include "io.h"
#include <iostream>

using namespace std;

bplex_val_t bplex_lval;

static void report_error(const string& msg) {
	cerr << msg << " "
	     << bplex_filename() << ":" << bplex_lineno << endl;
}

#define CHECK1(token, TOK, msg)  do {					\
		if ((token) != TOK) {					\
			report_error(msg);				\
			return false;					\
		}							\
	} while(0)

#define CHECK(TOK, msg) CHECK1(bplex_lex(), TOK, msg)

static bool readInt(int& val) {
	CHECK(NUM, " Expecting integer ");
        val = bplex_lval.val;
        return true;
}

static bool readFloat(float& val) {
	int token = bplex_lex();
	if (token == NUM)
		val = bplex_lval.val;
	else if (token == FNUM)
		val = bplex_lval.fval;
	else {
		cerr << token << " " << FNUM << endl;
		report_error ("Expecting float ");
                return false;
        }
	return true;
} 

static bool readVector(vec3& v) {
	CHECK(LANGLE, "Expecting <");
	if (!readFloat(v[0])) return false;
	CHECK(COMMA, "Expecting , ");
	if (!readFloat(v[1])) return false;
	CHECK(COMMA, "Expecting ,");
	if (!readFloat(v[2])) return false;
	CHECK(RANGLE, "Expecting >");
	return true;
}

static bool readIntVector(int v[3]) {
	CHECK(LANGLE, "Expecting <");
	if (!readInt(v[0])) return false;
	CHECK(COMMA, "Expecting , ");
	if (!readInt(v[1])) return false;
	CHECK(COMMA, "Expecting ,");
	if (!readInt(v[2])) return false;
	CHECK(RANGLE, "Expecting >");
	return true;
}

static bool parseVertices(vector<vec3> &vertices) {
	int numVertices;
	CHECK(LBRACE, "expecting {");
	if (!readInt(numVertices))
		return false;

	while (numVertices-- > 0) {
		vec3 v;
		if (!readVector(v)) return false;
		vertices.emplace_back(v[0], v[1], v[2]);
	}
	CHECK(RBRACE, "expecting }");
	return true;
}

static bool parseFaces(scene& scene, vector<vec3> &vertices) {
	int numFaces;
	CHECK(LBRACE, "expecting {");
	if (!readInt(numFaces))
		return false;

	while (numFaces-- > 0) {
		int v[3];
		if (!readIntVector(v)) return false;
		if (v[0] > (int)vertices.size() || v[0] < 0 ||
		    v[1] > (int)vertices.size() || v[1] < 0 ||
		    v[2] > (int)vertices.size() || v[2] < 0) {
			report_error("Invalid vertex");
			cerr << "\t" << v[0] << " " << v[1] 
			     << " " << v[2] << endl;
			return false;
		}
		scene.add(vertices[v[0]], vertices[v[1]], vertices[v[2]]);
	}
	CHECK(RBRACE, "expecting }");
	return true;
}

static bool parseMesh(scene& scene) {
	vector<vec3> vertices;

	CHECK(LBRACE, " Expecting { in mesh");
	
	while(true) {
		int token = bplex_lex();
		switch(token) {
		case VERTICES:
			if (!parseVertices(vertices)) return false;
			break;
		case FACES:
			if (!parseFaces(scene, vertices)) return false;
			break;
		case RBRACE:
			return true;
		default:
			report_error("expecting vertex/face");
			cerr << token << endl;
			return false;
		}
	}

	return true;
}

static bool parseTopLevel(scene& scene) {
	int token = bplex_lex();
	switch (token) {
	case MESH:
		if (!parseMesh(scene))
			return false;
		break;
	default:
		cerr << "Error while parsing file " << endl;
		return false;
	};

	return true;
}

bool parsePov (char *filename, scene& scene) {
	if (bplex_file(filename) == 0)
		return false;

	return parseTopLevel(scene);
}
