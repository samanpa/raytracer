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

enum class splittype : char{
        Start,
        End,
        Planar        
};

struct splitevent {
        int tri;
        float pos;
        char axis;
        splittype type;
        splitevent(int tri, float pos, char axis, splittype type) :
                tri(tri), pos(pos), axis(axis), type(type) {}
};

class kdtreebuilder {
        kdtree& _kdtree;
        const scene& _scene;
        std::vector<vec3f> _maxextend;
        std::vector<vec3f> _minextend;
        chunkmemmanager<splitevent> _leftchunk;
        chunkmemmanager<splitevent> _rightchunk;
        chunkmemmanager<splitevent> _bothchunk;
        
        static const int INTERSECT_COST = 50;
        static const int TRAVERSAL_COST = 30;
public:
        kdtreebuilder(kdtree& kdtree, const scene& scene);
        void build(vec3f& lower, vec3f& upper);
        float findSplit (aabb &voxel, chunkmem<splitevent>& tri, float &split, int &axis);
	static void split(aabb& bb, int axis, float split, aabb& left, aabb&right);
	void recbuild(nodeid node, chunkmem<splitevent>& triangles, aabb& voxel);
        
        static float sah(int nl, int nr, float pl, float pr)
        {
                float mult = 1.0f;
                if ((nl == 0) | (nr == 0))
                        mult = 0.8f;
                return mult * (TRAVERSAL_COST + INTERSECT_COST * (pl * nl + pr * nr));
        }
                
};

#endif
