#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "canvas.h"
#include "color.h"
#include "camera.h"
#include "triangle.h"
#include "barycentric.h"
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

struct scene {
	camera _camera;
	std::vector<vec3f>    _normals;
	std::vector<triangle> _triangles;
	std::vector<triangle_barycentric> _accels;
	statistic _intersectCost;
	camera& getCamera() { return _camera; }
	void draw(canvas& canvas);
	void draw4(canvas& canvas);
	void shade(canvas& canvas, unsigned i, unsigned j, hit& hit);
	void shade(canvas& canvas, unsigned i, unsigned j, hit4& hit);
	bool save(canvas& canvas, const char* filename);
	void add(vec3f& p0, vec3f &p1, vec3f &p2) {
		_accels.emplace_back(p0, p1, p2);
	}
	void addNormal(vec3f& normal) {
		_normals.push_back(normal);
	}
};

#endif
