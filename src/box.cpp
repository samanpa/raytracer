#include "box.h"

void box::init (const vec3f& lower, const vec3f& upper, const vec3f& orig) {
        //translate bounds to the orig so we don't
        // do it every time we intersect
        bounds[0] = lower - orig;
        bounds[1] = upper - orig;
}

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

void box::intersect(ray4 &r, ssef& tnear, ssef& tfar) {
	ssef tymax, tymin, tzmax, tzmin, mask, mask2, tmin, tmax;

	int signx = getSign(r.rcpD().x()[0]);
	int signy = getSign(r.rcpD().y()[0]);
	int signz = getSign(r.rcpD().z()[0]);
	
	tmin  = ssef(bounds[    signx].x()) * r.rcpD().x();
	tymax = ssef(bounds[1 ^ signy].y()) * r.rcpD().y();
        mask  = (tmin > tymax);

	tymin = ssef(bounds[    signy].y()) * r.rcpD().y();	
        tmax  = ssef(bounds[1 ^ signx].x()) * r.rcpD().x();
        mask  = mask | (tymin > tmax);

        mask2 = (tymax < tmax); //if (tymax < tmax) tmax = tymax;
        tmax  = ifmask(mask2, tymax, tmax);
        tzmin = ssef(bounds[    signz].z()) * r.rcpD().z();
        mask  = mask | (tzmin > tmax);
	
	mask2 = (tymin > tmin); //if (tymin > tmin) tmin = tymin;
        tmin  = ifmask(mask2, tymin, tmin);
        tzmax = ssef(bounds[1 ^ signz].z()) * r.rcpD().z();
        mask  = mask | (tmin > tzmax);

        mask2 = (tzmax < tmax); //if (tzmax < tmax) tmax = tzmax;
        tmax  = ifmask(mask2, tzmax, tmax);
        mask  = mask | (tmax < _mm_setzero_ps());

        mask2 = (tzmin > tmin); //if (tzmin > tmin) tmin = tzmin;
        tmin  = ifmask(mask2, tzmin, tmin);

        tfar  = ifmask(mask, tfar, tmax);
        mask  = mask | (tmin < _mm_setzero_ps());
        tnear = ifmask(mask, tnear, tmin);
}

void box::clip(ray4 &r, ssef& tnear, ssef& tfar) {
	ssef tymax, tymin, tzmax, tzmin, mask, mask2, tmin, tmax;

	int signx = getSign(r.rcpD().x()[0]);
	int signy = getSign(r.rcpD().y()[0]);
	int signz = getSign(r.rcpD().z()[0]);

	tmin  = ssef(bounds[    signx].x()) * r.rcpD().x();
        tmax  = ssef(bounds[1 ^ signx].x()) * r.rcpD().x();

	tymin = ssef(bounds[    signy].y()) * r.rcpD().y();
	tymax = ssef(bounds[1 ^ signy].y()) * r.rcpD().y();

        tzmin = ssef(bounds[    signz].z()) * r.rcpD().z();
        tzmax = ssef(bounds[1 ^ signz].z()) * r.rcpD().z();

        tmin  = max(tymin, tmin);
        tmax  = min(tymax, tmax);

        tmin  = max(tzmin, tmin);
        tmax  = min(tzmax, tmax);

        tnear = max(_mm_setzero_ps(), tmin);
        tfar  = tmax;
}
