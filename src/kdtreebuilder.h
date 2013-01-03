#ifndef BPRAY_KDTREEBUILDER_H
#define BPRAY_KDTREEBUILDER_H

#include "kdtree.h"
#include "scene.h"
#include <vector>

struct aabb {
	vec3f lower;
	vec3f upper;
};

class kdtreebuilder {

public:
	void build(kdtree& kdtree, scene& scene, vec3f& lower, vec3f& upper);
	void split(aabb& bb, int axis, float split, aabb& left, aabb&right);
	void recbuild(kdtree &kdtree, nodeid node, scene& scene
		      , std::vector<int>& triangles, aabb& voxel
		      , int depth);
};

#endif
