#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "canvas.h"
#include "camera.h"
#include "triangle.h"
#include "barycentric.h"
#include "cycle.h"
#include "statistics.h"
#include <vector>

struct scene {
	camera _camera;
	std::vector<vec3f>    _vertices;
	std::vector<ssef>    _normals;
	std::vector<triangle> _triangles;
	std::vector<triangle_barycentric> _accels;
	statistic _intersectCost;
	statistic _rayCount;

	camera& getCamera() { return _camera; }
        template <class T>
	void draw(T& accel, canvas& canvas);
        template <class T>
	void draw4(T& accel, canvas& canvas);

        void addNormal(vec3f& n) { _normals.push_back(ssef(n.x(), n.y(), n.z(), 0.f)); }
        const ssef& getNormal(size_t v) const { return _normals[v]; }
	void addVertex(vec3f& vertex) { _vertices.push_back(vertex); }
	size_t getNumVertices() const { return _vertices.size(); }
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
