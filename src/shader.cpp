#include "shader.h"

void shader::shade(scene& scene
                   , ray &ray
                   , color & color
                   , hit& hit)
{
	if (hit.prim != -1) {
#if 1
                ssef normal = scene.getTriangles()[hit.prim].getSmoothNormal(scene, hit.u, hit.v);
                ssef dir(ray.D().x(), ray.D().y(), ray.D().z(), 0.0);
                ssef dp = _mm_dp_ps(dir, normal, 0x77);
                float phong = -dp[0];
#else
                float phong = 0.5;
#endif
                color.set(phong, phong, phong);
	}
        else {
                color.set(0.f, 0.f, 0.f);
        }
}

void shader::shade(scene &scene
                   , ray4 &r4
                   , color color[4]
                   , hit4& hit4)
{
	for (int x = 0; x < 4; ++x) {
                vec3f d(r4.D().x()[x], r4.D().y()[x], r4.D().z()[x]);
                ray ray(r4.O(), d);
                hit h;

		h.v    = hit4.v[x];
		h.u    = hit4.u[x];
		h.prim = hit4.prim[x];
               
                shade(scene, ray, color[x], h);
	}
}

