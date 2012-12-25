#ifndef BPRAY_TRIANGLE_BARYCENTRIC_H
#define BPRAY_TRIANGLE_BARYCENTRIC_H

#include "ray.h"

struct triangle_barycentric {
	triangle_barycentric (vec3 &v0, vec3 &v1, vec3 &v2);
	int   k;  //projection plane
	vec3  p0;
	float nu; //projected normal
	float nv;
	float au; //projected a (p1 - p0)
	float av;
	float bu; //projected b (p2 - p0);
	float bv;
	int   pad1;
	int   pad2; //pad to 12 words   

	bool intersect(unsigned int primId, ray& ray);
};

#endif
