#version 410
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_mvp;

out vec3 v_position;
out vec3 v_normal;

void main() {
    v_position = a_position;
    v_normal = a_normal;
    gl_Position = u_mvp * vec4(a_position, 1.0);
}