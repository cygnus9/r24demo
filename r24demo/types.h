#pragma once

struct vec2 {
	float x;
	float y;
};

struct vec3 {
	float x;
	float y;
	float z;
};

struct rgba {
	float r;
	float g;
	float b;
	float a;
};

struct vec4 {
	float x;
	float y;
	float z;
	float w;
};

struct matrix4f {
	vec4 m[4];

	float get(int x, int y) const {
		return ((float*)(&m[x]))[y];
	}

	void set(int x, int y, float value) {
		((float*)(&m[x]))[y] = value;
	}

	void inc(int x, int y, float value) {
		((float*)(&m[x]))[y] += value;
	}
};
