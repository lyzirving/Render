#ifndef RENDER_RRTSTRUCT_H
#define RENDER_RRTSTRUCT_H

#include <glm/glm.hpp>

namespace gfx
{
    const static char *RRT_SLOT_TRI = "u_triangles";
    const static char *RRT_SLOT_BVH = "u_BVHNodes";

    // make sure every field is vec3
    struct RrtTriangle
    {
        glm::vec3 p0, p1, p2;
        glm::vec3 n0, n1, n2;
        glm::vec3 color;
    };

    struct RrtBVHNode
    {
        // childInfo.x stores left child's index, childInfo.y stores right child's index, childInfo.z is reserved
        glm::vec3 childInfo;
        // posInfo.x stores children's count, posInfo.y stores current node's children's start index, posInfo.z is reserved
        glm::vec3 posInfo;
        glm::vec3 AA, BB;

        RrtBVHNode() {}
    };
}

#endif //RENDER_RRTSTRUCT_H
