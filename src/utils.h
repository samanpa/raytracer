#ifndef BPRAY_UTILS_H
#define BPRAY_UTILS_H

#include <iomanip>
#include <iostream>
#include <stdint.h>

double getTime();

#define INFO(msg) do {						\
	std::cout << std::fixed << std::setprecision(6)         \
		  << getTime() << ": " << msg << std::endl;	\
} while(0)

#endif
