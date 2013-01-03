#ifndef BPRAY_TRIANGLE_H
#define BPRAY_TRIANGLE_H

#include <vec3.h>

class triangle {
public:
	triangle(int p0, int p1, int p2)
		: p0(p0), p1(p1), p2(p2)
		{}
	
	static bool intersect(const vec3f &p0, const vec3f &p1, const vec3f &p2,
			      const vec3f &lower, const vec3f &upper);
	int p0;
	int p1;
	int p2;
};

#endif
