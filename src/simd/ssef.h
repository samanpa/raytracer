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

	__forceinline operator const __m128&( void ) const { return m128; }
	__forceinline operator       __m128&( void )       { return m128; }
	__forceinline const float& operator [] ( int x) const { return v[x]; }

	friend std::ostream& operator<<(std::ostream& o, const ssef& s) {
		o << "<" << s.v[0] << "," << s.v[1]
		  << "," << s.v[2] << "," << s.v[3] << ">";
		return o;
	}
};

__forceinline unsigned int movemask(const ssef &m) {
        return _mm_movemask_ps(m.m128);
}

__forceinline const ssef operator*(const ssef& a, const ssef& b ) {
	return _mm_mul_ps(a.m128, b.m128);
}

__forceinline ssef& operator*=(ssef& a, const ssef& b ) {
	a.m128 = _mm_mul_ps(a.m128, b.m128);
	return a;
}

__forceinline const ssef operator/(const ssef& a, const ssef& b) {
	return _mm_div_ps(a.m128, b.m128);
}

__forceinline ssef operator+(const ssef& a, const ssef& b) {
	return _mm_add_ps(a.m128, b.m128);
}

__forceinline ssef& operator+=(ssef& a, const ssef& b ) {
	a.m128 = _mm_add_ps(a.m128, b.m128);
	return a;
}

__forceinline const ssef operator-(const ssef& a, const ssef& b) {
	return _mm_sub_ps(a.m128, b.m128);
}

__forceinline const ssef operator<(const ssef& a, const ssef& b) {
	return _mm_cmplt_ps(a.m128, b.m128);
}

__forceinline const ssef operator<=(const ssef& a, const ssef& b) {
	return _mm_cmple_ps(a.m128, b.m128);
}

__forceinline const ssef operator>(const ssef& a, const ssef& b) {
	return _mm_cmpgt_ps(a.m128, b.m128);
}

__forceinline const ssef operator>=(const ssef& a, const ssef& b) {
	return _mm_cmpge_ps(a.m128, b.m128);
}

__forceinline const ssef operator==(const ssef& a, const ssef& b) {
	return _mm_cmpeq_ps(a.m128, b.m128);
}

__forceinline const ssef andnot(const ssef& a, const ssef& b) {
        return _mm_andnot_ps(a.m128, b.m128);
}

__forceinline const ssef operator|(const ssef& a, const ssef& b) {
	return _mm_or_ps(a.m128, b.m128);
}

__forceinline const ssef operator&(const ssef& a, const ssef& b) {
	return _mm_and_ps(a.m128, b.m128);
}

__forceinline const ssef ifmask(const ssef &m, const ssef& a, const ssef& b) {
        return (m & a) | andnot(m, b);
}

__forceinline const ssef max(const ssef& a, const ssef& b) {
        return _mm_max_ps(a, b);
}

__forceinline const ssef min(const ssef& a, const ssef& b) {
        return _mm_min_ps(a, b);
}

//http://en.wikipedia.org/wiki/Division_%28digital%29#Newton.E2.80.93Raphson_division
__forceinline ssef rcp(ssef n) {
	ssef x0 = _mm_rcp_ps(n.m128);
	return (x0 + x0) - (x0 * x0 * n);
}

// The instruction RSQRTSS or RSQRTPS gives the reciprocal square root with a 
// precision of 12 bits. You can improve the precision to 23 bits by using the
// Newton-Raphson formula described in Intel's application note AP-803:
// x0 = rsqrtss(a)
// x1 = 0.5 * x0 * (3 - (a * x0) * x0)
__forceinline ssef rsqrt(ssef a) {
	__m128 r = _mm_rsqrt_ps(a.m128);
	return _mm_add_ps(_mm_mul_ps(_mm_set_ps(1.5f, 1.5f, 1.5f, 1.5f), r),
			  _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(a,
							   _mm_set_ps(-0.5f, -0.5f, -0.5f, -0.5f)), r), _mm_mul_ps(r, r)));
}

#endif
