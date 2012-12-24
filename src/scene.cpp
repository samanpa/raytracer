#include "scene.h"
#include "ray.h"
#include <iostream>
#include <gd.h>
#include <stdio.h>
#include <cmath>


using namespace std;

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
	vec3 dir  = _camera.getLookAt() - _camera.getLocation();
	float mag = dir.getMagnitude();
	//assume magnitude of direction vector is normalized
	float leftstep = ::tan(_camera.getAngle() * M_PI / 360.) 
		/ (canvas.getWidth() - 1) * 2.;
	float downstep = (leftstep * canvas.getHeight()) / canvas.getWidth();
	vec3 left = vec3::cross(_camera.getUp(), dir);
	vec3 left_top = _camera.getLocation() + dir + left + _camera.getUp();
	vec3 down(_camera.getUp()[0]*-downstep,
		  _camera.getUp()[1]*-downstep,
		  _camera.getUp()[2]*-downstep);
	vec3 right(left[0]*-leftstep,
		   left[1]*-leftstep,
		   left[2]*-leftstep);
	ray ray;
	ray.O() = _camera.getLocation();
	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3 pos(left_top + (down * i));
		for (int j = 0; j < canvas.getWidth(); ++j) {
			ray.D() = pos - ray.O();
			ray.D() *= 1/ray.D().getMagnitude();
			ray.tfar = 3.3e38f;
			
			for (unsigned int p = 0; p < _accels.size(); ++p) {
				ticks start = getticks();
				bool  hit   = _accels[p].intersect(p, ray);
				ticks end = getticks();
				_intersectCost.inc(end - start);
				if (hit) {
					canvas(i,j).setRed(0.5);
					canvas(i,j).setGreen(0.5);
					canvas(i,j).setBlue(0.5);
				}
			}
			pos += right;
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
	return true;
}
