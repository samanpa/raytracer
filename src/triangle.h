#ifndef BPRAY_TRIANGLE_H
#define BPRAY_TRIANGLE_H

#include "simd/simd.h"
#include "aabb.h"

class scene;

class triangle {
public:
	triangle(int p0, int p1, int p2)
		: p0(p0), p1(p1), p2(p2)
		{}
	
        void getBounds(const scene& scene, aabb &bb) const;
        
        static
        ssef getSmoothNormal(uint32_t t, const scene& scene, float v, float w);

	const int p0;
	const int p1;
	const int p2;
};

void clip(const scene &scene, int tid
          , const aabb& voxel, aabb& leftv, aabb &rightv, float splitf, int axis);

#endif
