#ifndef BPRAY_UI_H
#define BPRAY_UI_H

#include <scene.h>

class UI {
        canvas _canvas;
        bool _usegl;
        void (*_renderer)(canvas&);
        scene& _scene;

public:
        UI(scene& scene
           , int h, int w
           , void (*renderer)(canvas &)
           , bool usegl) 
                : _canvas(h, w)
                , _usegl(usegl)
                , _renderer(renderer)
                , _scene(scene) {
        }

        void draw();
        void drawScene();
        void renderCanvas();

        camera& getCamera() { return _scene.getCamera(); }
};

#endif
