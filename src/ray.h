#ifndef BPRAY_RAY_H
#define BPRAY_RAY_H

#include "vec3.h"
#include <immintrin.h>
#include <simd/simd.h>

struct hit {
	unsigned int prim;
	float tfar;
	float u;
	float v;
};

struct hit4 {
 	ssei prim;
	ssef tfar;
	ssef u;
	ssef v;
};

template<typename T>
class ray {
public:
	vec3<T>& O() { return _orig; }
	vec3<T>& D() { return _dir; }
private:
	vec3<T> _orig;
	vec3<T> _dir;
};

typedef ray<ssef> ray4;

#endif
