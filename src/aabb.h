#ifndef BPRAY_AABB_H
#define BPRAY_AABB_H

#include "vec3.h"
#include "utils.h"

struct aabb {
	vec3f lower;
	vec3f upper;
};

#endif
