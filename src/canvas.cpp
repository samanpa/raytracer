#include "canvas.h"

#include <gd.h>
#include <stdio.h>

canvas::canvas(short height, short width) {
	_colors = new color*[height];
	_height  = height;
	_width   = width;
	for (int i = 0; i < height; ++i) {
		_colors[i] = new color[width];
		for (int j = 0; j < width; j++)
			_colors[i][j].set(0.f, 0.f, 0.f);
	}
}

bool canvas::save(const char *filename) {
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

