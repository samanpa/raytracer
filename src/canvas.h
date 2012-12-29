#ifndef BPRAY_CANVAS_H
#define BPRAY_CANVAS_H

#include "color.h"
#include <gd.h>
#include <stdio.h>

class canvas {
public:
	canvas (short height, short width);
	color& operator ()(short h, short w) {
		return _colors[h][w];
	}
	int getWidth() { return _width; }
	int getHeight() { return _height; }

	bool save(const char *filename) {
		gdImagePtr image = gdImageCreateTrueColor(getWidth(),
							  getHeight());
		FILE *file = fopen(filename, "wb");
		for (int i = 0; i < getHeight(); ++i) {
			for (int j = 0; j < getWidth(); ++j) {
				color& c = _colors[i][j];
				float r = gdImageColorExact(image
							    , 255 * c.getRed()
							    , 255 * c.getGreen()
							    , 255 * c.getBlue());
				gdImageSetPixel(image, j, i, r);
			}
		}
		gdImagePng(image, file);
		gdImageDestroy(image);
		fclose(file);
		return true;
	}
private:
	color **_colors;
	int _width;
	int _height;
};

#endif
