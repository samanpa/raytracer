#ifndef BPRAY_TRIANGLE_BARYCENTRIC_H
#define BPRAY_TRIANGLE_BARYCENTRIC_H

#include "ray.h"

struct triangle_barycentric {
	triangle_barycentric (vec3f &v0, vec3f &v1, vec3f &v2);
	int   k;    //projection plane
	float p2d;  //v2 . N
	float p2u;  //v2 projection in u
	float p2v;  //v2 projection in v
	float nu;   //projected normal
	float nv;
	float e0u;  //projected e0 (v0 - v2)
	float e0v;
	float e1u;  //projected e1 (v1 - v2);
	float e1v;
	int   pad1;
	int   pad2; //pad to 12 words   

	void intersect(unsigned int primId, ray& ray, hit& hit);
	void intersect(unsigned int primId, ray4& ray, hit4& hit);
};

#endif
