#ifndef BPRAY_SCENE_H
#define BPRAY_SCENE_H

#include "camera.h"

struct color {
	float getRed() { return _rgb[0]; }
	float getGreen() { return _rgb[1]; }
	float getBlue() { return _rgb[2]; }

	void setRed(float r) { _rgb[0] = r; }
	void setGreen(float g) {_rgb[1] = g; }
	void setBlue(float b) {_rgb[2] = b; }
	void set(float r, float g, float b) {
		_rgb[0] = r;
		_rgb[1] = g;
		_rgb[2] = b;
	}
private:
	float _rgb[3];
};
	
class canvas {
public:
	canvas (short height, short width);
	color& operator ()(short h, short w) {
		return _colors[h][w];
	}
	int getWidth() { return _width; }
	int getHeight() { return _height; }
private:
	color **_colors;
	int _width;
	int _height;
};

class scene {
	camera _camera;
	
public:
	camera& getCamera() { return _camera; }
	void draw(canvas& canvas);
	bool save(canvas& canvas, const char* filename);
};

#endif
