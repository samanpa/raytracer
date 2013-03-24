#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "canvas.h"
#include "camera.h"
#include "triangle.h"
#include "barycentric.h"
#include "cycle.h"
#include "ray.h"
#include <vector>

class scene {
	camera _camera;
        std::vector<ssef>     _faceNormals;
        std::vector<triangle> _triangles;
        std::vector<vec3f>    _vertices;
        std::vector<ssef>     _normals;

public:
        std::vector<triangle_barycentric> _accels;

public:
	camera& getCamera() { return _camera; }
	const camera& getCamera() const { return _camera; }
	void addVertex(vec3f& vertex) { _vertices.push_back(vertex); }
	size_t getNumVertices() const { return _vertices.size(); }
	const std::vector<triangle>& getTriangles() const { return _triangles; }
	const triangle& getTriangle(size_t id) const { return _triangles[id]; }
        const vec3f& getVertex(size_t id) const { return _vertices[id]; }
	const std::vector<vec3f>& getVertices() const { return _vertices; }

        void postProcess();

        void addNormal(vec3f& n) { 
                _normals.push_back(ssef(n.x(), n.y(), n.z(), 0.f));
        }
        const ssef& getFaceNormal(uint32_t t, uint32_t n) const { return _faceNormals[t*3+n];}

	bool addFace(size_t v0, size_t v1, size_t v2);
        
        __forceinline void intersect(uint32_t p, ray& ray, hit& hit) {
                _accels[p].intersect(p, ray, hit);
        }

        __forceinline void intersect(uint32_t p, ray4& ray, hit4& hit) {
                _accels[p].intersect(p, ray, hit);
        }
};

#endif
