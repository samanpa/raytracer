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

        nodeid dummyNode = allocNode();
        initLeaf(dummyNode, 0);

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

void kdtree::initLeaf(nodeid id, int numPrims) {
	new (_nodes + id)kdnode(_prims.size(), numPrims);
}

void kdtree::initInternalNode(nodeid id, nodeid leftChild, int axis, float split)
{
	new (_nodes + id)kdnode(axis, leftChild, split);
}

