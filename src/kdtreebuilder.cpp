#include "kdtreebuilder.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;

kdtreebuilder::kdtreebuilder(kdtree& kdtree
                             , const scene& scene)
        : _kdtree(kdtree)
        , _scene(scene)
        , _leftchunk(scene.getTriangles().size() * 3)
        , _rightchunk(scene.getTriangles().size() * 3)
{
}

void kdtreebuilder::build(vec3f &lower, vec3f &upper)
{
	auto prims = _leftchunk.create();
        int size   = _scene.getTriangles().size(); 
        _minextend.resize(size);
        _maxextend.resize(size);

	for (int i = 0; i < size; ++i) {
                auto tri = _scene.getTriangles()[i];
                tri.getBounds(_scene, _minextend[i], _maxextend[i]);
                prims.push_back(i);
        }
	aabb bb;
	bb.lower = lower;
	bb.upper = upper;
	recbuild(_kdtree.allocNode(), prims, bb, 1);
	INFO( "kdtree build complete " << size);
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

void kdtreebuilder::findSplit (aabb &voxel, float &split, int &axis) {
        split = (voxel.lower[axis] + voxel.upper[axis]) * 0.5f;
}

void kdtreebuilder::recbuild(nodeid node, chunkmem<int>& triangles, aabb& voxel
			     , int depth)
{
	if (triangles.size() < 20 || depth > 24) {
		_kdtree.initLeaf(node, triangles);
	}
	else {
                int axis;
                float splitf;
                axis = depth % 3;
                findSplit(voxel, splitf, axis);
                splitf = (voxel.lower[axis] + voxel.upper[axis]) * 0.5f;
		aabb lv, rv;

		auto left   = _kdtree.allocNode();
		auto right  = _kdtree.allocNode();
		
		_kdtree.initInternalNode(node, left, axis, splitf);
		split(voxel, axis, splitf, lv, rv);
                
		auto ltris = _leftchunk.create();
                auto rtris = _rightchunk.create();
		//auto& v = _scene.getVertices();
                
                
		for (int i = triangles.begin(); i < triangles.end() ; ++i) {
                        int tid = triangles[i];
                        if (_minextend[tid][axis] <= splitf)
				ltris.push_back(tid);
                        if (_maxextend[tid][axis] >= splitf)
                                rtris.push_back(tid);
                }
                recbuild(left,  ltris, lv, depth + 1);
                recbuild(right, rtris, rv, depth + 1);
        }
        triangles.destroy();
}
