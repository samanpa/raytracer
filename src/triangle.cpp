#include "triangle.h"
#include <iostream>

using namespace std;

static int modulo3[] = {0, 1, 2, 0, 1};

triangle::triangle(vec3 &v0, vec3 &v1, vec3 &v2) {
	_p0 = v0;
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v1;
	vec3 N(vec3::cross(e1, e2));
	N *= 1/N.getMagnitude();
	
	if (fabs(N[0]) > fabs(N[1]))
		k_plane = (fabs(N[0]) > fabs(N[2])) ? 0 : 2;
	else
		k_plane = (fabs(N[1]) > fabs(N[2])) ? 1 : 2;

	int u_plane = modulo3[1 + k_plane];
	int v_plane = modulo3[2 + k_plane];
		
	_au = v1[u_plane] - v0[u_plane];
	_av = v1[v_plane] - v0[v_plane];

	_bu = v2[u_plane] - v0[u_plane];
	_bv = v2[v_plane] - v0[v_plane];

	_area = 1 / (_au * _bv - _av * _bu);

	_nu   = N[u_plane] / N[k_plane];
	_nv   = N[v_plane] / N[k_plane];
}

bool triangle::intersect(ray& ray) {
	int u_plane = modulo3[1 + k_plane];
	int v_plane = modulo3[2 + k_plane];
		
	vec3 tmp(_p0 - ray.O());
	auto &dir = ray.D();
	float t = (tmp[u_plane] * _nu + tmp[v_plane] * _nv + tmp[k_plane])
		/ (dir[u_plane] * _nu + dir[v_plane] * _nv + dir[k_plane]);
	
	if (!(t > 0.0) || t > ray.tfar)
		return false;

	float hu = dir[u_plane] * t + ray.O()[u_plane] - _p0[u_plane];
	float hv = dir[v_plane] * t + ray.O()[v_plane] - _p0[v_plane];

	float varea = _au * hv - _av * hu;
	float uarea = _bv * hu - _bu * hv;

	float u = uarea * _area;
	float v = varea * _area;

	if (((unsigned int&)u | (unsigned int&)v) > 0x80000000)
		return false;
	
	if ((u + v) > 1.f)
		return false;

	ray.u = u;
	ray.v = v;
	ray.tfar = t;
	return true;
}
