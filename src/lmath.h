#ifndef LMATH_H
#define LMATH_H

#define PI 3.14159265358979323846
#define DEG_TO_RAD (PI / 180)
#define RAD_TO_DEG (180 / PI)
#define CLAMP(value, min, max) (min > value ? min : (max < value ? max : value))

struct vec3 {
    float x;
    float y;
    float z;
};

struct vec3 vec3_add(struct vec3 a, struct vec3 b);
struct vec3 vec3_sub(struct vec3 a, struct vec3 b);
struct vec3 vec3_scale(struct vec3 v, float s);
float vec3_dot(struct vec3 a, struct vec3 b);
struct vec3 vec3_cross(struct vec3 a, struct vec3 b);
float vec3_len(struct vec3 v);
struct vec3 vec3_normalize(struct vec3 v);

struct mat4 {
    float m[4 * 4];
};

void mat4_identity(struct mat4 *out);
void mat4_perspective(struct mat4 *out, float fov_rad, float aspect, float near,
                      float far);
void mat4_lookat(struct mat4 *out, struct vec3 eye, struct vec3 center,
                 struct vec3 up);
void mat4_mul(struct mat4 *out, struct mat4 *a, struct mat4 *b);

#endif /* LMATH_H */
