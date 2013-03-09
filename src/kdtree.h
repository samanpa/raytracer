#ifndef BPRAY_KDTREE_H
#define BPRAY_KDTREE_H

#include "box.h"
#include "scene.h"
#include <vector>
#include <chunkmem.h>

typedef unsigned int nodeid;
struct kdnode;

class kdtree {
public:
	kdtree(scene& scene);

	void draw(scene& scene, ray& ray, hit& hit);
	
	nodeid allocNode();
	void initLeaf(nodeid id, int numPrims);
	void initInternalNode(nodeid id, nodeid left, int axis, float split);

	int getCurrNodeId() const { return _numNodes; }
	size_t getCurrPrimId() const { return _prims.size(); }
	void addPrim(int prim) { _prims.push_back(prim); }

protected:
        //Maximum number of items we can store on the stack is the depth of the tree
        //    and a tree of depth 64 should be plenty deep
        static const int MAX_STACK_SIZE = 64;
	box _boundingBox;
	kdnode *_nodes;
	std::vector<int> _prims;
	nodeid _numNodes;
	nodeid _maxNodes;
};

struct kdtreewachter : public kdtree {
	kdtreewachter(scene& scene) : kdtree(scene) {}
        using kdtree::draw;
        template <int N>
	void draw(scene& scene, ray4* ray, hit4* hit);
};

struct kdtreebenthin : public kdtree {
	kdtreebenthin(scene& scene) : kdtree(scene) {}
        using kdtree::draw;
        template <int N>
	void draw(scene& scene, ray4* ray, hit4* hit);
};

#endif
