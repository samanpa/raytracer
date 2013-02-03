#include <iostream>
#include "scene.h"
#include "parser.h"
#include "io.h"
#include "utils.h"
#include "cycle.h"
#include "ui.h"

using namespace std;

int main(int argc, char **argv, char **environ) {
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

        scene scene;
        canvas canvas(1024, 1024);
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
        //camera.setLocation(vec3f(0, 0 ,-4));

        UI ui(scene, canvas, usegl);
        ui.draw();
}
