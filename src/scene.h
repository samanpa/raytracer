#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "canvas.h"
#include "color.h"
#include "camera.h"
#include "triangle.h"
#include "barycentric.h"
#include "cycle.h"
#include "statistics.h"
#include <vector>

struct scene {
	camera _camera;
	std::vector<vec3f>    _vertices;
	std::vector<vec3f>    _normals;
	std::vector<triangle> _triangles;
	std::vector<triangle_barycentric> _accels;
	statistic _intersectCost;

	camera& getCamera() { return _camera; }
	void draw(canvas& canvas);
	void drawPacket(canvas& canvas);
	void shade(canvas& canvas, unsigned i, unsigned j, hit& hit);
	void shade(canvas& canvas, ssei i, ssei j, hit4& hit);

	void addNormal(vec3f& normal) { _normals.push_back(normal); }
	void addVertex(vec3f& vertex) { _vertices.push_back(vertex); }
	size_t getNumVertices() { return _vertices.size(); }
	const std::vector<triangle>& getTriangles() const { return _triangles; }
	const std::vector<vec3f>& getVertices() const { return _vertices; }
	bool addFace(size_t v0, size_t v1, size_t v2) {
		if (v0 > _vertices.size()
		    || v1 > _vertices.size() 
		    || v2 > _vertices.size() ) 
			return false;
		_accels.emplace_back(_vertices[v0], _vertices[v1], _vertices[v2]);
		_triangles.emplace_back(v0, v1, v2);
		return true;
	}
};

#endif
