#ifndef BPRAY_CANVAS_H
#define BPRAY_CANVAS_H

#include "color.h"

class canvas {
public:
	canvas (short height, short width);
	color& get(short h, short w) {
		return _colors[h][w];
	}
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }

	bool save(const char *filename);
private:
	color **_colors;
	int _width;
	int _height;
};

#endif
