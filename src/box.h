#ifndef BPRAY_BOX_H
#define BPRAY_BOX_H

#include "ray.h"
#include <algorithm>

class box {
	vec3f bounds[2];
public:
	void init (const vec3f& lower, const vec3f& upper, const vec3f& orig);
	bool intersect(ray &r, float& tmin, float& tmax);
	// Works only if ray in packet is travelling in the same direction
        //    tmin and tmax are unchanged if any ray in bundle misses box
        void intersect(ray4 &r, ssef& tmin, ssef& tmax);

        void clip(ray4 &r, ssef& tmin, ssef& tmax);
};
#endif
