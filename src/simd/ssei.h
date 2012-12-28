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
	
};


#endif
