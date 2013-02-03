#include "kdtreebuilder.h"
#include "kdnode.h"
#include "utils.h"
#include <algorithm>
#include <unordered_set>

using namespace std;

kdtreebuilder::kdtreebuilder(kdtree& kdtree
                             , const scene& scene)
        : _kdtree(kdtree)
        , _scene(scene)
        , _levmgr(scene.getTriangles().size() * 12)
        , _revmgr(scene.getTriangles().size() * 12)
        , _bevmgr(scene.getTriangles().size() * 12)
{
        _sides.resize(scene.getTriangles().size());
}

void kdtreebuilder::build(vec3f &lower, vec3f &upper)
{
        auto bevents = _bevmgr.create();
        size_t size  = _scene.getTriangles().size(); 

        for (size_t i = 0; i < size; ++i) {
                auto& tri = _scene.getTriangle(i);
                vec3f min, max;
                tri.getBounds(_scene, min, max);
                for (int k = 0; k < 3; ++k) {
                        if (min[k] == max[k]) {
                                splitevent sp(i , min[k], k, splittype::Planar);
                                bevents.push_back(sp);
                        }
                        else {
                                splitevent sp1(i , min[k], k, splittype::Start);
                                splitevent sp2(i , max[k], k, splittype::End);
                                bevents.push_back(sp1);
                                bevents.push_back(sp2);
                        }
                }
        }
        aabb bb;
        bb.lower = lower;
        bb.upper = upper;
        std::sort(bevents.begin(), bevents.end());

        recbuild(_kdtree.allocNode(), bevents, bb, size);
        INFO( "kdtree build complete " << size);
}

static void splitVoxel(const aabb& voxel, int axis, float split, aabb& left, aabb &right)
{
        left.lower = voxel.lower;
        right.upper = voxel.upper;
        left.upper[axis] = split;
        right.lower[axis] = split;
        left.upper[modulo3[1 + axis]]  = voxel.upper[modulo3[1 + axis]]; 
        right.lower[modulo3[axis + 1]] = voxel.lower[modulo3[axis + 1]];
        left.upper[modulo3[2 + axis]]  = voxel.upper[modulo3[2 + axis]]; 
        right.lower[modulo3[axis + 2]] = voxel.lower[modulo3[axis + 2]];
}

static const int INTERSECT_COST = 50;
static const int TRAVERSAL_COST = 30;

static void sah(int nl, int nr, int np, float pl, float pr, float& cost, splitside &side)
{
        float lambda = ((nl == 0) | (nr == 0)) ? 0.8f : 1.0f;
        float lcost = lambda * (TRAVERSAL_COST + INTERSECT_COST * (pl * (nl + np) + pr * nr));
        float rcost = lambda * (TRAVERSAL_COST + INTERSECT_COST * (pl * nl + pr * (nr + np)));

        if (lcost < rcost) {
                cost = lcost;
                side = splitside::Left;
        }
        else {
                cost = rcost;
                side = splitside::Right;
        }
}

bool kdtreebuilder::findPlane(const aabb &v
                              , const chunkmem<splitevent>& events, split &split, size_t numtris)
{
        bool dosplit  = false;
        float mincost = numtris * INTERSECT_COST;
        float rcparea = rcp(v.getArea());
        vec3<size_t> nleft(0, 0, 0);
        vec3<size_t> nright(numtris, numtris, numtris);

        auto end = events.end();
        auto curr = events.begin();
        while (curr < end) {
                int k = curr->axis;
                float split_k = curr->pos;
                int np = 0, nend = 0, nstart = 0;

                while (curr < end && curr->pos == split_k 
                       && curr->axis == k && curr->type == splittype::End) {
                        ++curr; ++nend;
                }               
                while (curr < end && curr->pos == split_k
                       && curr->axis == k && curr->type == splittype::Planar) {
                        ++curr; ++np;
                }
                while (curr < end && curr->pos == split_k
                       && curr->axis == k && curr->type == splittype::Start) {
                        ++curr; ++nstart;
                }
              
                aabb lv, rv;
                splitVoxel(v, k, split_k, lv, rv);

                nright[k] -= (np + nend);
                float cost;
                splitside side;
                sah(nleft[k], nright[k],  np, v.getArea()*rcparea, rv.getArea()*rcparea, cost, side);
                nleft[k] += (nstart + np);

                if (cost < mincost 
                    && v.lower[k] < split_k
                    && v.upper[k] > split_k) {
                        split.pos  = split_k;
                        split.side = side;
                        split.lv   = lv;
                        split.rv   = rv;
                        split.axis = k;

                        mincost = cost;
                        dosplit = true;
                }
        }
        return dosplit;
}

void kdtreebuilder::classify(eventlist& allevents, split &split
                             , eventlist& left, eventlist& right
                             , size_t& nl, size_t& nr) 
{
        nl = nr = 0;
        for (auto& e : allevents) 
                _sides[e.tri] = splitside::Both;

        for (auto& e : allevents) {
                if (e.axis != split.axis) continue;
                switch (e.type) {
                case splittype::End:
                        if (e.pos <= split.pos)
                                _sides[e.tri] = splitside::Left;
                        break;
                case splittype::Start:
                        if (e.pos >= split.pos)
                                _sides[e.tri] = splitside::Right;
                        break;
                default:
                        if (e.pos == split.pos)
                                _sides[e.tri] = split.side;
                        else if (e.pos < split.pos)
                                _sides[e.tri] = splitside::Left;
                        else if (e.pos > split.pos)
                                _sides[e.tri] = splitside::Right;
                        break;
                }
        }

        auto both = _bevmgr.create();
        for (auto& e : allevents) {
                switch(_sides[e.tri]) {
                case splitside::Left:  left.push_back(e); break;
                case splitside::Right: right.push_back(e); break;
                case splitside::Both:  both.push_back(e);  break;
                default: break;
                }
        }

        for (auto& e : allevents) {
                switch(_sides[e.tri]) {
                case splitside::Left:  ++nl; break;
                case splitside::Right: ++nr; break;
                case splitside::Both:  ++nl; ++nr; break;
                default: break;
                }
                _sides[e.tri] = splitside::Undef;
        }

        auto onlyleftsize = left.size();
        auto onlyrightsize = right.size();

        for (auto& e : both) {
                if (e.axis != split.axis)
                        left.push_back(e);
                else if (e.type == splittype::Planar) {
                        INFO("messed up " << e.pos);
                        abort();
                }
                else if (e.type == splittype::Start) {
                        splitevent se(e.tri, split.pos, split.axis, splittype::End);
                        left.push_back(e);
                        left.push_back(se);
                }
        }
        sort(left.begin() + onlyleftsize, left.end());

        for (auto& e : both) {
                if (e.axis != split.axis)
                        right.push_back(e);
                else if (e.type == splittype::Planar) {
                        INFO("messed up " << e.pos);
                        abort();
                }
                else if (e.type == splittype::End) {
                        splitevent se(e.tri, split.pos, split.axis, splittype::Start);
                        right.push_back(se);
                        right.push_back(e);
                }
        }
        sort(right.begin() + onlyrightsize, right.end());

        inplace_merge(left.begin(), left.begin() + onlyleftsize, left.end());
        inplace_merge(right.begin(), right.begin() + onlyrightsize, right.end());

        both.destroy();
}

void kdtreebuilder::recbuild(nodeid node, chunkmem<splitevent>& events, aabb& voxel, size_t numtris)
{
        split split;
        bool isSplit = findPlane(voxel, events, split, numtris);
        if (isSplit) {
                auto left   = _kdtree.allocNode();
                auto right  = _kdtree.allocNode();
                
                _kdtree.initInternalNode(node, left, split.axis, split.pos);
               
                auto levents = _levmgr.create();
                auto revents = _revmgr.create();
                size_t nl, nr;
                classify(events, split, levents, revents, nl, nr);

                recbuild(left,  levents, split.lv, nl);
                recbuild(right, revents, split.rv, nr);
        }
        else {
                unordered_set<int> tris;
                for (auto& e : events)
                        tris.insert(e.tri);
                _kdtree.initLeaf(node, tris.size());
                for (auto& tri : tris)
                        _kdtree.addPrim(tri);
        }
        events.destroy();
}
