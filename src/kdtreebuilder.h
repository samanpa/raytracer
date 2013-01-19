#ifndef BPRAY_KDTREEBUILDER_H
#define BPRAY_KDTREEBUILDER_H

#include "kdtree.h"
#include "scene.h"
#include <vector>
#include "chunkmem.h"

struct aabb {
	vec3f lower;
	vec3f upper;

        float getArea()
        {
                vec3f d(upper - lower);
                return 2 * (d.x() * d.y() + d.y() * d.z() + d.z() * d.x());
        }
};

class kdtreebuilder {
        kdtree& _kdtree;
        const scene& _scene;
        std::vector<vec3f> _maxextend;
        std::vector<vec3f> _minextend;
        chunkmemmanager<int> _leftchunk;
        chunkmemmanager<int> _rightchunk;
        
        static const int INTERSECT_COST = 30;
        static const int TRAVERSAL_COST = 8;
public:
        kdtreebuilder(kdtree& kdtree, const scene& scene);
        void build(vec3f& lower, vec3f& upper);
        void findSplit (aabb &voxel, float &split, int &axis);
	void split(aabb& bb, int axis, float split, aabb& left, aabb&right);
	void recbuild(nodeid node, chunkmem<int>& triangles, aabb& voxel , int depth);
        
        
        float sah(int nl, int nr, float pl, float pr)
        {
                float mult = 1.0f;
                if ((nl == 0) | (nr == 0))
                        mult = 0.8f;
                return mult * (TRAVERSAL_COST + INTERSECT_COST * (pl * nr + pr * nr));
        }
                
};

#endif
