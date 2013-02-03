#ifndef BPRAY_KDTREEBUILDER_H
#define BPRAY_KDTREEBUILDER_H

#include "kdtree.h"
#include "scene.h"
#include <vector>
#include "chunkmem.h"
#include "aabb.h"
#include <iomanip>

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
        bool operator < (const splitevent& s) const {
                if (pos < s.pos) return true;
                if (pos == s.pos) {
                        if (axis < s.axis) return true;
                        if (axis > s.axis) return false;
                        bool res = ((int)type < (int)s.type);
                        return res;
                }
                return false;
        }

        splitevent(int tri, float pos, char axis, splittype type) :
                tri(tri), pos(pos), axis(axis), type(type) {}
};

enum class splitside : char {
        Left, 
        Right,
        Both,
        Undef
};

struct split {
        aabb lv;
        float pos;
        aabb rv;
        char axis;
        splitside side;
};

class kdtreebuilder {
        kdtree& _kdtree;
        const scene& _scene;
        chunkmemmanager<splitevent> _levmgr;
        chunkmemmanager<splitevent> _revmgr;
        chunkmemmanager<splitevent> _bevmgr;
        std::vector<splitside> _sides;
        
        typedef chunkmem<splitevent> eventlist;
        bool findPlane (const aabb &voxel, const eventlist& events, split &split, size_t numTris);
	void recbuild(nodeid node, eventlist& triangles, aabb& voxel, size_t numTris);
        void classify(eventlist& allevents, split &split
                      , eventlist& left, eventlist& right
                      , size_t &nl, size_t &nr);
public:
        kdtreebuilder(kdtree& kdtree, const scene& scene);
        void build(vec3f& lower, vec3f& upper);              
};

#endif
