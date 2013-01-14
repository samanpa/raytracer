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
	float e1u; //projected e1 (p1 - p0)
	float e1v;
	float e2u; //projected e2 (p2 - p0);
	float e2v;
	int   pad1;
	int   pad2; //pad to 12 words   

	void intersect(unsigned int primId, ray& ray, hit& hit);
	void intersect(unsigned int primId, ray4& ray, hit4& hit);
};

#endif
