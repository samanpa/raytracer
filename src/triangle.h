#ifndef BPRAY_TRIANGLE_H
#define BPRAY_TRIANGLE_H

class triangle {
public:
	triangle(int p0, int p1, int p2)
		: p0(p0), p1(p1), p2(p2)
		{}
	
private:
	int p0;
	int p1;
	int p2;
};

#endif
