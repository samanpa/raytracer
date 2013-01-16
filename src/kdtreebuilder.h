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
        std::vector<vec3f> _maxextend;
        std::vector<vec3f> _minextend;

public:
	void build(kdtree& kdtree, const scene& scene, vec3f& lower, vec3f& upper);
	void split(aabb& bb, int axis, float split, aabb& left, aabb&right);
	void recbuild(kdtree &kdtree, nodeid node, const scene& scene
		      , std::vector<int>& triangles, aabb& voxel , int depth);
};

#endif
