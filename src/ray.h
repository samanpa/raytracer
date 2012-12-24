#ifndef BPRAY_RAY_H
#define BPRAY_RAY_H

#include "vec3.h"

class ray {
public:
	vec3& O() { return _orig; }
	vec3& D() { return _dir; }
	unsigned int _prim;
	float tfar;
	float u;
	float v;
private:
	vec3 _orig;
	vec3 _dir;
};

#endif
