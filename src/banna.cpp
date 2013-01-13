#include <iostream>
#include "scene.h"
#include "parser.h"
#include "io.h"
#include "utils.h"

using namespace std;

int main(int argc, char **argv, char **environ) {
	scene scene;
	canvas canvas(1024, 1024);
	camera& camera = scene.getCamera();

	bpray_library_path_add(".");
	INFO(argv[0]);
	if (argc > 1) {
		try {
			parsePov(argv[1], scene);
		}
		catch (parse_error& pe) {
			cerr << "Could not parse " << argv[1]
			     << " " << pe.what() << endl;
			return 1;
		}
	}

	INFO("Done parsing");
	camera.getAngle() = 15;
	camera.getLookAt().y() = 0.1;

#if 0
	scene.drawPacket(canvas);
#else
	scene.draw(canvas);
#endif
	if (scene._intersectCost.cnt) {
		INFO( "Draw took "
		      << scene._intersectCost.val / scene._intersectCost.cnt
		      << "cycles per pixel " << scene._intersectCost.cnt);
	} else
                INFO("Draw done");
	canvas.save("image.png");

}
