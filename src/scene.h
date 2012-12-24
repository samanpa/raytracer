#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "color.h"
#include "camera.h"
#include "triangle.h"
#include <vector>

class canvas {
public:
	canvas (short height, short width);
	color& operator ()(short h, short w) {
		return _colors[h][w];
	}
	int getWidth() { return _width; }
	int getHeight() { return _height; }
private:
	color **_colors;
	int _width;
	int _height;
};

class scene {
	camera _camera;
	std::vector<triangle> _triangles;
public:
	camera& getCamera() { return _camera; }
	void draw(canvas& canvas);
	bool save(canvas& canvas, const char* filename);
	void add(vec3& p0, vec3 &p1, vec3 &p2) {
		_triangles.emplace_back(p0, p1, p2);
	}
};

#endif
