#include "kdtree.h"
#include "float.h"
#include "kdnode.h"
#include "stdlib.h"
#include "kdtreebuilder.h"
#include "kdnode.h"

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
	_boundingBox.init(lower, upper, scene.getCamera().getLocation());
	kdtreebuilder builder;
	builder.build(*this, scene, lower, upper);
}

nodeid kdtree::allocNode() {
	if (_numNodes == _maxNodes) {
		_maxNodes *= 2;
		_nodes = (kdnode*)realloc(_nodes, _maxNodes * sizeof(kdnode));
	}
	return _numNodes++;
}

void kdtree::initLeaf(nodeid id, vector<int> &tris) {
	new (_nodes + id)kdnode(_prims.size(), tris.size());
	for (auto t : tris)
		_prims.push_back(t);
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

	stackitem stack[MAX_STACK_SIZE];
	uint32_t stackptr = 0;
	kdnode* currNode = _nodes;
	if (tentry < 0.f) tentry = 0.f;

	while (true) {	
		//cout << (currNode - _nodes) << " " << currNode->isLeaf() << endl;
		if (!currNode->isLeaf()) {

			//We classify traversal cases by the ray origin
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
				ticks start = getticks();
				scene._accels[t].intersect(t, ray, hit);
				ticks end = getticks();
				scene._intersectCost.inc(end - start);
			}

			if (ray.tfar < texit) return;

			if (stackptr == 0) return;
			
			--stackptr;
			currNode = stack[stackptr].node + _nodes;
			texit    = stack[stackptr].texit;
		}
	}
}
