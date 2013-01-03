#ifndef BPRAY_BOX_H
#define BPRAY_BOX_H

#include "ray.h"
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

	bool intersect(ray &r, float& tmin, float& tmax);
};
#endif
