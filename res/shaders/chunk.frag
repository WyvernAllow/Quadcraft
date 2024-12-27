#version 410
out vec4 v_frag;

in vec3 v_position;
in vec3 v_normal;
in float v_layer;

uniform sampler2DArray u_texture;

void main() {
    vec2 uv = fract(vec2(dot(v_normal.xzy, v_position.zxx), v_position.y + v_normal.y * v_position.z));
    
    float num_layers = textureSize(u_texture, 0).z;
    float layer = v_layer * float(v_layer < num_layers);

    vec4 color = texture(u_texture, vec3(uv, layer));
    if(color.a < 0.1) {
        discard;
    }

    v_frag = color;
}