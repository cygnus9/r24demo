#pragma once

#include "types.h"

void translate(const matrix4f &mtx_in, matrix4f &mtx_out, const float x, const float y, const float z);
void scale(const matrix4f &mtx_in, matrix4f &mtx_out, const float x, const float y, const float z);
void xrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta);
void yrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta);
void zrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta);
void rotate(const matrix4f &mtx_in, matrix4f &mtx_out, float angle, float x, float y, float z);
void frustum(matrix4f &mtx_out, float left, float right, float bottom, float top, float znear, float zfar);
void identity(matrix4f &mtx_out);