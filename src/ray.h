#ifndef BPRAY_RAY_H
#define BPRAY_RAY_H

#include "vec3.h"
#include <immintrin.h>
#include <simd/simd.h>

struct hit {
	unsigned int prim;
	float u;
	float v;
};

struct hit4 {
 	ssei prim;
	ssef tfar;
	ssef u;
	ssef v;
};

class ray {
public:
	ray(vec3f& o, vec3f& d) {
		tfar = 3.3e38f;
		_orig = o;
		_dir = d;
		normalize(_dir);
		_invdir = rcp(_dir);
		signx = (0x80000000 & (int&)_dir[X_Axis]) >> 31;
		signy = (0x80000000 & (int&)_dir[Y_Axis]) >> 31;
		signz = (0x80000000 & (int&)_dir[Z_Axis]) >> 31;
	}

	const vec3f& O() const { return _orig; }
	const vec3f& D() const { return _dir; }
	const vec3f& rcpD() const { return _invdir; }
	float tfar;
private:
	vec3f _orig;
	vec3f _dir;
	vec3f _invdir;
public:
	int signx;
	int signy;
	int signz;
};

#endif
