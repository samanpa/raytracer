#include <iostream>
#include "scene.h"
using namespace std;

int main(int argc, char **argv, char **environ) {
	scene scene;
	canvas canvas(1024, 1024);
	scene.draw(canvas);
	scene.save(canvas, "image.png");
	cout << argv[0] << endl;
}
