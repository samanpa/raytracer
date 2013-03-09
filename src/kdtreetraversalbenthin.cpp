// Implementation of the kdtree traversal algorithm described in 
//    Benthins phd thesis


#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;

#define MBOX 3
#include "mailbox.h"
mailbox<128> mbox;

template <>
void kdtreebenthin::draw<1>(scene& scene, ray4* r, hit4* hit4)
{
        unsigned int signx = movemask(r->D().x());
        unsigned int signy = movemask(r->D().y());
        unsigned int signz = movemask(r->D().z());

        //If the traversal direction is not same for all rays we 
        //  do a single ray traversal
        if (((signx - 1) < 14)       // sign of x is 0xF or 0
            || ((signy - 1) < 14)    // sign of y is 0xF or 0
            || ((signz - 1) < 14)) { // sign of z is 0xF or 0
                hit hit[4];
                for (int i = 0; i < 4; ++i) {
                        vec3f d(r->D().x()[i], r->D().y()[i], r->D().z()[i]);
                        ray ray(r->O(), d);
                        hit[i].prim = -1;
                        draw(scene, ray, hit[i]);
                }
                hit4->prim = ssei(hit[0].prim, hit[1].prim, hit[2].prim, hit[3].prim);
                hit4->u    = ssef(hit[0].u, hit[1].u, hit[2].u, hit[3].u);
                hit4->v    = ssef(hit[0].v, hit[1].v, hit[2].v, hit[3].v);
                return;
        }

        ssef tentry(_mm_setzero_ps());
        ssef texit(_mm_setzero_ps());
        //tentry and texit will be unchanged if ray misses the box
        _boundingBox.intersect(*r, tentry, texit);
        
        if (movemask(tentry == texit) == 0xF)
                return;
        const unsigned int ray_dir[3][2] = {
                { signx & 1 , 1 - (signx & 1) },
                { signy & 1 , 1 - (signy & 1) },
                { signz & 1 , 1 - (signz & 1) } };

        ssef far[MAX_STACK_SIZE];
        ssef near[MAX_STACK_SIZE];
        int  nodes[MAX_STACK_SIZE];

        //push dummyNode onto stack which will cause us to exit
        nodes[0] = 0;
        far[0]  = BPRAY_INF;

        uint32_t stackptr = 1;
        kdnode* currNode = _nodes + 1;

        int activemask = 0xF;
        static uint64_t rayid = 0;
        ++rayid;
        while (true) {        
                if (!currNode->isLeaf()) {
                        int axis   = currNode->getAxis();
                        int back   = currNode->getLeft() + ray_dir[axis][1];
                        int front  = currNode->getLeft() + ray_dir[axis][0];
                        ssef dist  = currNode->getSplit() - r->O()[axis];
                        ssef t     = dist * r->rcpD()[axis];

                        currNode   = _nodes + back;
                        if (!(movemask(tentry <= t) & activemask)) continue;

                        currNode   = _nodes + front;
                        if (!(movemask(texit >= t) & activemask))  continue;

                        nodes[stackptr] = back;
                        near[stackptr]  = _mm_max_ps(tentry, t);
                        far[stackptr]   = texit;
                        texit           = _mm_min_ps(texit, t);
                        activemask     &= _mm_movemask_ps(_mm_cmple_ps(tentry, texit));
                        ++stackptr;
                } else {
                        int primidx   = currNode->getPrimitiveOffset();
                        int primcount = currNode->getNumPrims();

                        for (int i = 0; i != primcount; ++i) {
                                int t = _prims[primidx + i];

                                //prefetch
                                int t2 = _prims[primidx + i + 1];
                                _mm_prefetch((char*)&scene._accels[t2], _MM_HINT_T0);
                                //mailboxing
                                if (mbox.find(scene, rayid, t)) continue;
                                scene._accels[t].intersect(t, *r, *hit4);
                                mbox.add(scene, rayid, t);
                        }

                        if (movemask(texit < r->tfar) == 0) return;
                        
                        --stackptr;
                        currNode   = nodes[stackptr] + _nodes;
                        texit      = far[stackptr];
                        tentry     = near[stackptr];
                        activemask = movemask(tentry <= texit);
                }
        }
}

