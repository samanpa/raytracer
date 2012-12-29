#include "scene.h"
#include "ray.h"
#include <iostream>
#include <cmath>
#include "grid.h"
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
	grid grid;
	vec3f down;
	vec3f right;
	vec3f left_top;
	getDownRight(*this, canvas, down, right, left_top);

	grid.init(*this);
	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(left_top + (down * i));
		for (int j = 0; j < canvas.getWidth(); ++j) {
			vec3f dir = pos - _camera.getLocation();
			ray ray (_camera.getLocation(), dir);
			hit h;
			h.prim = -1; 
			grid.draw(*this, ray, h);
			shade(canvas, i, j, h);
			pos += right;
		}
	}
}

