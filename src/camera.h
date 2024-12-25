#ifndef CAMERA_H
#define CAMERA_H
#include "lmath.h"

struct camera {
    struct vec3 position;
    struct vec3 forward;
    struct vec3 right;
    struct vec3 up;

    float pitch_deg;
    float yaw_deg;

    float fov_deg;
    float aspect;
    float near;
    float far;

    struct mat4 view;
    struct mat4 proj;
    struct mat4 view_proj;
};

void camera_init(struct camera *cam, float fov_deg, float aspect);
void camera_update(struct camera *cam);

#endif /* CAMERA_H */
