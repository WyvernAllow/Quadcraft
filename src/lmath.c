#include "lmath.h"

#include <math.h>

struct vec3 vec3_add(struct vec3 a, struct vec3 b) {
    struct vec3 result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
    return result;
}

struct vec3 vec3_sub(struct vec3 a, struct vec3 b) {
    struct vec3 result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
    return result;
}

struct vec3 vec3_scale(struct vec3 v, float s) {
    struct vec3 result = {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s,
    };
    return result;
}

float vec3_dot(struct vec3 a, struct vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct vec3 vec3_cross(struct vec3 a, struct vec3 b) {
    struct vec3 result = {
        .x = a.y * b.z - a.z * b.y,
        .y = a.z * b.x - a.x * b.z,
        .z = a.x * b.y - a.y * b.x,
    };
    return result;
}

float vec3_len(struct vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

struct vec3 vec3_normalize(struct vec3 v) {
    float len = vec3_len(v);
    if (len == 0.0f) {
        return (struct vec3){0.0f, 0.0f, 0.0f};
    }
    return vec3_scale(v, 1.0f / len);
}

void mat4_identity(struct mat4 *out) {
    for (int i = 0; i < 16; ++i) {
        out->m[i] = 0.0f;
    }

    out->m[0] = 1.0f;
    out->m[5] = 1.0f;
    out->m[10] = 1.0f;
    out->m[15] = 1.0f;
}

void mat4_perspective(struct mat4 *out, float fov_rad, float aspect, float near,
                      float far) {
    mat4_identity(out);
    float inv_tan_half_fov = 1.0f / tanf(fov_rad * 0.5f);

    out->m[0] = inv_tan_half_fov / aspect;
    out->m[5] = inv_tan_half_fov;
    out->m[10] = (near + far) / (near - far);
    out->m[11] = -1.0f;
    out->m[14] = (2.0f * near * far) / (near - far);
    out->m[15] = 0.0f;
}

void mat4_lookat(struct mat4 *out, struct vec3 eye, struct vec3 center,
                 struct vec3 up) {
    struct vec3 f = vec3_normalize(vec3_sub(center, eye));
    struct vec3 s = vec3_normalize(vec3_cross(f, up));
    struct vec3 u = vec3_cross(s, f);

    mat4_identity(out);
    out->m[0] = s.x;
    out->m[1] = u.x;
    out->m[2] = -f.x;
    out->m[4] = s.y;
    out->m[5] = u.y;
    out->m[6] = -f.y;
    out->m[8] = s.z;
    out->m[9] = u.z;
    out->m[10] = -f.z;

    out->m[12] = -vec3_dot(s, eye);
    out->m[13] = -vec3_dot(u, eye);
    out->m[14] = vec3_dot(f, eye);
}

void mat4_mul(struct mat4 *out, struct mat4 *a, struct mat4 *b) {
    float tmp[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp[j * 4 + i] = 0.0f;
            for (int k = 0; k < 4; k++) {
                tmp[j * 4 + i] += a->m[k * 4 + i] * b->m[j * 4 + k];
            }
        }
    }

    for (int i = 0; i < 16; i++) {
        out->m[i] = tmp[i];
    }
}