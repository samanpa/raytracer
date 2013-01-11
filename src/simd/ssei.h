#ifndef BPRAY_SSEI_H
#define BPRAY_SSEI_H

#include <immintrin.h>
#include "platform.h"

struct ssei
{
	union { __m128i m128; int v[4]; };

	__forceinline ssei() {}
	__forceinline ssei( const ssei& a) { m128 = a.m128; }
	__forceinline ssei& operator=(const ssei& a) {m128 = a.m128; return *this; }
	__forceinline ssei( const float&  a ) : m128(_mm_shuffle_epi32(_mm_castps_si128(_mm_load_ss((float*)&a)), _MM_SHUFFLE(0, 0, 0, 0))) {}
	__forceinline ssei( int a, int b, int c, int d) : m128(_mm_set_epi32(d, c, b, a)) {}
	__forceinline ssei( const __m128i& m128) : m128(m128) {}

	__forceinline operator const __m128i&( ) const { return m128; }
	__forceinline operator       __m128i&( )       { return m128; }
	__forceinline const int& operator [] ( int x) const { return v[x]; }

	friend std::ostream& operator<<(std::ostream& o, const ssei& s) {
		o << "<" << s.v[0] << "," << s.v[1]
		  << "," << s.v[2] << "," << s.v[3] << ">";
		return o;
	}
};

__forceinline const ssei operator*(const ssei& a, const ssei& b ) {
	return _mm_mul_epi32(a.m128, b.m128);
}

__forceinline ssei& operator*=(ssei& a, const ssei& b ) {
	a.m128 = _mm_mul_epi32(a.m128, b.m128);
	return a;
}

__forceinline ssei operator+(const ssei& a, const ssei& b) {
	return _mm_add_epi32(a.m128, b.m128);
}

__forceinline ssei& operator+=(ssei& a, const ssei& b ) {
	a.m128 = _mm_add_epi32(a.m128, b.m128);
	return a;
}

__forceinline const ssei operator-(const ssei& a, const ssei& b) {
	return _mm_sub_epi32(a.m128, b.m128);
}
#endif
