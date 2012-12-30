#ifndef BPRAY_VEC3_H
#define BPRAY_VEC3_H

#include <string.h>
#include <iostream>
#include <cmath>
#include "platform.h"
#include "float.h"

enum {
	X_Axis = 0,
	Y_Axis,
	Z_Axis
};

template<class T>
class vec3 {
public:
	__forceinline vec3() {}
	__forceinline vec3(T x, T y, T z) {_v[0] = x; _v[1] = y; _v[2] = z; }
	__forceinline vec3(vec3&& v) {_v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2];}
	__forceinline vec3(const vec3& v) {_v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2];}

	__forceinline vec3& operator=(const vec3& v) {
		_v[0] = v[0]; _v[1] = v._v[1]; _v[2] = v._v[2];
		return *this;
	}

	__forceinline T &operator [](int idx) { return _v[idx]; }
	__forceinline const T &operator [](int idx) const { return _v[idx]; }

	//Math
	__forceinline vec3& operator*=(const T& s) {
		_v[0] *= s; _v[1] *= s; _v[2] *= s;
		return *this;
	}
	__forceinline vec3 operator*(const T& s) {
		return vec3(_v[0]*s, _v[1]*s, _v[2]*s);
	}
	__forceinline vec3& operator-=(const vec3& v) {
		_v[0] -= v[0]; _v[1] -= v[1]; _v[2] -= v[2];
		return *this;
	}
	__forceinline vec3& operator+=(const vec3& v) {
		_v[0] += v[0]; _v[1] += v[1]; _v[2] += v[2];
		return *this;
	}
	__forceinline vec3 operator-(const vec3& v) {
		return vec3(_v[0]-v._v[0], _v[1]-v._v[1], _v[2]-v._v[2]);
	}
	__forceinline vec3 operator+(const vec3& v) {
		return vec3(_v[0]+v._v[0], _v[1]+v._v[1], _v[2]+v._v[2]);
	}
	
	friend std::ostream& operator<<(std::ostream& o, const vec3& v) {
		o << "<" << v[0] << "," << v[1] << "," << v[2] << ">";
		return o;
	}

	const T& x() const { return _v[0]; }
	const T& y() const { return _v[1]; }
	const T& z() const { return _v[2]; }

private:
	T _v[3];
};

template<typename T>
__forceinline T length(const vec3<T> &v) {
	return sqrt (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
template<typename T>
__forceinline float dot(const vec3<T>& v1, const vec3<T>& v2) {
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}
template<typename T>
__forceinline vec3<T> cross(vec3<T> &v1, vec3<T> &v2) {
	vec3<T> v(v1[1]*v2[2] - v1[2]*v2[1],
		  v1[2]*v2[0] - v1[0]*v2[2],
		  v1[0]*v2[1] - v1[1]*v2[0]);
	return v;
}

template <typename T>
__forceinline void normalize(vec3<T> &v) {
	v *= rcp(length(v));
}

template <typename T>
__forceinline vec3<T> rcp(vec3<T>& v) {
	return vec3<T>(rcp(v[0]), rcp(v[1]), rcp(v[2]));
}

typedef vec3<float> vec3f;

#endif
