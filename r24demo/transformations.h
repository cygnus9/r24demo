#pragma once

void translate(const float mtx_in[4][4], float mtx_out[4][4], const float x, const float *py = nullptr, const float *pz = nullptr);
void scale(const float mtx_in[4][4], float mtx_out[4][4], const float x, const float* py, const float* pz);
void xrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta);
void yrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta);
void zrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta);
void rotate(const float mtx_in[4][4], float mtx_out[4][4], float angle, float x, float y, float z);
void frustum(float mtx_out[4][4], float left, float right, float bottom, float top, float znear, float zfar);
