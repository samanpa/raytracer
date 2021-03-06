#include "scene.h"

bool scene::addFace(size_t v0, size_t v1, size_t v2) {
        if (v0 > _vertices.size()
            || v1 > _vertices.size() 
            || v2 > _vertices.size() ) 
                return false;
        _accels.emplace_back(_vertices[v0], _vertices[v1], _vertices[v2]);
        _triangles.emplace_back(v0, v1, v2);
        return true;
}

void scene::postProcess() {
        for (auto &t : _triangles) {
                _faceNormals.push_back(_normals[t.p0]);
                _faceNormals.push_back(_normals[t.p1]);
                _faceNormals.push_back(_normals[t.p2]);
        }
        _normals.clear();
}
