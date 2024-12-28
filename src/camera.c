#include "camera.h"

#include <math.h>

void camera_init(camera *cam, float fov_deg, float aspect) {
    cam->position.x = 0.0f;
    cam->position.y = 0.0f;
    cam->position.z = 0.0f;

    cam->pitch_deg = 0.0f;
    cam->yaw_deg = -90.0f;

    cam->fov_deg = fov_deg;
    cam->aspect = aspect;
    cam->near = 0.1f;
    cam->far = 1000.0f;

    camera_update(cam);
}

void camera_update(camera *cam) {
    /* Pitch values of 90 degrees up or down cause gimbal lock. */
    cam->pitch_deg = CLAMP(cam->pitch_deg, -89.999f, 89.999f);

    float pitch = cam->pitch_deg * DEG_TO_RAD;
    float yaw = cam->yaw_deg * DEG_TO_RAD;

    vec3 forward = {
        .x = cosf(yaw) * cosf(pitch),
        .y = sinf(pitch),
        .z = sinf(yaw) * cosf(pitch),
    };

    cam->forward = vec3_normalize(forward);
    cam->right = vec3_normalize(
        vec3_cross(cam->forward, (vec3){0.0f, 1.0f, 0.0f}));
    cam->up = vec3_normalize(vec3_cross(cam->right, cam->forward));

    mat4_perspective(&cam->proj, cam->fov_deg * DEG_TO_RAD, cam->aspect,
                     cam->near, cam->far);
    mat4_lookat(&cam->view, cam->position,
                vec3_add(cam->position, cam->forward), cam->up);
    mat4_mul(&cam->view_proj, &cam->proj, &cam->view);
}
