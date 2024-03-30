#include <cassert>
#include <cstring>
#include <cmath>
#include <numbers>

namespace {
    void dot_product(const float mtx_a[4][4], const float mtx_b[4][4], float mtx_out[4][4]) {
        if (mtx_a == mtx_out) {
            float mtx_temp[4][4];
            dot_product(mtx_a, mtx_b, mtx_temp);
            memcpy(mtx_out, mtx_temp, sizeof(mtx_temp));
        } else {
            assert(mtx_b != mtx_out);
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    mtx_out[x][y] = 0;
                    for (int n = 0; n < 4; ++n) {
                        mtx_out[x][y] += mtx_a[x][n] * mtx_b[n][y];
                    }
                }
            }
        }
    }
}

void translate(const float mtx_in[4][4], float mtx_out[4][4], const float x, const float *py, const float *pz) {
    const float y = py ? *py : x;
    const float z = pz ? *pz : x;

    const float mtx_transform[4][4] = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { x, y, z, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void scale(const float mtx_in[4][4], float mtx_out[4][4], const float x, const float *py, const float *pz) {
    const float y = py ? *py : x;
    const float z = pz ? *pz : x;

    const float mtx_transform[4][4] = {
        { x, 0, 0, 0 },
        { 0, y, 0, 0 },
        { 0, 0, z, 0 },
        { 0, 0, 0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void xrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const float mtx_transform[4][4] = {
        { 1,    0,    0, 0 },
        { 0, cosT, sinT, 0 },
        { 0, sinT, cosT, 0 },
        { 0,    0,    0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void yrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const float mtx_transform[4][4] = {
        {  cosT, 0, sinT, 0 },
        {     0, 1,    0, 0 },
        { -sinT, 0, cosT, 0 },
        {     0, 0,    0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void zrotate(const float mtx_in[4][4], float mtx_out[4][4], float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const float mtx_transform[4][4] = {
        { cosT, -sinT, 0, 0 },
        { sinT,  cosT, 0, 0 },
        {    0,     0, 1, 0 },
        {    0,     0, 0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void rotate(const float mtx_in[4][4], float mtx_out[4][4], float angle, float x, float y, float z) {
    const float a = std::numbers::pi_v<float> * angle / 180.0f;
    const float c = std::cos(a);
    const float s = std::sin(a);
    const float n = std::sqrt(x*x + y*y + z*z);
    x /= n;
    y /= n;
    z /= n;
    const float cx = (1 - c) * x;
    const float cy = (1 - c) * y;
    const float cz = (1 - c) * z;
    const float mtx_transform[4][4] = {
        { cx*x +   c, cx*y + z*s, cx*z - y*s, 0},
        { cy*x - z*s, cy*y +   c, cy*z + x*s, 0},
        { cz*x + y*s, cz*y - x*s, cz*z +   c, 0},
        {          0,          0,          0, 1},
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void frustum(float mtx_out[4][4], float left, float right, float bottom, float top, float znear, float zfar) {
    assert(right  != left );
    assert(bottom != top  );
    assert(znear  != zfar );

    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            mtx_out[x][y] = 0;
        }
    }

    mtx_out[0][0] = +2.0f * znear / (right - left);
    mtx_out[2][0] = (right + left) / (right - left);
    mtx_out[1][1] = +2.0f * znear / (top - bottom);
    mtx_out[3][1] = (top + bottom) / (top - bottom);
    mtx_out[2][2] = -(zfar + znear) / (zfar - znear);
    mtx_out[3][2] = -2.0f * znear * zfar / (zfar - znear);
    mtx_out[2][3] = -1.0f;
}
