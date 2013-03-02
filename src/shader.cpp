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
                   , ray4  *r4
                   , color *color
                   , hit4  *hit4
                   , unsigned int n)
{
        for (int i = 0; i < n; ++i) {
                for (int j = 0; j < 4; ++j) {
                        vec3f d(r4[i].D().x()[j], r4[i].D().y()[j], r4[i].D().z()[j]);
                        ray ray(r4[i].O(), d);
                        hit h;
                        
                        h.v    = hit4[i].v[j];
                        h.u    = hit4[i].u[j];
                        h.prim = hit4[i].prim[j];
                        
                        shade(scene, ray, color[i*4 +j], h);
                }
        }
}

