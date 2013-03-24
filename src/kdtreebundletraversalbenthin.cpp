// Implementation of the kdtree traversal algorithm described in 
//    Benthins phd thesis


#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "kdnode.h"
#include "utils.h"

#include "mailbox.h"
mailbox<128> mbox;

#define N 4


template <>
void kdtreebenthin::draw<N>(scene& scene, ray4* r, hit4* h)
{
        unsigned int signx = 0, signy = 0, signz = 0;
        
        for (unsigned i = 0; i < N; ++i)  {
                signx |= movemask(r[i].D().x()) << (4 * i);
                signy |= movemask(r[i].D().y()) << (4 * i);
                signz |= movemask(r[i].D().z()) << (4 * i);
        }

        const unsigned int MASK = (1 << (N * 4)) - 2;// 0xFFFE
        //If the traversal direction is not same for all rays we 
        //  do a single ray traversal
        if (((signx-1) < MASK) || ((signy-1) < MASK) || ((signz-1) < MASK)) { 
                for (int i = 0; i < N; ++i)
                        draw<1>(scene, r+i, h+i);
                return;
        }

        ssef tnear[N];
        ssef tfar[N];
        unsigned count = 0;
        for (unsigned i = 0; i < N; ++i) {
                _boundingBox.clip(*r, tnear[i], tfar[i]);
                if (movemask(tnear >= tfar) == 0xF)
                        ++count;
        }
        
        if (count == N) return;

        const unsigned int dir[3][2] = {
                { signx & 1 , 1 - (signx & 1) },
                { signy & 1 , 1 - (signy & 1) },
                { signz & 1 , 1 - (signz & 1) } };

        ssef far[N][MAX_STACK_SIZE];
        ssef near[N][MAX_STACK_SIZE];
        int  nodes[MAX_STACK_SIZE];

        //push dummyNode onto stack which will cause us to exit
        nodes[0] = 0;
        for (unsigned i = 0; i < N; ++i)
                far[i][0]  = BPRAY_INF;
        uint32_t stackptr = 1;
        kdnode* currNode = _nodes + 1;

        int activemask = 0xFFFF;
        static uint64_t rayid = 0;
        ++rayid;
        while (true) {
                if (!currNode->isLeaf()) {
                        const int k     = currNode->getAxis();
                        const int front = currNode->getLeft() + dir[k][0];
                        const int back  = currNode->getLeft() + dir[k][1];
                        //All rays have the same origin
                        const ssef dist = currNode->getSplit() - r[0].O()[k];
                        unsigned dnear = 0;
                        unsigned dfar  = 0;
                        ssef t[N];

                        for (unsigned i = 0; i < N; ++i) {
                                const int shift = 4 * i;
                                t[i]   = dist * r[i].rcpD()[k];
                                dnear |= (movemask(tnear[i] <= t[i]) << shift);
                                dfar  |= (movemask(tfar[i]  >= t[i]) << shift);
                        }
                        currNode   = _nodes + back;
                        if ((activemask & dnear) == 0) continue;

                        currNode   = _nodes + front;
                        if ((activemask & dfar) == 0)  continue;

                        unsigned newmask = 0;
                        for (unsigned i = 0; i < N; ++i) {
                                const int shift = 4 * i;
                                near[i][stackptr] = max(tnear[i], t[i]);
                                far[i][stackptr]  = tfar[i]; 
                                tfar[i] = min(tfar[i], t[i]);
                                newmask |= (movemask(tnear[i] <= tfar[i])
                                               << shift);
                        }
                        activemask &= newmask;
                        nodes[stackptr] = back;
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
                                for (unsigned j = 0; j < N; ++j) {
                                        scene.intersect(t, r[j], h[j]);
                                }
                                mbox.add(scene, rayid, t);
                        }
                        
                        int termination = 0;
                        for (unsigned i = 0; i < N; ++i)
                                termination |= (movemask(tfar[i] < r[i].tfar)
                                                << (4*i));
                        if (termination == 0) return;
                        
                        --stackptr;
                        currNode   = nodes[stackptr] + _nodes;
                        activemask = 0;
                        for (unsigned i = 0; i < N; ++i) {
                                tfar[i]  = far[i][stackptr];
                                tnear[i] = near[i][stackptr];
                                ssef nlef = tnear[i] <= tfar[i];
                                activemask |= movemask(nlef) << (4*i);
                        }
                        activemask &= termination;
                }
        }
}
