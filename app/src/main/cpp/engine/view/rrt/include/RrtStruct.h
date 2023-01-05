#ifndef RENDER_RRTSTRUCT_H
#define RENDER_RRTSTRUCT_H

#include <glm/glm.hpp>

namespace view
{
    const static char *RRT_SLOT_TRI = "u_triangles";

    // make sure every field is vec3
    struct RrtTriangle
    {
        glm::vec3 p0, p1, p2;
        glm::vec3 n0, n1, n2;
        glm::vec3 color;

        RrtTriangle() {}
        RrtTriangle(glm::vec3 inP0, glm::vec3 inP1, glm::vec3 inP2,
                    glm::vec3 inN0, glm::vec3 inN1, glm::vec3 inN2, glm::vec3 inColor) :
                    p0(inP0), p1(inP1), p2(inP2), n0(inN0), n1(inN1), n2(inN2), color(inColor)
        {}
    };
}

#endif //RENDER_RRTSTRUCT_H
