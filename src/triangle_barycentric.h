#ifndef BPRAY_TRIANGLE_BARYCENTRIC_H
#define BPRAY_TRIANGLE_BARYCENTRIC_H

#include "ray.h"

struct triangle_barycentric {
	triangle_barycentric (vec3 &v0, vec3 &v1, vec3 &v2);
	int   k; //projection plane
	vec3  p0;
	float nu;
	float nv;
	float area;
	float au;
	float av;
	float bu;
	float bv;

	bool intersect(unsigned int primId, ray& ray);
};

#endif
