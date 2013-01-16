#ifndef BPRAY_TRIANGLE_H
#define BPRAY_TRIANGLE_H

#include <vec3.h>

class scene;

class triangle {
public:
	triangle(int p0, int p1, int p2)
		: p0(p0), p1(p1), p2(p2)
		{}
	
        void getBounds(const scene& scene, vec3f &min, vec3f &max) const;

	const int p0;
	const int p1;
	const int p2;
};

#endif
