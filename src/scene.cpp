#include "scene.h"
#include <iostream>
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

void scene::draw(canvas& canvas) {
	for (int i = 0; i < canvas.getHeight(); ++i) {
		for (int j = 0; j < canvas.getWidth(); ++j) {
			canvas(i,j).setRed((i+0.)/canvas.getHeight());
			canvas(i,j).setGreen((j+0.)/canvas.getWidth());
		}
	}
}


bool scene::save(canvas& canvas, const char *filename) {
	gdImagePtr image = gdImageCreateTrueColor(canvas.getWidth(),
						  canvas.getHeight());
	FILE *file = fopen(filename, "wb");
	for (int i = 0; i < canvas.getHeight(); ++i) {
		for (int j = 0; j < canvas.getWidth(); ++j) {
			float color = gdImageColorExact(image,
							(int)(255 * canvas(i,j).getRed()),
							(int)(255 * canvas(i,j).getGreen()),
							(int)(255 * canvas(i,j).getBlue()));
			gdImageSetPixel(image, j, i, color);
		}
	}
	gdImagePng(image, file);
	gdImageDestroy(image);
	fclose(file);
}
