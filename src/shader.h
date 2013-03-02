#ifndef BPRAY_SHADER_H
#define BPRAY_SHADER_H

#include "scene.h"
#include "canvas.h"

class shader {
public:
	void shade(scene &scene
                   , ray &ray
                   , color &color
                   , hit& hit);

	void shade(scene &scene
                   , ray4 *ray
                   , color *color
                   , hit4 *hit
                   , unsigned int n);
};

#endif

