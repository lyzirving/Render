#version 320 es
precision highp float;

in vec3 i_pixel;
out vec4 o_color;

/******************* uniform fields **************************/
uniform samplerBuffer u_triangles;

uniform vec4 u_bgColor;
uniform vec3 u_eysPos;
uniform int u_triCnt;
/*************************************************************/


/********************** constant *****************************/
#define TRIANGLE_OFFSET 7
/*************************************************************/


/********************* definition ****************************/
struct Ray {
    vec3 start;
    vec3 dir;
};
struct Triangle {
    vec3 p0, p1, p2;
    vec3 n0, n1, n2;
    vec3 color;
};
/*************************************************************/

/************************ method *****************************/

Triangle getTri(int i) {
    int offset = i * TRIANGLE_OFFSET;
    Triangle t;

    t.p0 = texelFetch(u_triangles, offset + 0).xyz;
    t.p1 = texelFetch(u_triangles, offset + 1).xyz;
    t.p2 = texelFetch(u_triangles, offset + 2).xyz;

    t.n0 = texelFetch(u_triangles, offset + 3).xyz;
    t.n1 = texelFetch(u_triangles, offset + 4).xyz;
    t.n2 = texelFetch(u_triangles, offset + 5).xyz;

    t.color = texelFetch(u_triangles, offset + 6).xyz;

    return t;
}

/*************************************************************/

void main() {
    Ray ray;
    ray.start = u_eysPos;
    ray.dir = normalize(i_pixel - ray.start);
}