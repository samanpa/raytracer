#ifndef BPRAY_CANVAS_H
#define BPRAY_CANVAS_H

#include "color.h"

class canvas {
public:
	canvas (short height, short width);
	color& operator ()(short h, short w) {
		return _colors[h][w];
	}
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	bool save(const char *filename);
private:
	color **_colors;
	int _width;
	int _height;
};

#endif
