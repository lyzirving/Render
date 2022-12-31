#version 320 es
precision mediump float;
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex;

uniform mat4 u_modelMt;
uniform mat4 u_viewMt;
uniform mat4 u_prjMt;

out vec2 v_tex;

void main() {
    v_tex = a_tex;
    gl_Position = u_prjMt * u_viewMt * u_modelMt * vec4(a_pos, 1.0);
}