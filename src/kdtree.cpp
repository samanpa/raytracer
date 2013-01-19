#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "stdlib.h"
#include "kdtreebuilder.h"
#include "kdnode.h"
#include "utils.h"

using namespace std;

kdtree::kdtree(scene &scene)
	: _nodes((kdnode*)calloc(128, sizeof(kdnode)))
	, _numNodes(0)
	, _maxNodes(128)
{
	vec3f lower (BIG_FLOAT, BIG_FLOAT, BIG_FLOAT);
	vec3f upper (NBIG_FLOAT, NBIG_FLOAT, NBIG_FLOAT);
	
	auto& vertices  = scene.getVertices();
	for (auto& v : vertices) {
		lower.x() = std::min(v.x(), lower.x());
		lower.y() = std::min(v.y(), lower.y());
		lower.z() = std::min(v.z(), lower.z());
		upper.x() = std::max(v.x(), upper.x());
		upper.y() = std::max(v.y(), upper.y());
		upper.z() = std::max(v.z(), upper.z());
	}
	INFO( "bounding box " << lower << " " << upper);
	_boundingBox.init(lower, upper, scene.getCamera().getLocation());
	kdtreebuilder builder(*this, scene);
	builder.build(lower, upper);
}

nodeid kdtree::allocNode() {
	if (_numNodes == _maxNodes) {
		_maxNodes *= 2;
		_nodes = (kdnode*)realloc(_nodes, _maxNodes * sizeof(kdnode));
	}
	return _numNodes++;
}

void kdtree::initLeaf(nodeid id, chunkmem<int> &tris) {
	new (_nodes + id)kdnode(_prims.size(), tris.size());
	for (int i = tris.begin(); i < tris.end(); ++i)
		_prims.push_back(tris[i]);
}

void kdtree::initInternalNode(nodeid id, nodeid leftChild, int axis, float split)
{
	new (_nodes + id)kdnode(axis, leftChild, split);
}

struct stackitem {
	int node;
	float texit;
};
//Maximum number of items we can store on the stack is the depth of the tree
//    and a tree of depth 64 should be plenty deep
static const int MAX_STACK_SIZE = 64;
void kdtree::draw(scene& scene, ray& ray, hit& hit) {
	float tentry, texit;
	if (!_boundingBox.intersect(ray, tentry, texit))
		return;

#if 0
        //ray.tfar = texit;
#endif
	if (tentry < 0.f) tentry = 0.f;
	stackitem stack[MAX_STACK_SIZE];
	uint32_t stackptr = 0;
	kdnode* currNode = _nodes;

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

                        ticks start = getticks();
			for (int i = 0; i != primcount; ++i) {
				int t = _prims[primidx + i];
				scene._accels[t].intersect(t, ray, hit);
			}
                        ticks end = getticks();
                        scene._intersectCost.inc(end - start, primcount);

			if (ray.tfar < texit) return;
			if (stackptr == 0) return;
			
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
        _boundingBox.intersect(r4, tentry, texit);
	
        if (_mm_movemask_ps(tentry == texit) == 0xF)
                return;

        ssef far[MAX_STACK_SIZE];
        int  nodes[MAX_STACK_SIZE];
	uint32_t stackptr = 0;
	kdnode* currNode = _nodes;
        

	while (true) {	
                if (!currNode->isLeaf()) {
			int axis  = currNode->getAxis();
                        float dist = currNode->getSplit () - r4.O()[axis];
                        ssef dist4(dist);
                        ssef t = dist4 * r4.rcpD()[axis];
                        unsigned int test = 
                            _mm_movemask_ps(dist4 <= (r4.D()[axis] * tentry));

			int left = currNode->getLeft();
                        // does the ray traverse both cells
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

                        ticks start = getticks();
			for (int i = 0; i != primcount; ++i) {
				int t = _prims[primidx + i];
				scene._accels[t].intersect(t, r4, hit4);
			}
                        ticks end = getticks();
                        scene._intersectCost.inc(end - start, primcount);

			if (_mm_movemask_ps(texit < r4.tfar) == 0) return;

			if (stackptr == 0) return;
			
			--stackptr;
                        tentry   = _mm_max_ps(tentry, texit);
			currNode = nodes[stackptr] + _nodes;
			texit    = far[stackptr];
                }
        }
}
