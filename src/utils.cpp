#include "utils.h"

static double getCurrTime() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec +  ts.tv_nsec * 1e-9;
}


double getTime() {
        static double start_time = getCurrTime();
	double curr = getCurrTime();
	double diff = curr - start_time;
        start_time  = curr;
	return diff;
}
