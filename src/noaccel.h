#ifndef BPRAY_NOACCEL_H
#define BPRAY_NOACCEL_H

#include <scene.h>

struct noaccel {
        
        noaccel(scene& scene){}
        
	void draw(scene& scene, ray& ray, hit& hit) {
		for (unsigned int p = 0; p < scene._accels.size(); ++p) {
			ticks start = getticks();
			scene._accels[p].intersect(p, ray, hit);
			ticks end = getticks();
			scene._intersectCost.inc(end - start);
		}
	}


	void draw(scene& scene, ray4& ray, hit4& hit) {
                ticks start = getticks();
		for (unsigned int p = 0; p < scene._accels.size(); ++p) {
			scene._accels[p].intersect(p, ray, hit);
		}
                ticks end = getticks();
                scene._intersectCost.inc(end - start, scene._accels.size());
	}
};

#endif
