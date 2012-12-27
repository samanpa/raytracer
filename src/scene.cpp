#include "scene.h"
#include "ray.h"
#include <iostream>
#include <gd.h>
#include <stdio.h>
#include <cmath>
#include "noaccel.h"

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

static void getDownRight(scene &scene, canvas &canvas,
			 vec3f& down, vec3f& right, vec3f& left_top) {
	auto& cam = scene.getCamera();
	vec3f dir  = cam.getLookAt() - cam.getLocation();
	//assume magnitude of direction vector is normalized
	float leftstep = ::tan(cam.getAngle() * M_PI / 360.) 
		/ (canvas.getWidth() - 1) * 2.;
	float downstep = (leftstep * canvas.getHeight()) / canvas.getWidth();
	vec3f left = cross(cam.getUp(), dir);
	left_top = cam.getLocation() + dir + left + cam.getUp();
	down = vec3f(cam.getUp()[0]*-downstep,
		    cam.getUp()[1]*-downstep,
		    cam.getUp()[2]*-downstep);
	right= vec3f(left[0]*-leftstep,
		    left[1]*-leftstep,
		    left[2]*-leftstep);
}

void scene::shade(canvas& canvas, unsigned i, unsigned j, hit4& hit)
{
	
}

void scene::shade(canvas& canvas, unsigned i, unsigned j, hit& hit)
{
	if (hit.prim != (unsigned int)-1) {
		canvas(i,j).setRed(0.5);
		canvas(i,j).setGreen(0.5);
		canvas(i,j).setBlue(0.5);
	}
}

void scene::draw(canvas& canvas)
{
	vec3f down;
	vec3f right;
	vec3f left_top;
	getDownRight(*this, canvas, down, right, left_top);
	ray<float> ray;
	ray.O() = _camera.getLocation();
	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(left_top + (down * i));
		for (int j = 0; j < canvas.getWidth(); ++j) {
			ray.D() = pos - ray.O();
			normalize(ray.D());
			hit h;
			h.tfar = 3.3e38f;
			h.prim = -1; 
			noaccel::draw(*this, ray, h);
			shade(canvas, i, j, h);
			pos += right;
		}
	}
}

void scene::draw4(canvas& canvas)
{
	vec3f down;
	vec3f right;
	vec3f left_top;
	getDownRight(*this, canvas, down, right, left_top);
	ray<ssef> ray;
	ray.O()[X_Axis] = _camera.getLocation()[X_Axis];
	ray.O()[Y_Axis] = _camera.getLocation()[Y_Axis];
	ray.O()[Z_Axis] = _camera.getLocation()[Z_Axis];
	
	for (int i = 0; i < canvas.getHeight() ; i += 2) {
		vec3f pos(left_top + (down * i));
		for (int j = 0; j < canvas.getWidth(); j += 2) {
/*
			ray.D() *= 1/ray.D().getMagnitude();
			ray.tfar = 3.3e38f;
			ray.prim = -1; 
			noaccel::draw(*this, ray);
			shade(canvas, i, j, ray);
*/
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
