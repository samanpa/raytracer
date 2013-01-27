#include "barycentric.h"
#include "utils.h"

int modulo3[] = {0, 1, 2, 0, 1};

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
	
	e1u = v1[ku] - v0[ku];
	e1v = v1[kv] - v0[kv];

	e2u = v2[ku] - v0[ku];
	e2v = v2[kv] - v0[kv];

	float area = 1 / (e1u * e2v - e1v * e2u);
	e2u *= area;
	e2v *= area;
	e1u *= area;
	e1v *= area;

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

	float u = e2v * hu - e2u * hv;
	float v = e1u * hv - e1v * hu;

	float uv = u + v;
	//(u + v) > 1.0
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

void triangle_barycentric::intersect(unsigned int primId, ray4& ray, hit4& h) {
	int ku = modulo3[1 + k];
	int kv = modulo3[2 + k];
	
        const ssef FLOAT4_FF = _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF));
        const ssef FLOAT4_1(1.0f);
        const vec3f &o = ray.O();
	const vec3<ssef> &d = ray.D();
        ssef NU(nu);
        ssef NV(nv);
	ssef nd = d[ku] * NU + d[kv] * NV + d[k]; //N . D
	ssef t = ssef(p0d - o[k] - o[ku]*nu - o[kv]*nv)/nd;
	
        ssef mask = andnot((t > _mm_setzero_ps()), FLOAT4_FF); // !(t > 0.f)
        mask = mask | ( t > ray.tfar);        // t > ray.tfar

	ssef hu = d[ku] * t + ssef(o[ku] - p0u);
	ssef hv = d[kv] * t + ssef(o[kv] - p0v);

	ssef u  = e2v * hu - e2u * hv;
	ssef v  = e1u * hv - e1v * hu;
	ssef uv = u + v;

        mask = mask | ( u < _mm_setzero_ps()); // u < 0.0f
        mask = mask | ( v < _mm_setzero_ps()); // v < 0.0f
        mask = mask | ( uv > FLOAT4_1);        // u + v > 1.0

        ssef p1  = _mm_castsi128_ps(h.prim);
        ssef p2  = _mm_set1_ps((float&)primId);
        h.prim   = _mm_castps_si128(ifmask(mask, p1, p2));
	h.u      = ifmask(mask, h.u, u);
	h.v      = ifmask(mask, h.v, v);
	ray.tfar = ifmask(mask, ray.tfar, t);

}
