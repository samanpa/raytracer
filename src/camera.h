#ifndef BP_CAMERA_H
#define BP_CAMERA_H

#include "vec3.h"

class camera {
public:
	camera() :
		_position(0, 0, 1)
		, _lookat(0, 0, 0)
		, _up(0, 1, 0)
		, _angle(60) 
		{}
	
	vec3& getPosition() { return _position; }
	vec3& getLookAt() { return _lookat; }
	vec3& getUp() { return _up; }
	float getAngle() { return _angle; }

private:		
	vec3 _position;
	vec3 _lookat;
	vec3 _up;
	float _angle;
};

#endif
