#ifndef BPRAY_FLOAT_H
#define BPRAY_FLOAT_H

static const float BIG_FLOAT = 3.3e38f;
static const float NBIG_FLOAT = -3.3e38f;
extern int modulo3[];

__forceinline float rcp(float f) {
	return 1.0f/f;
}

__forceinline float rsqrt(float f) {
	return 1.0f/sqrt(f);
}

__forceinline int getSign(float f) {
	return (0x80000000 & (int&)f) >> 31;
}
#endif
