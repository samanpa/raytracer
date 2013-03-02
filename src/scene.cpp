#include "scene.h"
#include "ray.h"
#include <cmath>
#include "kdtree.h"
#include "noaccel.h"
#include "simdv.h"
#include "utils.h"
#include "shader.h"
#include "color.h"

using namespace std;

static void getDownRight(const scene &scene, const canvas &canvas,
			 vec3f& down, vec3f& right, vec3f& lefttop) {
	const auto& cam = scene.getCamera();
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
        _rayCount.reset();

	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); ++j) {
                        _rayCount.inc(1);
			vec3f dir = pos - _camera.getLocation();
                        normalize(dir);
			ray ray (_camera.getLocation(), dir);
			hit h;
                        accel.draw(*this, ray, h);
			shader.shade(*this, ray, canvas.get(i, j), h);
			pos += right;
		}
		leftedge += down;
	}
}

template <typename T>
void scene::draw4(T& accel, canvas& canvas)
{
        const int N = 1;
	vec3f downf, rightf, lefttopf;
	getDownRight(*this, canvas, downf, rightf, lefttopf);
        shader shader;
        color colors[4 * N];

	vec3<ssef> lefttop = makeSimdVec(lefttopf
					 , lefttopf + rightf
					 , lefttopf + downf
					 , lefttopf + rightf + downf);
	vec3<ssef> down  = makeSimdVec(downf + downf);
	vec3<ssef> right = makeSimdVec(rightf + rightf);
	vec3<ssef> loc   = makeSimdVec(_camera.getLocation());
	vec3<ssef> leftedge (lefttop);
        _rayCount.reset();

	for (int i = 0; i < canvas.getHeight() ; i += 2) {
		vec3<ssef> pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); j += 2) {
                        _rayCount.inc(4);
			vec3<ssef> dir(pos - loc);
                        normalize(dir);
			ray4 ray (_camera.getLocation(), dir);
			hit4 h;
			accel.draw(*this, ray, h);
			shader.shade(*this, &ray, colors, &h, N);
                        canvas.get(i  , j  ) = colors[0];
                        canvas.get(i  , j+1) = colors[1];
                        canvas.get(i+1, j  ) = colors[2];
                        canvas.get(i+1, j+1) = colors[3];
			pos += right;
		}
		leftedge += down;
	}
}

template void scene::draw<kdtreewachter>(kdtreewachter& accel, canvas& canvas);
template void scene::draw4<kdtreewachter>(kdtreewachter& accel, canvas& canvas);

template void scene::draw<kdtreebenthin>(kdtreebenthin& accel, canvas& canvas);
template void scene::draw4<kdtreebenthin>(kdtreebenthin& accel, canvas& canvas);

template void scene::draw<noaccel>(noaccel& accel, canvas& canvas);
template void scene::draw4<noaccel>(noaccel& accel, canvas& canvas);
