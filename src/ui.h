#ifndef BPRAY_UI_H
#define BPRAY_UI_H

#include <scene.h>
#include "kdtree.h"
#include "noaccel.h"

class UI {
        scene& _scene;
        canvas& _canvas;
        bool _usegl;
        kdtreebenthin _accel;

public:
        UI(scene& scene, canvas& canvas, bool usegl = true) 
                : _scene(scene)
                , _canvas(canvas)
                , _usegl(usegl)
                , _accel(scene) {
        }

        void draw();
        void drawScene();
        void renderCanvas();

        camera& getCamera() { return _scene.getCamera(); }
};

#endif
