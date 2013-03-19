#include "renderer.h"
#include "scene.h"
#include "ray.h"
#include <cmath>
#include "kdtree.h"
#include "noaccel.h"
#include "simdv.h"
#include "utils.h"
#include "color.h"

using namespace std;

static void setColors1x1(canvas& canvas, color* colors, unsigned i, unsigned j)
{
        canvas.get(i, j) = *colors;
}

static void setColors2x2(canvas& canvas, color* colors, unsigned i, unsigned j)
{
        setColors1x1(canvas, colors + 0, i + 0, j + 0);
        setColors1x1(canvas, colors + 1, i + 0, j + 1);
        setColors1x1(canvas, colors + 2, i + 1, j + 0);
        setColors1x1(canvas, colors + 3, i + 1, j + 1);
}

template <int N>
inline void setColors(canvas& canvas, color* colors, unsigned i, unsigned j)
{
        unsigned idx = 0;
        for (unsigned h = 0; h < N; ++h)
                for (unsigned w = 0; w < N; ++w) {
                        setColors2x2(canvas
                                     , colors + 4 * idx
                                     , i + 2 * h
                                     , j + 2 * w);
                        ++idx;
                }
}



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

template <class T>
void renderer<T>::draw(canvas& canvas)
{
	vec3f down, right, lefttop;
	getDownRight(_scene, canvas, down, right, lefttop);
	vec3f leftedge (lefttop);
        auto& camera = _scene.getCamera();
        _rayCount.reset();

	for (int i = 0; i < canvas.getHeight() ; ++i) {
		vec3f pos(leftedge);
		for (int j = 0; j < canvas.getWidth(); ++j) {
                        _rayCount.inc(1);
			vec3f dir = pos - camera.getLocation();
                        normalize(dir);
			ray ray (camera.getLocation(), dir);
			hit h;
                        _accel.draw(_scene, ray, h);
			_shader.shade(_scene, ray, canvas.get(i, j), h);
			pos += right;
		}
		leftedge += down;
	}
}


template <int SQRTN>
inline void makeRays(const vec3<ssef>& oleftedge
                     , const vec3f& cameraLoc
                     , const vec3<ssef>& right
                     , const vec3<ssef>& down
                     , const unsigned i
                     , const unsigned j
                     , ray4 *ray
                     , hit4 *h)
{
	vec3<ssef> loc   = makeSimdVec(cameraLoc);
        vec3<ssef> leftedge(oleftedge);
        ssef ni = (float)i/2;
        ssef nj = (float)j/2;
        leftedge += mul(right, vec3<ssef>(nj, nj, nj));
        leftedge += mul(down,  vec3<ssef>(ni, ni, ni));
        
        unsigned k = 0;
        for (unsigned k1 = 0; k1 < SQRTN; ++k1) {
                vec3<ssef> pos(leftedge);
                for (unsigned k2 = 0; k2 < SQRTN; ++k2) {
                        vec3<ssef> dir(pos - loc);
                        normalize(dir);
                        ray[k].init(cameraLoc, dir);
                        pos   += right;
                        ++k;
                }
                leftedge += down;
        }
}
                            
//N == number of bundles
template <class T>
template <int SQRTN>
void renderer<T>::drawTile(canvas& canvas
                           , const vec3<ssef>& leftedge
                           , const vec3<ssef>& right
                           , const vec3<ssef>& down
                           , unsigned i, unsigned j)
{
        static const unsigned N = SQRTN * SQRTN;
        color colors[4*N];
        auto& camera = _scene.getCamera();
        ray4 ray[N];
        hit4 h[N];
        makeRays<SQRTN>(leftedge, camera.getLocation(), right, down, i, j
                        , ray, h);
#if 1
        for (unsigned k = 0; k < N; ++k)
                _accel.template draw<1>(_scene, ray+k, h+k);
#else
        _accel.template draw<N>(_scene, ray, h);
#endif
        _shader.shade(_scene, ray, colors, h, N);
        setColors<SQRTN>(canvas, colors, i, j);
        _rayCount.inc(4 * N);
}

template <class T>
template <int SQRTN>
void renderer<T>::drawBundle(canvas& canvas)
{
	vec3f downf, rightf, lefttopf;
	getDownRight(_scene, canvas, downf, rightf, lefttopf);
	vec3<ssef> lefttop = makeSimdVec(lefttopf
					 , lefttopf + rightf
					 , lefttopf + downf
					 , lefttopf + rightf + downf);
	vec3<ssef> down  = makeSimdVec(downf + downf);
	vec3<ssef> right = makeSimdVec(rightf + rightf);
	vec3<ssef> leftedge (lefttop);
        _rayCount.reset();

	for (int i = 0; i < canvas.getHeight() ; i += 2 * SQRTN) {
		for (int j = 0; j < canvas.getWidth(); j += 2 * SQRTN) {
                        drawTile<SQRTN>(canvas
                                        , leftedge, right, down
                                        , i, j);
		}
	}
}

