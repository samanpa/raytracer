#ifndef BPRAY_AABB_H
#define BPRAY_AABB_H

#include "vec3.h"
#include "utils.h"

struct aabb {
	vec3f lower;
	vec3f upper;

        float getArea() const
        {
                vec3f d(upper - lower);
                return 2 * (d.y() *(d.x() + d.z()) + d.z() * d.x());
        }
};

#endif
