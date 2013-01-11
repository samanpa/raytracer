#ifndef BPRAY_SIMDV_H
#define BPRAY_SIMDV_H

#include "vec3.h"
#include "simd/simd.h"

__forceinline vec3<ssef> makeSimdVec( const vec3f &v1
				      , const vec3f &v2
				      , const vec3f &v3
				      , const vec3f &v4) {
	return vec3<ssef> (ssef(v1.x(), v2.x(), v3.x(), v4.x()),
			   ssef(v1.y(), v2.y(), v3.y(), v4.y()),
			   ssef(v1.z(), v2.z(), v3.z(), v4.z()));
}

__forceinline vec3<ssef> makeSimdVec(const vec3f &v1) {
	return vec3<ssef> (ssef(v1.x()), ssef(v1.y()), ssef(v1.z()));
}

#endif
