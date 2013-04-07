#ifndef BPRRAY_RENDERER_H
#define BPRRAY_RENDERER_H

#include "vec3.h"
#include "simd/simd.h"
#include "statistics.h"
#include "kdtree.h"
#include "noaccel.h"
#include "shader.h"
#include "jobmanager.h"

class scene;
class canvas;

template <class T, int SQRTN>
struct renderer {
        renderer(scene &scene);

        void draw(canvas& canvas);
        void draw4(canvas& canvas);
        void drawThreaded(canvas& canvas);

        const uint64_t getRayCount() const { return _rayCount.val; }
private:
        scene &_scene;
        T _accel;
        shader _shader;
	statistic _intersectCost;
	statistic _rayCount;
        jobmanager _jobmanager;

        void drawTile(color* colors
                      , const vec3<ssef>& pos
                      , const vec3<ssef>& right
                      , const vec3<ssef>& down
                      , unsigned i, unsigned j);

        static void *renderThread(void *tid);
};

#include "renderer.inl.cpp"
#endif
