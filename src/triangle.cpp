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
