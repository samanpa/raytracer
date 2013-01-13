#include "kdtreebuilder.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;

void kdtreebuilder::build(kdtree& kdtree, scene& scene, vec3f &lower, vec3f &upper) {
	vector<int> prims;
	prims.resize(scene.getTriangles().size());
	for (size_t i = 0; i < scene.getTriangles().size(); ++i)
		prims[i] = i;

	aabb bb;
	bb.lower = lower;
	bb.upper = upper;
	recbuild(kdtree, kdtree.allocNode(), scene, prims, bb, 1);
	INFO( "kdtree build complete ");
}

void kdtreebuilder::split(aabb& voxel, int axis, float split, 
			  aabb& left, aabb &right)
{
	right.upper = voxel.upper;
	right.lower[axis] = split;
	right.lower[modulo3[axis + 1]] = voxel.lower[modulo3[axis + 1]];
	right.lower[modulo3[axis + 2]] = voxel.lower[modulo3[axis + 2]];

	left.lower = voxel.lower;
	left.upper[axis] = split;
	left.upper[modulo3[1 + axis]] = voxel.upper[modulo3[1 + axis]]; 
	left.upper[modulo3[2 + axis]] = voxel.upper[modulo3[2 + axis]]; 
}

void kdtreebuilder::recbuild(kdtree &tree, nodeid node, scene& scene
			     , std::vector<int>& triangles, aabb& voxel
			     , int depth) {
	int axis = depth % 3;
	if (triangles.size() < 20 || depth > 24) {
		tree.initLeaf(node, triangles);
	}
	else {
		auto left   = tree.allocNode();
		auto right  = tree.allocNode();
		float splitf = (voxel.lower[axis] + voxel.upper[axis]) * 0.5f;
		tree.initInternalNode(node, left, axis, splitf);
		aabb lv, rv;
		split(voxel, axis, splitf, lv, rv);

		vector<int> ltris;
		vector<int> rtris;
		auto& v = scene.getVertices();
		for (auto tid : triangles) {
			auto &t = scene.getTriangles()[tid];
			auto &v0 = v[t.p0];
			auto &v1 = v[t.p1];
			auto &v2 = v[t.p2];
			if (triangle::intersect(v0, v1, v2, lv.lower, lv.upper))
				ltris.push_back(tid);
			if (triangle::intersect(v0, v1, v2, rv.lower, rv.upper))
				rtris.push_back(tid);
		}

		recbuild(tree, left,  scene, ltris, lv, depth + 1);
		recbuild(tree, right, scene, rtris, rv, depth + 1);
	}
}
