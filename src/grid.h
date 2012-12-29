#ifndef BPRAY_GRID_H
#define BPRAY_GRID_H

#include <vector>
#include <algorithm>
#include "box.h"

struct grid {
        grid() {}
		
	void init(scene& scene) {
		vec3f lower (3.3e38f, 3.3e38f, 3.3e38f);
		vec3f upper (-3.3e38f, -3.3e38f, -3.3e38f);
	
		auto& vertices = scene.getVertices();
		for (auto& v : vertices) {
			lower[X_Axis] = std::min(v[X_Axis], lower[X_Axis]);
			lower[Y_Axis] = std::min(v[Y_Axis], lower[Y_Axis]);
			lower[Z_Axis] = std::min(v[Z_Axis], lower[Z_Axis]);
			upper[X_Axis] = std::max(v[X_Axis], upper[X_Axis]);
			upper[Y_Axis] = std::max(v[Y_Axis], upper[Y_Axis]);
			upper[Z_Axis] = std::max(v[Z_Axis], upper[Z_Axis]);
		}
		boundingBox.init(lower, upper);
	}

	void draw(scene& scene, ray& ray, hit& hit) {
		if (!boundingBox.intersect(ray))
			return;

		for (unsigned int p = 0; p < scene._accels.size(); ++p) {
			ticks start = getticks();
			scene._accels[p].intersect(p, ray, hit);
			ticks end = getticks();
			scene._intersectCost.inc(end - start);
		}
	}
private:
	box boundingBox;
};

#endif
