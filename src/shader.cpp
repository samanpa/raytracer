#include "shader.h"

void shader::shade(scene& scene
                   , canvas& canvas
                   , unsigned i
                   , unsigned j
                   , hit& hit)
{
	if (hit.prim != -1) {
		canvas(i,j).setRed(0.5);
		canvas(i,j).setGreen(0.5);
		canvas(i,j).setBlue(0.5);
	}
}

void shader::shade(scene &scene
                   , canvas& canvas
                   , ssei i
                   , ssei j
                   , hit4& hit4)
{
	hit h;
	for (int x = 0; x < 4; ++x) {
		h.u    = hit4.u[x];
		h.v    = hit4.v[x];
		h.prim = hit4.prim[x];
		shade(scene, canvas, i[x], j[x], h);
	}
}

