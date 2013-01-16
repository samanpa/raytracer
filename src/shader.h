#ifndef BPRAY_SHADER_H
#define BPRAY_SHADER_H

#include "scene.h"
#include "canvas.h"

class shader {
public:
	void shade(scene &scene
                   , canvas& canvas
                   , unsigned i
                   , unsigned j
                   , hit& hit);
	void shade(scene &scene
                   , canvas& canvas
                   , ssei i
                   , ssei j
                   , hit4& hit);
};

#endif

