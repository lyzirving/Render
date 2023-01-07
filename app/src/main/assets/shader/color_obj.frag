#version 320 es
precision highp float;

in vec2 v_tex;

uniform vec4 u_color;

out vec4 o_color;

void main() {
    o_color = u_color;
}