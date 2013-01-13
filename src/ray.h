#ifndef BPRAY_RAY_H
#define BPRAY_RAY_H

#include "vec3.h"
#include <immintrin.h>
#include "simd/simd.h"
#include "float.h"

struct hit {
	int prim;
	float u;
	float v;
};

struct hit4 {
 	ssei prim;
	ssef u;
	ssef v;
};

template <typename T>
class rayt {
public:
        rayt(const vec3f& o, vec3<T>& d) :
		tfar(BIG_FLOAT),
		_orig(o),
		_dir(d) {
		normalize(_dir);
		_invdir = rcp(_dir);
	}

	const vec3f& O() const { return _orig; }
	const vec3<T>& D() const { return _dir; }
	const vec3<T>& rcpD() const { return _invdir; }
	T tfar;
private:
        vec3f _orig;
        vec3<T> _dir;
        vec3<T> _invdir;
};

typedef rayt<float> ray;
typedef rayt<ssef> ray4;
#endif
