#ifndef BPRAY_KDNODE_H
#define BPRAY_KDNODE_H

#include <stdint.h>

class kdnode {
	/***
	 *   31     is leaf
	 *for a non leaf bits
	 *   29..30 axis
	 *   1..28  prims
	 * for leaf bits
	 *   0..30  first primitive
	 */
	const uint32_t data;
	const union {
		float  splitPlane;
		uint32_t numPrims;
	};
public:
	int isLeaf()       { return data & (unsigned int) (1 << 31); }

	//internal node
	kdnode(int axis, int leftidx, float splitPlane)
		: data(leftidx | (axis << 29))
		, splitPlane(splitPlane) {}
	int getAxis()    { return (data >> 29); }
	int getLeft()    { return data & 0x1FFFFFF; }
	float getSplit() { return splitPlane; }
	
	//leaf
	kdnode(int offset, uint32_t numPrims) 
		: data((1 << 31) | offset)
		, numPrims(numPrims) {}
	int getPrimitiveOffset() { return data & 0x7FFFFFFF; }
	int getNumPrims() { return numPrims; }
};

#endif
