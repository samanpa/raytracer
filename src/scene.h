#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "color.h"
#include "camera.h"
#include "triangle.h"
#include "triangle_barycentric.h"
#include <vector>
#include "cycle.h"

struct statistic {
	uint64_t val;
	uint64_t cnt;
	statistic() 
		: val (0), cnt(0) {}      
	void inc(uint64_t val) {
		this->val += val;
		++this->cnt;
	}
};

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

struct scene {
	camera _camera;
	std::vector<vec3>     _normals;
	std::vector<triangle> _triangles;
	std::vector<triangle_barycentric> _accels;
	statistic _intersectCost;
	camera& getCamera() { return _camera; }
	void draw(canvas& canvas);
	bool save(canvas& canvas, const char* filename);
	void add(vec3& p0, vec3 &p1, vec3 &p2) {
		_accels.emplace_back(p0, p1, p2);
	}
	void addNormal(vec3& normal) {
		_normals.push_back(normal);
	}
};

#endif
