#ifndef BP_CAMERA_H
#define BP_CAMERA_H

#include "vec3.h"

class camera {
public:
        camera() :
                _loc(0, 0, 1)
                , _lookat(0, 0, 0)
                , _up(0, 1, 0)
                , _angle(90) {}
                
        const vec3f& getLocation() const { return _loc; }
        void setLocation(const vec3f& loc) { _loc = loc; }

        const vec3f& getLookAt() const { return _lookat; }
        void setLookAt(const vec3f& lookat) { _lookat = lookat; }

        const vec3f& getUp() const { return _up; }
        void setUp(const vec3f& up) { _up = up; }

        float getAngle() const { return _angle; }
        void setAngle(float angle) { if (angle < 180) _angle = angle; }
private:
        vec3f _loc;
        vec3f _lookat;
        vec3f _up;
        float _angle;
};

#endif
