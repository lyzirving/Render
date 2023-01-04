#ifndef RENDER_RRTSTRUCT_H
#define RENDER_RRTSTRUCT_H

#include <glm/glm.hpp>

namespace view
{
    const static char *RTX_SLOT_TRI = "u_triangles";

    // make sure every field is vec3
    struct RtxTriangle
    {
        glm::vec3 p0, p1, p2;
        glm::vec3 n0, n1, n2;
        glm::vec3 color;
    };
}

#endif //RENDER_RRTSTRUCT_H
