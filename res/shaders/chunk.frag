#version 410
out vec4 v_frag;

in vec3 v_position;
in vec3 v_normal;

void main() {
    vec2 uv = fract(vec2(dot(v_normal.xzy, v_position.zxx), v_position.y + v_normal.y * v_position.z));

    v_frag = vec4(uv, 0.0, 1.0);
}