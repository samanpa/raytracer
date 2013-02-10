#ifndef BPRAY_SSED_H
#define BPRAY_SSED_H

#include "platform.h"

#include <immintrin.h>
#include <iostream>

struct ssed
{
	union { __m128d m128; double v[2]; };

	__forceinline ssed() {}
	__forceinline ssed( const ssed& a) { m128 = a.m128; }
	__forceinline ssed& operator=(const ssed& a) {m128 = a.m128; return *this; }
	__forceinline ssed( const __m128d a ) : m128(a) {}
	__forceinline ssed(double a, double b) : m128(_mm_set_pd(b, a)) {}

	__forceinline operator const __m128d&( void ) const { return m128; }
	__forceinline operator       __m128d&( void )       { return m128; }
	__forceinline const double& operator [] ( int x) const { return v[x]; }

	friend std::ostream& operator<<(std::ostream& o, const ssed& s) {
		o << "<" << s.v[0] << "," << s.v[1] << ">";
		return o;
	}
};

__forceinline const ssed operator*(const ssed& a, const ssed& b ) {
	return _mm_mul_pd(a.m128, b.m128);
}

__forceinline ssed& operator*=(ssed& a, const ssed& b ) {
	a.m128 = _mm_mul_pd(a.m128, b.m128);
	return a;
}

__forceinline const ssed operator/(const ssed& a, const ssed& b) {
	return _mm_div_pd(a.m128, b.m128);
}

__forceinline ssed operator+(const ssed& a, const ssed& b) {
	return _mm_add_pd(a.m128, b.m128);
}

__forceinline ssed& operator+=(ssed& a, const ssed& b ) {
	a.m128 = _mm_add_pd(a.m128, b.m128);
	return a;
}

__forceinline const ssed operator-(const ssed& a, const ssed& b) {
	return _mm_sub_pd(a.m128, b.m128);
}

__forceinline const ssed operator<(const ssed& a, const ssed& b) {
	return _mm_cmplt_pd(a.m128, b.m128);
}

__forceinline const ssed operator<=(const ssed& a, const ssed& b) {
	return _mm_cmple_pd(a.m128, b.m128);
}

__forceinline const ssed operator>(const ssed& a, const ssed& b) {
	return _mm_cmpgt_pd(a.m128, b.m128);
}

__forceinline const ssed operator>=(const ssed& a, const ssed& b) {
	return _mm_cmpge_pd(a.m128, b.m128);
}

__forceinline const ssed operator==(const ssed& a, const ssed& b) {
	return _mm_cmpeq_pd(a.m128, b.m128);
}

__forceinline const ssed andnot(const ssed& a, const ssed& b) {
        return _mm_andnot_pd(a.m128, b.m128);
}

__forceinline const ssed operator|(const ssed& a, const ssed& b) {
	return _mm_or_pd(a.m128, b.m128);
}

__forceinline const ssed operator&(const ssed& a, const ssed& b) {
	return _mm_and_pd(a.m128, b.m128);
}

__forceinline const ssed max(const ssed& a, const ssed& b) {
        return _mm_max_pd(a.m128, b.m128);
}

__forceinline const ssed min(const ssed& a, const ssed& b) {
        return _mm_min_pd(a.m128, b.m128);
}

__forceinline const ssed clamp(const ssed& v, const ssed& minv, const ssed& maxv) {
        return max(minv, min(v, maxv));
}


#endif
