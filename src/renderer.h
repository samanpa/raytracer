#ifndef BPRRAY_RENDERER_H
#define BPRRAY_RENDERER_H

#include "vec3.h"
#include "simd/simd.h"
#include "statistics.h"
#include "kdtree.h"
#include "noaccel.h"
#include "shader.h"

class scene;
class canvas;

template <class T>
struct renderer {
        renderer(scene &scene) 
                : _scene(scene)
                , _accel(scene) {}

        void draw(canvas& canvas);

        template<int SQRTN>
        void drawBundle(canvas& canvas);

        const uint64_t getRayCount() const { return _rayCount.val; }
private:
        scene &_scene;
        T _accel;
        shader _shader;
	statistic _intersectCost;
	statistic _rayCount;

        template <int SQRTN>
        void drawTile(canvas& canvas
                      , const vec3<ssef>& pos
                      , const vec3<ssef>& right
                      , const vec3<ssef>& down
                      , unsigned i, unsigned j);
       
};

#include "renderer.inl.cpp"
#endif
