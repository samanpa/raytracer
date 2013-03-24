#include "parser.h"
#include "io.h"
#include "utils.h"
#include <sstream>

using namespace std;

bplex_val_t bplex_lval;

static inline void CHECK(int token, const char* msg){
	if (bplex_lex() != token)
		throw parse_error(msg);
}

static int readInt() {
	CHECK(NUM, " Expecting integer ");
        return bplex_lval.val;
}

static float readFloat() {
	int token = bplex_lex();
	if (token == NUM)
		return bplex_lval.val;
	else if (token == FNUM)
		return bplex_lval.fval;
	throw parse_error("Expecting float ");
} 

static vec3f readVector() {
	CHECK(LANGLE, "Expecting <");
        float f1 = readFloat(); CHECK(COMMA, "Expecting , ");
	float f2 = readFloat(); CHECK(COMMA, "Expecting ,");
	float f3 = readFloat();
	CHECK(RANGLE, "Expecting >");
	return vec3f(f1, f2, f3);
}

static vec3<int> readIntVector() {
	CHECK(LANGLE, "Expecting <");
        int v1 = readInt(); CHECK(COMMA, "Expecting , ");
	int v2 = readInt(); CHECK(COMMA, "Expecting ,");
	int v3 = readInt();
	CHECK(RANGLE, "Expecting >");
	return vec3<int>(v1, v2, v3);
}

static void parseVertices(scene& scene) {
	CHECK(LBRACE, "expecting {");
	int numVertices = readInt();
	for (int i = 0; i < numVertices; i++) {
		vec3f v = readVector();
		scene.addVertex(v);
	}
	CHECK(RBRACE, "expecting }");
}

static void parseNormals(scene &scene) {
	CHECK(LBRACE, "expecting {");
	unsigned int numNormals = readInt();

	if (numNormals != scene.getNumVertices()) {
		stringstream str;
		str << "number of vertices and normals don't match ."
		    << " num vertices " << scene.getNumVertices() 
		    << " num normals  " << numNormals;
		throw parse_error(str.str());
	}
	while (numNormals-- > 0) {
		vec3f v = readVector();
		scene.addNormal(v);
	}
	CHECK(RBRACE, "expecting }");
}

static void parseFaces(scene& scene) {
	CHECK(LBRACE, "expecting {");
	int numFaces = readInt();

	while (numFaces-- > 0) {
		vec3<int> v(readIntVector());
		if (!scene.addFace(v[0], v[1], v[2])) {
			stringstream str;
			str << "Invalid vertex " << v;
			throw parse_error(str.str());
		}
	}
	CHECK(RBRACE, "expecting }");
}

static void parseMaterial(scene& scene) {
	CHECK(LBRACE, "expecting {");
	CHECK(IDENTIFIER, " expecing identifier");
	CHECK(RBRACE, "expecting }");
}

static void parseMesh(scene& scene) {
	CHECK(LBRACE, " Expecting { in mesh");
	
	while(true) {
		int token = bplex_lex();
		switch(token) {
		case VERTICES: parseVertices(scene); break;
		case FACES:    parseFaces(scene); break;
		case NORMALS:  parseNormals(scene); break;
		case MATERIAL: parseMaterial(scene); break;
		case RBRACE:   scene.postProcess(); return;
		default:
			stringstream str;
			str << "expecting vertex/face " << token;
			throw parse_error(str.str());
		}
	}
}

bool parsePov (char *filename, scene& scene) {
	if (bplex_file(filename) == 0)
		return false;

	int token = bplex_lex();
	switch (token) {
	case MESH: parseMesh(scene); break;
	default: throw parse_error("Error while parsing file ");
	};
        return true;
}
