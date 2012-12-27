#ifndef BPRAY_TRIANGLE_BARYCENTRIC_H
#define BPRAY_TRIANGLE_BARYCENTRIC_H

#include "ray.h"

struct triangle_barycentric {
	triangle_barycentric (vec3f &v0, vec3f &v1, vec3f &v2);
	int   k;  //projection plane
	float p0d;//p0 . N
	float p0u;//p0 projection in u
	float p0v;//p0 projection in v
	float nu; //projected normal
	float nv;
	float au; //projected a (p1 - p0)
	float av;
	float bu; //projected b (p2 - p0);
	float bv;
	int   pad1;
	int   pad2; //pad to 12 words   

	void intersect(unsigned int primId, ray<float>& ray, hit& hit);
	void intersect(unsigned int primId, ray<ssef>& ray, hit4& hit);
};

#endif
