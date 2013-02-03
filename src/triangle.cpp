#include "triangle.h"
#include <algorithm>
#include <iostream>
#include "scene.h"

using namespace std;


void triangle::getBounds(const scene& scene, vec3f &min, vec3f &max) const {
        const auto &vs = scene.getVertices();
        const auto &v0 = vs[p0];
        const auto &v1 = vs[p1];
        const auto &v2 = vs[p2];

	max.x() = std::max(v0.x(), v1.x());
	max.x() = std::max(max.x(), v2.x());
	max.y() = std::max(v0.y(), v1.y());
	max.y() = std::max(max.y(), v2.y());
	max.z() = std::max(v0.z(), v1.z());
	max.z() = std::max(max.z(), v2.z());
	min.x() = std::min(v0.x(), v1.x());
	min.x() = std::min(min.x(), v2.x());
	min.y() = std::min(v0.y(), v1.y());
	min.y() = std::min(min.y(), v2.y());
	min.z() = std::min(v0.z(), v1.z());
	min.z() = std::min(min.z(), v2.z());
}

ssef triangle::getSmoothNormal(const scene& scene, float u, float v) const {
        const auto& n0 = scene.getNormal(p0);
        const auto& n1 = scene.getNormal(p1);
        const auto& n2 = scene.getNormal(p2);

        ssef n  = u * n0 + v * n1  + (1 - u - v) * n2;
        ssef dp = _mm_dp_ps(n, n, 0x77);
#if 0
        ssef nn = rcp(dp * dp) * n; // approximate normalized normal
#else
        ssef nn = rsqrt(dp) * n; //normalize normal
#endif
        return nn;
}
