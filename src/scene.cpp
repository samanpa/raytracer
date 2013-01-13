#include "scene.h"
#include "ray.h"
#include <cmath>
#include "kdtree.h"
#include "simdv.h"
#include "utils.h"

using namespace std;

static void getDownRight(scene &scene, canvas &canvas,
			 vec3f& down, vec3f& right, vec3f& lefttop) {
	auto& cam = scene.getCamera();
	vec3f up(cam.getUp());
	vec3f dir  = cam.getLookAt() - cam.getLocation();

	//Assumes the direction vector is of unit length
	float leftmag = ::tan(cam.getAngle() * M_PI / 360.);
	float upmag   = leftmag * canvas.getHeight() / canvas.getWidth();
	vec3f left    = cross(up, dir);

	normalize(dir);
	normalize(up);
	normalize(left);

	lefttop = cam.getLocation() + dir 
		+ (leftmag * left) + (upmag * up);

	float leftstep = 2.f * leftmag / (canvas.getWidth() - 1);
	float downstep = 2.f * upmag   / (canvas.getHeight() - 1);;
	down = up   * -downstep;
	right= left * -leftstep;
}

void scene::shade(canvas& canvas, unsigned i, unsigned j, hit& hit)
{
	if (hit.prim != -1) {
		canvas(i,j).setRed(0.5);
		canvas(i,j).setGreen(0.5);
		canvas(i,j).setBlue(0.5);
	}
}

void scene::shade(canvas& canvas, ssei i, ssei j, hit4& hit4)
{
	hit h;
	for (int x = 0; x < 4; ++x) {
		h.u    = hit4.u[x];
		h.v    = hit4.v[x];
		h.prim = hit4.prim[x];
		shade(canvas, i[x], j[x], h);
	}
}

void scene::draw(canvas& canvas)
{
	kdtree accel(*this);
	vec3f down, right, lefttop;
	getDownRight(*this, canvas, down, right, lefttop);
	vec3f leftedge (lefttop);

	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); ++j) {
			vec3f dir = pos - _camera.getLocation();
			ray ray (_camera.getLocation(), dir);
			hit h;
			h.prim = -1; 
                        accel.draw(*this, ray, h);
			shade(canvas, i, j, h);
			pos += right;
		}
		leftedge += down;
	}
}

void scene::drawPacket(canvas& canvas)
{
	kdtree accel(*this);
	vec3f downf, rightf, lefttopf;
	getDownRight(*this, canvas, downf, rightf, lefttopf);

	vec3<ssef> lefttop = makeSimdVec(lefttopf
					 , lefttopf + rightf
					 , lefttopf + downf
					 , lefttopf + rightf + downf);
	vec3<ssef> down  = makeSimdVec(downf + downf);
	vec3<ssef> right = makeSimdVec(rightf + rightf);
	vec3<ssef> loc   = makeSimdVec(_camera.getLocation());
	vec3<ssef> leftedge (lefttop);

	for (int i = 0; i < canvas.getHeight() ; i += 2) {
		vec3<ssef> pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); j += 2) {
			vec3<ssef> dir(pos - loc);
			ray4 ray (_camera.getLocation(), dir);
			hit4 h;
			h.prim = -1;
			accel.draw(*this, ray, h);
			shade(canvas
			      , ssei(i, i, i+1, i+1)
			      , ssei(j, j+1, j, j+1)
			      , h);
			pos += right;
		}
		leftedge += down;
	}
}

