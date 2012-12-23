#ifndef BPRAY_TRIANGLE_H
#define BPRAY_TRIANGLE_H

#include "ray.h"

class triangle {
public:
	triangle(vec3 &v0, vec3 &v1, vec3 &v2);

	bool intersect(ray& ray);
private:
	vec3  _p0;
	float _nu;
	float _nv;
	int   k_plane; //projection plane
	float _area;
	float _au;
	float _av;
	float _bu;
	float _bv;
};

#endif
