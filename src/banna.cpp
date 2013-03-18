#include <iostream>
#include "scene.h"
#include "parser.h"
#include "io.h"
#include "utils.h"
#include "cycle.h"
#include "ui.h"
#include "renderer.h"

using namespace std;

static renderer<kdtreebenthin> *arenderer;
void drawit (canvas &canvas) {
        ticks start = getticks();
        arenderer->drawBundle<1>(canvas);
        ticks end = getticks();
        INFO( "Cycles per ray " << (end - start) / arenderer->getRayCount());
}


int main(int argc, char **argv, char **environ) {
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

        scene scene;
        camera& camera = scene.getCamera();
        bool usegl = true;

        bpray_library_path_add(".");
        INFO(argv[0]);
        if (argc == 1)
                return 1;

        for (int i = 1; i < argc; ++i) {
                if (strcmp(argv[i], "--nogl") == 0)
                        usegl = false;
                else {
                        try {
                                parsePov(argv[i], scene);
                                arenderer = new renderer<kdtreebenthin>(scene);
                        }
                        catch (parse_error& pe) {
                                cerr << "Could not parse " << argv[1]
                                     << " " << pe.what() << endl;
                                return 1;
                        }
                }
        }

        INFO("Number of prims " << scene.getTriangles().size());
        camera.setAngle(15);
        camera.setLookAt(vec3f(0, 0.1, 0));

        UI ui(scene, 1024, 1024, drawit, usegl);
        ui.draw();
}
