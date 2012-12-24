#ifndef BPRAY_VEC3_H
#define BPRAY_VEC3_H

#include <string.h>
#include <iostream>
#include <cmath>

enum {
	X_Axis = 0,
	Y_Axis,
	Z_Axis
};

class vec3 {
public:
	vec3() {
	}

	vec3(vec3&& v) {
		_v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2];
	}

	vec3(const vec3& v) {
		_v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2];
	}

	vec3(float x, float y, float z) {
		_v[0] = x; _v[1] = y; _v[2] = z; 
	}

	vec3& operator=(const vec3& v) {
		_v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2];
		return *this;
	}

	vec3& operator-=(const vec3& v) {
		_v[0] -= v._v[0]; _v[1] -= v._v[1]; _v[2] -= v._v[2];
		return *this;
	}

	vec3& operator+=(const vec3& v) {
		_v[0] += v._v[0]; _v[1] += v._v[1]; _v[2] += v._v[2];
		return *this;
	}

	vec3& operator*=(const float s) {
		_v[0] *= s; _v[1] *= s; _v[2] *= s;
		return *this;
	}

	vec3 operator*(const float s) {
		return vec3(_v[0]*s, _v[1]*s, _v[2]*s);
	}

	vec3 operator-(const vec3& v) {
		return vec3(_v[0]-v._v[0], _v[1]-v._v[1], _v[2]-v._v[2]);
	}

	vec3 operator+(const vec3& v) {
		return vec3(_v[0]+v._v[0], _v[1]+v._v[1], _v[2]+v._v[2]);
	}

	float &operator [](int idx) {
		return _v[idx];
	}
	
	float getMagnitude() {
		return ::sqrt(dot(*this, *this));
	}

	friend std::ostream& operator<<(std::ostream& o, vec3& v) {
		o << "<" << v[0] << "," << v[1] << "," << v[2] << ">";
		return o;
	}

	static float dot(vec3& v1, vec3& v2) {
		return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	}

	static vec3 cross(vec3 &v1, vec3 &v2) {
		vec3 v(v1._v[1]*v2._v[2] - v1._v[2]*v2._v[1],
		       v1._v[2]*v2._v[0] - v1._v[0]*v2._v[2],
		       v1._v[0]*v2._v[1] - v1._v[1]*v2._v[0]);
		return v;
	}

private:
	float _v[3];
};

#endif
