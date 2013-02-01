#include "shader.h"

void shader::shade(scene& scene
                   , ray &ray
                   , canvas& canvas
                   , unsigned i
                   , unsigned j
                   , hit& hit)
{
	if (hit.prim != -1) {
#if 1
                ssef normal = scene.getTriangles()[hit.prim].getSmoothNormal(scene, hit.u, hit.v);
                ssef dir(ray.D().x(), ray.D().y(), ray.D().z(), 0.0);
                ssef dp = _mm_dp_ps(dir, normal, 0x7f);
                float phong = -dp[0];
#else
                float phong = 0.5;
#endif
 		canvas.get(i,j).setRed(phong);
		canvas.get(i,j).setGreen(phong);
		canvas.get(i,j).setBlue(phong);
	}
}

void shader::shade(scene &scene
                   , ray4 &r4
                   , canvas& canvas
                   , ssei i
                   , ssei j
                   , hit4& hit4)
{
	hit h;
	for (int x = 0; x < 4; ++x) {
                vec3f d(r4.D().x()[x], r4.D().y()[x], r4.D().z()[x]);
                ray ray(r4.O(), d);

		h.v    = hit4.v[x];
		h.u    = hit4.u[x];
		h.prim = hit4.prim[x];
		shade(scene, ray, canvas, i[x], j[x], h);
	}
}

