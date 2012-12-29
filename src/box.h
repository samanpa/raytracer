#ifndef BPRAY_BOX_H
#define BPRAY_BOX_H

#include "ray.h"
#include <iostream>

struct box {
	vec3f bounds[2];

	void init (vec3f& lower, vec3f& upper) {
		bounds[0] = lower;
		bounds[1] = upper;
		
		std::cout << lower << upper << std::endl;
	}

	bool intersect(ray &r) {
		float tmin =  (bounds[r.signx].x() - r.O().x()) * r.rcpD().x();
		float tymin = (bounds[r.signy].y() - r.O().y()) * r.rcpD().y();
		float tmax =  (bounds[1 - r.signx].x() - r.O().x()) * r.rcpD().x();
		float tymax = (bounds[1 - r.signy].y() - r.O().y()) * r.rcpD().y();
		
		if (tmin > tymax) return false;
		if (tymin > tmax) return false;

		if (tymin > tmin) tmin = tymin;
		if (tymax < tmax) tmax = tymax;
		
		float tzmin = (bounds[r.signz].z() - r.O().z()) * r.rcpD().z();
		float tzmax = (bounds[1 - r.signz].z() - r.O().z()) * r.rcpD().z();

		if ((tmin > tzmax) || (tzmin > tmax)) return false;
		if (tzmin > tmin) tmin = tzmin;
		if (tzmax < tmax) tmax = tzmax;

		return true;
	}
		
};
#endif
