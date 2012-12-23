#ifndef BPRAY_COLOR_H
#define BPRAY_COLOR_H

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
	


#endif
