#include "box.h"

bool box::intersect(ray &r, float& tmin, float& tmax) {
	float tymax, tymin, tzmax, tzmin;
	int signx = getSign(r.rcpD().x());
	int signy = getSign(r.rcpD().y());
	int signz = getSign(r.rcpD().z());
	
	tmin  = bounds[    signx].x() * r.rcpD().x();
	tymax = bounds[1 ^ signy].y() * r.rcpD().y();
	if (tmin > tymax) return false;
	
	tymin = bounds[    signy].y() * r.rcpD().y();	
	tmax  = bounds[1 ^ signx].x() * r.rcpD().x();
	if (tymin > tmax) return false;
	
	if (tymax < tmax) tmax = tymax;
	tzmin = bounds[signz].z() * r.rcpD().z();
	if (tzmin > tmax) return false;
	
	if (tymin > tmin) tmin = tymin;
	tzmax = bounds[1 ^ signz].z() * r.rcpD().z();
	if (tmin > tzmax) return false;
		
	if (tzmax < tmax) tmax = tzmax;
	if (tmax < 0.0f) return false;
	
	if (tzmin > tmin) tmin = tzmin;
	
	return true;
}
