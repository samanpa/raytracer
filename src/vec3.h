#ifndef BPRAY_VEC3_H
#define BPRAY_VEC3_H

enum {
	X_Axis = 0,
	Y_Axis,
	Z_Axis
};

class vec3 {
public:
	vec3() {
	}

	vec3(float x, float y, float z) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}

	float operator [](int idx) {
		return _data[idx];
	}
private:
	float _data[3];
};

#endif
