// Implementation of the kdtree traversal algorithm described in 
//    Carstein Wachter's diploma thesis


#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;


struct stackitem {
	int node;
	float texit;
};

void kdtree::draw(scene& scene, ray& ray, hit& hit) {
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
                                if (scene._accels[t].pad1 == rayid)
                                        continue;

				scene._accels[t].intersect(t, ray, hit);
                                scene._accels[t].pad1 = rayid;
                        }

			if (ray.tfar < texit) return;
			
			--stackptr;
                        tentry   = texit;
			currNode = stack[stackptr].node + _nodes;
			texit    = stack[stackptr].texit;
		}
	}
}

void kdtree::draw(scene& scene, ray4& r4, hit4& hit4) {
	hit hit[4];
	unsigned int signx = getSign(r4.O().x());
	unsigned int signy = getSign(r4.O().y());
	unsigned int signz = getSign(r4.O().z());

        //If the traversal direction is not same for all rays we 
        //  do a single ray traversal
        if (((signx - 1) < 14)       // sign of x is 0xF or 0
            || ((signy - 1) < 14)    // sign of y is 0xF or 0
            || ((signz - 1) < 14)) { // sign of z is 0xF or 0
                for (int i = 0; i < 4; ++i) {
                        vec3f d(r4.D().x()[i], r4.D().y()[i], r4.D().z()[i]);
                        ray ray(r4.O(), d);
                        hit[i].prim = -1;
                        draw(scene, ray, hit[i]);
                }
                hit4.prim = ssei(hit[0].prim, hit[1].prim, hit[2].prim, hit[3].prim);
                hit4.u    = ssef(hit[0].u, hit[1].u, hit[2].u, hit[3].u);
                hit4.v    = ssef(hit[0].v, hit[1].v, hit[2].v, hit[3].v);
                return;
        }

        ssef tentry(_mm_setzero_ps());
        ssef texit(_mm_setzero_ps());
        //tentry and texit will be unchanged if ray misses the box
        _boundingBox.intersect(r4, tentry, texit);
	
        if (_mm_movemask_ps(tentry == texit) == 0xF)
                return;

        ssef far[MAX_STACK_SIZE];
        int  nodes[MAX_STACK_SIZE];

        //push dummyNode onto stack which will cause us to exit
        nodes[0] = 0;
        far[0]  = BPRAY_INF;

	uint32_t stackptr = 1;
	kdnode* currNode = _nodes + 1;

        static uint64_t rayid = 0;
        ++rayid;
	while (true) {	
                if (!currNode->isLeaf()) {
			int axis  = currNode->getAxis();
                        float dist = currNode->getSplit () - r4.O()[axis];
                        ssef dist4(dist);
                        ssef t = dist4 * r4.rcpD()[axis];
                        unsigned int test = 
                            _mm_movemask_ps(dist4 <= (r4.D()[axis] * tentry));

			int left = currNode->getLeft();
                        // does the ray bundle traverse both cells
                        if ((test - 1) < 14) {
                                test = getSign(dist);
                  
                                far[stackptr] = texit;
                                nodes[stackptr] = left + ( 1 ^ test);
                                ++stackptr;
                                texit = _mm_min_ps(texit, t);
                                currNode = _nodes + left + test;
                        } else { //test is 0xF and 0
                                test  = test & 0x1;
                                ssef cmp((t > tentry) & (t < texit));
                                if (_mm_movemask_ps(cmp) != 0 ) {
                                        far[stackptr] = texit;
                                        nodes[stackptr] = left + ( 1 ^ test);
                                        ++stackptr;
                                        texit = _mm_min_ps(texit, t);
                                }
                                currNode = _nodes + left + test;
                        }                       
                } else {
			int primidx   = currNode->getPrimitiveOffset();
			int primcount = currNode->getNumPrims();


			for (int i = 0; i != primcount; ++i) {
				int t = _prims[primidx + i];

                                //prefetch
                                int t2 = _prims[primidx + i + 1];
                                _mm_prefetch((char*)&scene._accels[t2], _MM_HINT_T0);
                                 //mailboxing
                                if (scene._accels[t].pad1 == rayid)
                                        continue;
				scene._accels[t].intersect(t, r4, hit4);
                                scene._accels[t].pad1 = rayid;
                        }

			if (_mm_movemask_ps(texit < r4.tfar) == 0) return;
                        			
			--stackptr;
                        tentry   = _mm_max_ps(tentry, texit);
			currNode = nodes[stackptr] + _nodes;
			texit    = far[stackptr];
                }
        }
}
