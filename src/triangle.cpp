#include "triangle.h"
#include <algorithm>
#include <iostream>
#include "scene.h"

using namespace std;


void triangle::getBounds(const scene& scene, aabb& bb) const {
        const auto &v0 = scene.getVertex(p0);
        const auto &v1 = scene.getVertex(p1);
        const auto &v2 = scene.getVertex(p2);

	bb.upper.x() = std::max(v0.x(), v1.x());
	bb.upper.x() = std::max(bb.upper.x(), v2.x());
	bb.upper.y() = std::max(v0.y(), v1.y());
	bb.upper.y() = std::max(bb.upper.y(), v2.y());
	bb.upper.z() = std::max(v0.z(), v1.z());
	bb.upper.z() = std::max(bb.upper.z(), v2.z());
	bb.lower.x() = std::min(v0.x(), v1.x());
	bb.lower.x() = std::min(bb.lower.x(), v2.x());
	bb.lower.y() = std::min(v0.y(), v1.y());
	bb.lower.y() = std::min(bb.lower.y(), v2.y());
	bb.lower.z() = std::min(v0.z(), v1.z());
	bb.lower.z() = std::min(bb.lower.z(), v2.z());
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
