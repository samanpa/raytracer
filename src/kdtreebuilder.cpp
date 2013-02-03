#include "kdtreebuilder.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;

kdtreebuilder::kdtreebuilder(kdtree& kdtree
                             , const scene& scene)
        : _kdtree(kdtree)
        , _scene(scene)
        , _leftprims(scene.getTriangles().size() * 12)
        , _rightprims(scene.getTriangles().size() * 12)
        , _bothprims(scene.getTriangles().size() * 12)
{
}

void kdtreebuilder::build(vec3f &lower, vec3f &upper)
{
        auto prims = _bothprims.create();
        int size   = _scene.getTriangles().size(); 
        _minextend.resize(size);
        _maxextend.resize(size);

        for (int i = 0; i < size; ++i) {
                auto tri = _scene.getTriangles()[i];
                tri.getBounds(_scene, _minextend[i], _maxextend[i]);
                splitevent sp(i
                              , _minextend[i].x()
                              , X_Axis
                              , splittype::Start);
                prims.push_back(sp);
        }
        aabb bb;
        bb.lower = lower;
        bb.upper = upper;
        recbuild(_kdtree.allocNode(), prims, bb);
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

bool kdtreebuilder::findSplitPlane(aabb &v, chunkmem<splitevent>& events, float &split, int &axis) {
        bool dosplit  = false;
        float mincost = events.size() * INTERSECT_COST * 0.8;
        float rcparea = rcp(v.getArea());
        for (int t = 0; t < 3; ++t) {
                int cnt = min(events.size(), 2);
                float diff = (v.upper[t] - v.lower[t]) / cnt;
                for (int i = 1; i < cnt; ++i) {
                        int nl = 0;
                        int nr = 0;
                        float pos = v.lower[t] + diff* i;
                        for (auto& e : events) {
                                if (_minextend[e.tri][t] <= pos) ++nl;
                                if (_maxextend[e.tri][t] >= pos) ++nr;
                        }
                        aabb l, r;
                        kdtreebuilder::split(v, t, pos, l, r);
                        float cost = sah(nl, nr
                                         , l.getArea()*rcparea, r.getArea()*rcparea);
                        if (cost < mincost) {
                                mincost = cost;
                                split   = pos;
                                axis    = t;
                                dosplit = true;
                        }
                }
        }

        return dosplit;
}

void kdtreebuilder::recbuild(nodeid node, chunkmem<splitevent>& events, aabb& voxel)
{
        int axis;
        float splitf;
        bool isSplit = findSplitPlane(voxel, events, splitf, axis);
        if (isSplit) {
                aabb lv, rv;
                auto left   = _kdtree.allocNode();
                auto right  = _kdtree.allocNode();
                
                _kdtree.initInternalNode(node, left, axis, splitf);
                split(voxel, axis, splitf, lv, rv);
                
                auto ltris = _leftprims.create();
                auto rtris = _rightprims.create();
                
                for (auto& e : events) {
                        if (_minextend[e.tri][axis] <= splitf)
                                ltris.push_back(e);
                        if (_maxextend[e.tri][axis] >= splitf)
                                rtris.push_back(e);
                }
                recbuild(left,  ltris, lv);
                recbuild(right, rtris, rv);
        }
        else {
                _kdtree.initLeaf(node, events.size());
                for (auto& e : events)
                        _kdtree.addPrim(e.tri);
        }
        events.destroy();
}
