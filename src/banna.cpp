#include <iostream>
#include "scene.h"
#include "parser.h"
#include "io.h"

using namespace std;

int main(int argc, char **argv, char **environ) {
	scene scene;
	canvas canvas(128, 128);

	bpray_library_path_add(".");
	if (argc > 1) {
		if (!parsePov(argv[1], scene)) {
			cerr << "Could not parse " << argv[1] << endl;
			return 1;
		}
	}

	scene.getCamera().getAngle() = 60;

	scene.draw(canvas);
	cout << "Draw took "
	     << scene._intersectCost.val / scene._intersectCost.cnt
	     << "cycles per pixel"
	     << endl;
	scene.save(canvas, "image.png");

}
