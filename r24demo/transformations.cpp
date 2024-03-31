#include <cassert>
#include <cstring>
#include <cmath>
#include <numbers>

#include "transformations.h"


namespace {
    void dot_product(const matrix4f &mtx_a, const matrix4f &mtx_b, matrix4f &mtx_out) {
        if (&mtx_a == &mtx_out) {
            matrix4f mtx_temp;
            dot_product(mtx_a, mtx_b, mtx_temp);
            memcpy(&mtx_out, &mtx_temp, sizeof(mtx_temp));
        } else {
            assert(&mtx_b != &mtx_out);
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    mtx_out.set(x, y, 0);
                    for (int n = 0; n < 4; ++n) {
                        mtx_out.inc(x, y, mtx_a.get(x, n) * mtx_b.get(n, y));
                    }
                }
            }
        }
    }
}

void translate(const matrix4f &mtx_in, matrix4f &mtx_out, const float x, const float y, const float z) {
    const matrix4f mtx_transform = {
        vec4 { 1, 0, 0, 0 },
        vec4 { 0, 1, 0, 0 },
        vec4 { 0, 0, 1, 0 },
        vec4 { x, y, z, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void scale(const matrix4f &mtx_in, matrix4f &mtx_out, const float x, const float y, const float z) {
    const matrix4f mtx_transform = {
        vec4 { x, 0, 0, 0 },
        vec4 { 0, y, 0, 0 },
        vec4 { 0, 0, z, 0 },
        vec4 { 0, 0, 0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void xrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const matrix4f mtx_transform = {
        vec4 { 1,    0,    0, 0 },
        vec4 { 0, cosT, sinT, 0 },
        vec4 { 0, sinT, cosT, 0 },
        vec4 { 0,    0,    0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void yrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const matrix4f mtx_transform = {
        vec4 {  cosT, 0, sinT, 0 },
        vec4 {     0, 1,    0, 0 },
        vec4 { -sinT, 0, cosT, 0 },
        vec4 {     0, 0,    0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void zrotate(const matrix4f &mtx_in, matrix4f &mtx_out, float theta) {
    const float t = std::numbers::pi_v<float> * theta / 180.0f;
    const float cosT = std::cos(t);
    const float sinT = std::sin(t);

    const matrix4f mtx_transform = {
        vec4 { cosT, -sinT, 0, 0 },
        vec4 { sinT,  cosT, 0, 0 },
        vec4 {    0,     0, 1, 0 },
        vec4 {    0,     0, 0, 1 },
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void rotate(const matrix4f &mtx_in, matrix4f &mtx_out, float angle, float x, float y, float z) {
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
    const matrix4f &mtx_transform = {
        vec4 { cx*x +   c, cx*y + z*s, cx*z - y*s, 0},
        vec4 { cy*x - z*s, cy*y +   c, cy*z + x*s, 0},
        vec4 { cz*x + y*s, cz*y - x*s, cz*z +   c, 0},
        vec4 {          0,          0,          0, 1},
    };
    dot_product(mtx_in, mtx_transform, mtx_out);
}

void frustum(matrix4f &mtx_out, float left, float right, float bottom, float top, float znear, float zfar) {
    assert(right  != left );
    assert(bottom != top  );
    assert(znear  != zfar );

    memset(&mtx_out, 0, sizeof(mtx_out));

    mtx_out.set(0, 0, +2.0f * znear / (right - left));
    mtx_out.set(2, 0, (right + left) / (right - left));
    mtx_out.set(1, 1, +2.0f * znear / (top - bottom));
    mtx_out.set(3, 1, (top + bottom) / (top - bottom));
    mtx_out.set(2, 2, -(zfar + znear) / (zfar - znear));
    mtx_out.set(3, 2, -2.0f * znear * zfar / (zfar - znear));
    mtx_out.set(2, 3, -1.0f);
}

void identity(matrix4f &mtx_out)
{
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {
            mtx_out.set(x, y, 0);
        }
    }

    mtx_out.set(0, 0, 1.0);
    mtx_out.set(1, 1, 1.0);
    mtx_out.set(2, 2, 1.0);
    mtx_out.set(3, 3, 1.0);
}
