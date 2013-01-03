#ifndef BPRAY_KDTREE_H
#define BPRAY_KDTREE_H

#include "box.h"
#include "scene.h"
#include <vector>

typedef unsigned int nodeid;
struct kdnode;

class kdtree {
public:
	kdtree(scene& scene);
	void draw(scene& scene, ray& ray, hit& hit);
	
	nodeid allocNode();
	void initLeaf(nodeid id, std::vector<int> &tris);
	void initInternalNode(nodeid id, nodeid left, int axis, float split);

	int getCurrNodeId() const { return _numNodes; }
	size_t getCurrPrimId() const { return _prims.size(); }
	void addPrim(int prim) { _prims.push_back(prim); }

private:
	box _boundingBox;
	kdnode *_nodes;
	std::vector<int> _prims;
	nodeid _numNodes;
	nodeid _maxNodes;
};

#endif