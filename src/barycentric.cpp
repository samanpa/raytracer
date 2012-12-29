#include "barycentric.h"

static int modulo3[] = {0, 1, 2, 0, 1};

triangle_barycentric::triangle_barycentric(vec3f &v0, vec3f &v1, vec3f &v2) {
	vec3f e1 = v1 - v0;
	vec3f e2 = v2 - v1;
	vec3f N(cross(e1, e2));
	normalize(N);
	
	if (fabs(N[X_Axis]) > fabs(N[Z_Axis]))
		k = (fabs(N[X_Axis]) > fabs(N[Z_Axis])) ? X_Axis : Z_Axis;
	else
		k = (fabs(N[Y_Axis]) > fabs(N[Z_Axis])) ? Y_Axis : Z_Axis;

	int ku = modulo3[1 + k];
	int kv = modulo3[2 + k];
	
	au = v1[ku] - v0[ku];
	av = v1[kv] - v0[kv];

	bu = v2[ku] - v0[ku];
	bv = v2[kv] - v0[kv];

	float area = 1 / (au * bv - av * bu);
	bu *= area;
	bv *= area;
	au *= area;
	av *= area;

	nu   = N[ku] / N[k];
	nv   = N[kv] / N[k];
	
	p0d  = v0[k] + v0[ku]*nu + v0[kv]*nv;
	p0u  = v0[ku];
	p0v  = v0[kv];
}

void triangle_barycentric::intersect(unsigned int prim_id, ray& ray, hit& h) {
	int ku = modulo3[1 + k];
	int kv = modulo3[2 + k];
	
	const vec3f &o = ray.O();
	const vec3f &d = ray.D();
	float nd = d[ku] * nu + d[kv] * nv + d[k]; //N . D
	float t = (p0d - o[k] - o[ku]*nu - o[kv]*nv)/nd;
	
	if (!(t > 0.0) || t > ray.tfar)
		return;

	float hu = d[ku] * t - p0u + o[ku];
	float hv = d[kv] * t - p0v + o[kv];

	float u = bv * hu - bu * hv;
	float v = au * hv - av * hu;

	float uv = u + v;
	if ((unsigned int&)uv > 0x3F800000)
		return;

	// if (u < 0.0f || v < 0.0f)
	if (((unsigned int&)u | (unsigned int&)v) > 0x80000000)
		return;
	
	h.prim    = prim_id;
	h.u       = u;
	h.v       = v;
	ray.tfar  = t;
}

