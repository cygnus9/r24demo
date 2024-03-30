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
};
