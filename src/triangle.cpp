#include "triangle.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool triangle::intersect(const vec3f &p0, const vec3f &p1, const vec3f &p2,
			 const vec3f &lower, const vec3f &upper) {
	vec3f pl, ph;
	ph.x() = std::max(p0.x(), p1.x());
	ph.x() = std::max(ph.x(), p2.x());
	ph.y() = std::max(p0.y(), p1.y());
	ph.y() = std::max(ph.y(), p2.y());
	ph.z() = std::max(p0.z(), p1.z());
	ph.z() = std::max(ph.z(), p2.z());
	pl.x() = std::min(p0.x(), p1.x());
	pl.x() = std::min(pl.x(), p2.x());
	pl.y() = std::min(p0.y(), p1.y());
	pl.y() = std::min(pl.y(), p2.y());
	pl.z() = std::min(p0.z(), p1.z());
	pl.z() = std::min(pl.z(), p2.z());

	if (ph.x() < lower.x()) return false;
	if (ph.y() < lower.y()) return false;
	if (ph.z() < lower.z()) return false;
	if (pl.x() > upper.x()) return false;
	if (pl.y() > upper.y()) return false;
	if (pl.z() > upper.z()) return false;
	return true;
}
