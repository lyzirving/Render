#version 320 es
precision highp float;

in vec2 v_tex;

uniform sampler2D u_diffuse;
uniform sampler2D u_spec;

out vec4 o_color;

void main() {
    o_color = texture(u_diffuse, v_tex);
}