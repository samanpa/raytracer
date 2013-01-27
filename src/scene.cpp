#include "scene.h"
#include "ray.h"
#include <cmath>
#include "kdtree.h"
#include "noaccel.h"
#include "simdv.h"
#include "utils.h"
#include "shader.h"

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

template <typename T>
void scene::draw(T& accel, canvas& canvas)
{
	vec3f down, right, lefttop;
	getDownRight(*this, canvas, down, right, lefttop);
	vec3f leftedge (lefttop);
        shader shader;

	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); ++j) {
			vec3f dir = pos - _camera.getLocation();
			ray ray (_camera.getLocation(), dir);
			hit h;
                        accel.draw(*this, ray, h);
			shader.shade(*this, canvas, i, j, h);
			pos += right;
		}
		leftedge += down;
	}
}

template <typename T>
void scene::draw4(T& accel, canvas& canvas)
{
	vec3f downf, rightf, lefttopf;
	getDownRight(*this, canvas, downf, rightf, lefttopf);
        shader shader;

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
			accel.draw(*this, ray, h);
			shader.shade(*this, canvas
                                     , ssei(i, i, i+1, i+1)
                                     , ssei(j, j+1, j, j+1)
                                     , h);
			pos += right;
		}
		leftedge += down;
	}
}

template void scene::draw<kdtree>(kdtree& accel, canvas& canvas);
template void scene::draw4<kdtree>(kdtree& accel, canvas& canvas);
template void scene::draw<noaccel>(noaccel& accel, canvas& canvas);
template void scene::draw4<noaccel>(noaccel& accel, canvas& canvas);
