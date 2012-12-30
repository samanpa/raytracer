#ifndef BPRAY_BOX_H
#define BPRAY_BOX_H

#include "ray.h"
#include <iostream>
#include <algorithm>

class box {
	vec3f bounds[2];
public:
	void init (vec3f& lower, vec3f& upper, vec3f& orig) {
		//translate bounds to the orig so we don't
		// do it every time we intersect
		bounds[0] = lower - orig;
		bounds[1] = upper - orig;
	}

	bool intersect(ray &r, float& tmin, float& tmax) {
		float tymax, tymin, tzmax, tzmin;
		int signx = (0x80000000 & (int&)r.rcpD()[X_Axis]) >> 31;
		int signy = (0x80000000 & (int&)r.rcpD()[Y_Axis]) >> 31;
		int signz = (0x80000000 & (int&)r.rcpD()[Z_Axis]) >> 31;

		tmin  = bounds[    signx].x() * r.rcpD().x();
		tymax = bounds[1 ^ signy].y() * r.rcpD().y();
		if (tmin > tymax) return false;

		tymin = bounds[    signy].y() * r.rcpD().y();	
		tmax  = bounds[1 ^ signx].x() * r.rcpD().x();
		if (tymin > tmax) return false;

		if (tymax < tmax) tmax = tymax;
		tzmin = bounds[signz].z() * r.rcpD().z();
		if (tzmin > tmax) return false;

		if (tymin > tmin) tmin = tymin;
		tzmax = bounds[1 ^ signz].z() * r.rcpD().z();
		if (tmin > tzmax) return false;
		
		if (tzmax < tmax) tmax = tzmax;
		if (tmax < 0.0f) return false;

		if (tzmin > tmin) tmin = tzmin;

		return true;
	}
		
};
#endif
