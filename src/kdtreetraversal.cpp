// Implementation of the kdtree traversal algorithm described in 
//    Carstein Wachter's diploma thesis

#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "kdnode.h"
#include "utils.h"
#include "mailbox.h"

static mailbox<128> mbox;

using namespace std;


struct stackitem {
        int node;
        float texit;
};

void kdtree::draw(scene& scene, ray& ray, hit& hit)
{
       float tentry, texit;
       if (!_boundingBox.intersect(ray, tentry, texit))
                return;
        ray.tfar = texit;
        if (tentry < 0.f) tentry = 0.f;
        stackitem stack[MAX_STACK_SIZE];
        //push dummy element
        stack[0].node  = 0;
        stack[0].texit = BPRAY_INF;
        uint32_t stackptr = 1;
        kdnode* currNode = _nodes + 1;
        static uint64_t rayid = 0;
        ++rayid;
        while (true) {        
                if (!currNode->isLeaf()) {
                        int axis = currNode->getAxis();
                        float dist = currNode->getSplit() - ray.O()[axis];
                        int left = currNode->getLeft();
                        if (dist <= tentry * ray.D()[axis]) { // right node traversed first
                                if (dist >= texit * ray.D()[axis]) {
                                        stack[stackptr].node  = left;
                                        stack[stackptr].texit = texit;
                                        ++stackptr;
                                        texit = dist * ray.rcpD()[axis];
                                }
                                currNode = _nodes + left + 1;
                        }
                        else { //left node traversed first
                                if (dist <= texit * ray.D()[axis]) { // exit in right
                                        stack[stackptr].node  = left + 1;
                                        stack[stackptr].texit = texit;
                                        ++stackptr;
                                        texit = dist * ray.rcpD()[axis];
                                }
                                currNode = _nodes + left;
                        }
                }
                else {
                        int primidx   = currNode->getPrimitiveOffset();
                        int primcount = currNode->getNumPrims();

                        for (int i = 0; i != primcount; ++i) {
                                int t = _prims[primidx + i];
                                //prefetch
                                int t2 = _prims[primidx + i + 1];
                                _mm_prefetch((char*)&scene._accels[t2], _MM_HINT_T0);

                                //mailboxing
                                if (mbox.find(scene, rayid, t)) continue;
                                scene.intersect(t, ray, hit);
                                mbox.add(scene, rayid, t);
                        }

                        if (ray.tfar < texit) return;
                        
                        --stackptr;
                        tentry   = texit;
                        currNode = stack[stackptr].node + _nodes;
                        texit    = stack[stackptr].texit;
                }
        }
}
