#ifndef RENDER_RRTDEBUGENGINE_H
#define RENDER_RRTDEBUGENGINE_H

#include <vector>
#include "RrtStruct.h"

struct HitResult
{
    bool isHit;
    float distance;
    glm::vec3 hitPt, viewDir;
    HitResult() : isHit(false), distance(0.f), hitPt(0.f), viewDir(0.f) {}
};

class RrtDebugEngine
{
public:
    static RrtDebugEngine* get();

    static void debug(const std::vector<gfx::RrtTriangle> &triangles, const std::vector<gfx::RrtBVHNode> &nodes);
private:
    static void hitBVH(const glm::vec3 &start, const glm::vec3 &dir, const std::vector<gfx::RrtTriangle> &triangles,
                const std::vector<gfx::RrtBVHNode> &nodes, HitResult &result);
    static HitResult hitTriArray(const glm::vec3 &start, const glm::vec3 &dir, const int l, const int r,
                          const std::vector<gfx::RrtTriangle> &triangles);
    static HitResult hitTriangle(const glm::vec3 &start, const glm::vec3 &dir, const gfx::RrtTriangle &tri);
    static float hitAABB(const glm::vec3 &start, const glm::vec3 &dir, const glm::vec3 &AA, const glm::vec3 &BB);

    RrtDebugEngine();
    ~RrtDebugEngine();
};

#endif //RENDER_RRTDEBUGENGINE_H
