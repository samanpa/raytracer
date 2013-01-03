#ifndef BPRAY_GRID_H
#define BPRAY_GRID_H

#include <vector>
#include <algorithm>
#include "box.h"
#include "float.h"
#include "scene.h"
#include <iostream>

struct grid {
        grid() {}
		
	void init(scene& scene);
	void draw(scene& scene, ray& ray, hit& hit);
private:
	std::vector<int> &getCell(int x, int y, int z) {
		int idx = (x * _n.y()  + y) * _n.z() + z;
		return _cellContents.at(idx);
	}
	std::vector<std::vector<int>> _cellContents;
	box _boundingBox;
	vec3f _lower;
	vec3<int> _n; //resolution
	vec3f _rcpwidth; //reciprocal of the cell width 
	vec3f _cellwidth;
};

#endif
