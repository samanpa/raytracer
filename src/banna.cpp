#include <iostream>
#include "scene.h"
#include <time.h>
#include "parser.h"
#include "io.h"

static uint64_t gettime() {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000 * 1000 * 1000 +  ts.tv_nsec;
}

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

	uint64_t start = gettime();
	scene.draw(canvas);
	uint64_t end   = gettime();

	cout << "Draw took "
	     << (end - start) / canvas.getHeight() / canvas.getWidth() /1000.
	     << "us per pixel"
	     << endl;
	scene.save(canvas, "image.png");

}
