#ifndef BPRAY_GRID_H
#define BPRAY_GRID_H

#include <vector>
#include <algorithm>
#include "box.h"
#include "float.h"

struct grid {
        grid() {}
		
	void init(scene& scene) {
		vec3f lower (BIG_FLOAT, BIG_FLOAT, BIG_FLOAT);
		vec3f upper (NBIG_FLOAT, NBIG_FLOAT, NBIG_FLOAT);
	
		auto& vertices = scene.getVertices();
		for (auto& v : vertices) {
			lower[X_Axis] = std::min(v[X_Axis], lower[X_Axis]);
			lower[Y_Axis] = std::min(v[Y_Axis], lower[Y_Axis]);
			lower[Z_Axis] = std::min(v[Z_Axis], lower[Z_Axis]);
			upper[X_Axis] = std::max(v[X_Axis], upper[X_Axis]);
			upper[Y_Axis] = std::max(v[Y_Axis], upper[Y_Axis]);
			upper[Z_Axis] = std::max(v[Z_Axis], upper[Z_Axis]);
		}
		boundingBox.init(lower, upper, scene.getCamera().getLocation());
	}

	void draw(scene& scene, ray& ray, hit& hit) {
		float tentry, texit;
		if (!boundingBox.intersect(ray, tentry, texit))
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
