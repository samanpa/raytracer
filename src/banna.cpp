#include <iostream>
#include "scene.h"
#include "parser.h"
#include "io.h"

using namespace std;

int main(int argc, char **argv, char **environ) {
	scene scene;
	canvas canvas(512, 512);

	bpray_library_path_add(".");
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

	scene.getCamera().getAngle() = 56;
	scene.draw(canvas);
	cout << "Draw took "
	     << scene._intersectCost.val / scene._intersectCost.cnt
	     << "cycles per pixel"
	     << endl;
	canvas.save("image.png");

}
