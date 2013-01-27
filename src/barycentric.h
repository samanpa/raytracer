#ifndef BPRAY_TRIANGLE_BARYCENTRIC_H
#define BPRAY_TRIANGLE_BARYCENTRIC_H

#include "ray.h"

struct triangle_barycentric {
	triangle_barycentric (vec3f &v0, vec3f &v1, vec3f &v2);
	int   k;  //projection plane
	float p0d;//p0 . N
	float p0v;//p0 projection in v
	float p0w;//p0 projection in w
	float nv; //projected normal
	float nw;
	float e1v; //projected e1 (p1 - p0)
	float e1w;
	float e2v; //projected e2 (p2 - p0);
	float e2w;
	int   pad1;
	int   pad2; //pad to 12 words   

	void intersect(unsigned int primId, ray& ray, hit& hit);
	void intersect(unsigned int primId, ray4& ray, hit4& hit);
};

#endif
