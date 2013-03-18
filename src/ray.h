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
        hit() : prim(-1){}
};

struct hit4 {
 	ssei prim;
	ssef u;
	ssef v;
        hit4() : prim(-1){}
};

template <typename T>
class rayt {
public:
        rayt(){}

        //d must be normalized
        rayt(const vec3f& o, const vec3<T>& d) {
                init(o, d);
	}
        
        void init(const vec3f& o, const vec3<T>& d)
        {
		tfar  = BIG_FLOAT;
		_orig = o;
		_dir  = d;
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
