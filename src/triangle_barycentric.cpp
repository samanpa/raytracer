#include "triangle_barycentric.h"

static int modulo3[] = {0, 1, 2, 0, 1};

triangle_barycentric::triangle_barycentric(vec3 &v0, vec3 &v1, vec3 &v2) {
	p0 = v0;
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v1;
	vec3 N(vec3::cross(e1, e2));
	N *= 1/N.getMagnitude();
	
	if (fabs(N[0]) > fabs(N[1]))
		k = (fabs(N[0]) > fabs(N[2])) ? 0 : 2;
	else
		k = (fabs(N[1]) > fabs(N[2])) ? 1 : 2;

	int ku = modulo3[1 + k];
	int kv = modulo3[2 + k];
		
	au = v1[ku] - v0[ku];
	av = v1[kv] - v0[kv];

	bu = v2[ku] - v0[ku];
	bv = v2[kv] - v0[kv];

	area = 1 / (au * bv - av * bu);

	nu   = N[ku] / N[k];
	nv   = N[kv] / N[k];
}

bool triangle_barycentric::intersect(unsigned int prim_id, ray& ray) {
	int ku = modulo3[1 + k];
	int kv = modulo3[2 + k];
		
	vec3 diff(p0 - ray.O());
	auto &dir = ray.D();
	float t = dir[ku] * nu + dir[kv] * nv + dir[k];
	t = (diff[ku] * nu + diff[kv] * nv + diff[k]) / t;
	
	if (!(t > 0.0) || t > ray.tfar) {
		return false;
	}

	float hu = dir[ku] * t - diff[ku];
	float hv = dir[kv] * t - diff[kv];

	float uarea = bv * hu - bu * hv;
	float varea = au * hv - av * hu;

	float u = uarea * area;
	float v = varea * area;

	float uv = u + v;
	if ((unsigned int&)uv > 0x3F800000)
		return false;

	// if (u < 0.0f || v < 0.0f)
	if (((unsigned int&)u | (unsigned int&)v) > 0x80000000)
		return false;
	
	ray._prim = prim_id;
	ray.u     = u;
	ray.v     = v;
	ray.tfar  = t;
	return true;
}
