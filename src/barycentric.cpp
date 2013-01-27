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

	int kv = modulo3[1 + k];
	int kw = modulo3[2 + k];
	
	e1v = v1[kv] - v0[kv];
	e1w = v1[kw] - v0[kw];

	e2v = v2[kv] - v0[kv];
	e2w = v2[kw] - v0[kw];

	float area = 1 / (e1v * e2w - e1w * e2v);
	e2v *= area;
	e2w *= area;
	e1v *= area;
	e1w *= area;

	nv   = N[kv] / N[k];
	nw   = N[kw] / N[k];
	
	p0d  = v0[k] + v0[kv]*nv + v0[kw]*nw;
	p0v  = v0[kv];
	p0w  = v0[kw];

}

void triangle_barycentric::intersect(unsigned int prim_id, ray& ray, hit& h) {
	int kv = modulo3[1 + k];
	int kw = modulo3[2 + k];
	
	const vec3f &o = ray.O();
	const vec3f &d = ray.D();
	float nd = d[kv] * nv + d[kw] * nw + d[k]; //N . D
	float t = (p0d - o[k] - o[kv]*nv - o[kw]*nw)/nd;
	
	if (!(t > 0.0) || t > ray.tfar)
                return;

	float hv = d[kv] * t - p0v + o[kv];
	float hw = d[kw] * t - p0w + o[kw];

	float v = e2w * hv - e2v * hw;
	float w = e1v * hw - e1w * hv;

	float vw = v + w;
	//(v + w) > 1.0
	if ((unsigned int&)vw > 0x3F800000)
		return;

	// if (v < 0.0f || w < 0.0f)
	if (((unsigned int&)v | (unsigned int&)w) > 0x80000000)
		return;
	
	h.prim    = prim_id;
	h.v       = v;
	h.w       = w;
	ray.tfar  = t;
}

void triangle_barycentric::intersect(unsigned int primId, ray4& ray, hit4& h) {
	int kv = modulo3[1 + k];
	int kw = modulo3[2 + k];
	
        const ssef FLOAT4_FF = _mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF));
        const ssef FLOAT4_1(1.0f);
        const vec3f &o = ray.O();
	const vec3<ssef> &d = ray.D();
        ssef NV(nv);
        ssef NW(nw);
	ssef nd = d[kv] * NV + d[kw] * NW + d[k]; //N . D
	ssef t = ssef(p0d - o[k] - o[kv]*nv - o[kw]*nw)/nd;
	
        ssef mask = andnot((t > _mm_setzero_ps()), FLOAT4_FF); // !(t > 0.f)
        mask = mask | ( t > ray.tfar);        // t > ray.tfar

	ssef hv = d[kv] * t + ssef(o[kv] - p0v);
	ssef hw = d[kw] * t + ssef(o[kw] - p0w);

	ssef v  = e2w * hv - e2v * hw;
	ssef w  = e1v * hw - e1w * hv;
	ssef vw = v + w;

        mask = mask | ( v < _mm_setzero_ps()); // v < 0.0f
        mask = mask | ( w < _mm_setzero_ps()); // w < 0.0f
        mask = mask | ( vw > FLOAT4_1);        // v + w > 1.0

        ssef p1  = _mm_castsi128_ps(h.prim);
        ssef p2  = _mm_set1_ps((float&)primId);
        h.prim   = _mm_castps_si128(ifmask(mask, p1, p2));
	h.v      = ifmask(mask, h.v, v);
	h.w      = ifmask(mask, h.w, w);
	ray.tfar = ifmask(mask, ray.tfar, t);

}
