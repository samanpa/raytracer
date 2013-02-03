#ifndef BPRAY_KDTREEBUILDER_H
#define BPRAY_KDTREEBUILDER_H

#include "kdtree.h"
#include "scene.h"
#include <vector>
#include "chunkmem.h"
#include "aabb.h"

enum class splittype : char{
        End,
        Planar,
        Start       
};

struct splitevent {
        int tri;
        float pos;
        char axis;
        splittype type;
        bool operator < (const splitevent& s) {
                if (s.pos < pos) return true;
                if (s.pos == pos) return (int)type < (int)s.type;
                return false;
        }

        splitevent(int tri, float pos, char axis, splittype type) :
                tri(tri), pos(pos), axis(axis), type(type) {}
};

class kdtreebuilder {
        kdtree& _kdtree;
        const scene& _scene;
        std::vector<vec3f> _maxextend;
        std::vector<vec3f> _minextend;
        chunkmemmanager<splitevent> _leftprims;
        chunkmemmanager<splitevent> _rightprims;
        chunkmemmanager<splitevent> _bothprims;
        
        static const int INTERSECT_COST = 50;
        static const int TRAVERSAL_COST = 30;

        bool findSplitPlane (aabb &voxel, chunkmem<splitevent>& tri, float &split, int &axis);
	static void split(aabb& bb, int axis, float split, aabb& left, aabb&right);
	void recbuild(nodeid node, chunkmem<splitevent>& triangles, aabb& voxel);
        
        static float sah(int nl, int nr, float pl, float pr)
        {
                float mult = 1.0f;
                if ((nl == 0) | (nr == 0))
                        mult = 0.8f;
                return mult * (TRAVERSAL_COST + INTERSECT_COST * (pl * nl + pr * nr));
        }

public:
        kdtreebuilder(kdtree& kdtree, const scene& scene);
        void build(vec3f& lower, vec3f& upper);              
};

#endif
