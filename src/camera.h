#ifndef BP_CAMERA_H
#define BP_CAMERA_H

#include "vec3.h"

class camera {
public:
	camera() :
		_loc(0, 0, 1)
		, _lookat(0, 0, 0)
		, _up(0, 1, 0)
		, _angle(90) 
		{}
	
	vec3f& getLocation() { return _loc; }
	vec3f& getLookAt() { return _lookat; }
	vec3f& getUp() { return _up; }
	float& getAngle() { return _angle; }

private:		
	vec3f _loc;
	vec3f _lookat;
	vec3f _up;
	float _angle;
};

#endif
