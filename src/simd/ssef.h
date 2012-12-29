#ifndef BPRAY_SSEF_H
#define BPRAY_SSEF_H

#include "platform.h"

#include <immintrin.h>
#include <iostream>

struct ssef
{
	union { __m128 m128; float v[4]; };

	__forceinline ssef() {}
	__forceinline ssef( const ssef& a) { m128 = a.m128; }
	__forceinline ssef& operator=(const ssef& a) {m128 = a.m128; return *this; }
	__forceinline ssef( const float&  a ) : m128(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(_mm_load_ss(&a)), _MM_SHUFFLE(0, 0, 0, 0)))) {}
	__forceinline ssef( const __m128 a ) : m128(a) {}
	__forceinline ssef(float a, float b, float c, float d) : m128(_mm_set_ps(d, c, b, a)) {}

	friend std::ostream& operator<<(std::ostream& o, ssef& s) {
		o << "<" << s.v[0] << "," << s.v[1]
		  << "," << s.v[2] << "," << s.v[3] << ">";
		return o;
	}
};

__forceinline const ssef operator*( const ssef& a, const ssef& b ) {
	return _mm_mul_ps(a.m128, b.m128);
}
__forceinline const ssef operator/(const ssef& a, const ssef& b) {
	return _mm_div_ps(a.m128, b.m128);
}

__forceinline ssef operator+(ssef& a, ssef& b) {
	return _mm_add_ps(a.m128, b.m128);
}

__forceinline const ssef operator-(const ssef& a, const ssef& b) {
	return _mm_sub_ps(a.m128, b.m128);
}
//http://en.wikipedia.org/wiki/Division_%28digital%29#Newton.E2.80.93Raphson_division
__forceinline ssef rcp(ssef n) {
	ssef x0 = _mm_rcp_ps(n.m128);
	return (x0 + x0) - (x0 * x0 * n);
}


#endif
