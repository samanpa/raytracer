#ifndef BPRAY_SSEF_H
#define BPRAY_SSEF_H

#include <immintrin.h>
#include "platform.h"

struct ssef
{
	union { __m128 m128; float v[4]; };

	__forceinline ssef() {}
	__forceinline ssef( const ssef& a) { m128 = a.m128; }
	__forceinline ssef& operator=(const ssef& a) {m128 = a.m128; return *this; }
	__forceinline ssef( const float&  a ) : m128(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(_mm_load_ss(&a)), _MM_SHUFFLE(0, 0, 0, 0)))) {}

};


#endif
