#ifndef BPRAY_FLOAT_H
#define BPRAY_FLOAT_H

static const float BIG_FLOAT = 3.3e38f;
static const float NBIG_FLOAT = -3.3e38f;

__forceinline float rcp(float f) {
	return 1.0f/f;
}

#endif
