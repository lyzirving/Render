#include <mutex>
#include <limits>

#include "RrtDebugEngine.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtDebugEngine"

using namespace gfx;

static std::mutex g_mutex{};
static RrtDebugEngine *g_engine{nullptr};

RrtDebugEngine * RrtDebugEngine::get()
{
    if (!g_engine) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_engine)
            g_engine = new RrtDebugEngine;
    }
    return g_engine;
}

RrtDebugEngine::RrtDebugEngine() = default;

RrtDebugEngine::~RrtDebugEngine() = default;

void RrtDebugEngine::debug(const std::vector<gfx::RrtTriangle> &triangles,
                           const std::vector<gfx::RrtBVHNode> &nodes)
{
    LOG_FUNC_ENTER;
    int width = 1080;
    int height = 1854;

    glm::vec3 start{0.f, 0.f, 6.f};
    glm::vec3 dir{0.f};

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            //LOG_DEBUG("begin itr, row[%d], col[%d]", row ,col);
            dir.x = (float(col) - float(width) / 2.f) / (float(width) / 2.f);
            dir.y = (float(height) / 2.f - float(row)) / (float(height) / 2.f);
            dir.z = 0.f;
            dir = glm::normalize(dir - start);
            HitResult result;
            hitBVH(start, dir, triangles, nodes, result);
        }
    }
    LOG_FUNC_EXIT;
}

void RrtDebugEngine::hitBVH(const glm::vec3 &start, const glm::vec3 &dir,
                            const std::vector<gfx::RrtTriangle> &triangles,
                            const std::vector<gfx::RrtBVHNode> &nodes, HitResult &result)
{
    result.isHit = false;
    result.distance = FLT_MAX;

    int itrCnt{0};
    int stack[256];
    int sp = 0;
    // root node's index is 0
    stack[sp++] = 0;
    while(sp > 0)
    {
        itrCnt++;
        int index = stack[--sp];
        const RrtBVHNode &node = nodes[index];

        // leaf node
        if(node.posInfo.x > 0)
        {
            HitResult r = hitTriArray(start, dir, node.posInfo.y, node.posInfo.y + node.posInfo.x - 1, triangles);
            if (r.isHit)
                result.isHit = true;

            if(r.isHit && r.distance < result.distance)
                result.distance = r.distance;

            continue;
        }
        float d1 = -1.f;
        float d2 = -1.f;
        if(node.childInfo.x > 0.f)
        {
            const RrtBVHNode &leftChild = nodes[node.childInfo.x];
            d1 = hitAABB(start, dir, leftChild.AA, leftChild.BB);
        }
        if(node.childInfo.y > 0.f)
        {
            const RrtBVHNode &rightChild = nodes[node.childInfo.y];
            d2 = hitAABB(start, dir, rightChild.AA, rightChild.BB);
        }

        if(d1 > 0.f && d2 > 0.f)
        {
            if(d1 < d2)
            {
                // left should be polled out of stack firstly
                stack[sp++] = node.childInfo.y;
                stack[sp++] = node.childInfo.x;
            }
            else
            {
                // right should be polled out of stack firstly
                stack[sp++] = node.childInfo.x;
                stack[sp++] = node.childInfo.y;
            }
        }
        else if(d1 > 0.f)
        {
            stack[sp++] = node.childInfo.x;
        }
        else if(d2 > 0.f)
        {
            stack[sp++] = node.childInfo.y;
        }
    }
    //LOG_DEBUG("itr cnt[%d], hit[%s]", itrCnt, result.isHit ? "true" : "false");
}

HitResult RrtDebugEngine::hitTriArray(const glm::vec3 &start, const glm::vec3 &dir, const int l, const int r,
                                      const std::vector<gfx::RrtTriangle> &triangles)
{
    HitResult result;
    result.isHit = false;
    result.distance = FLT_MAX;
    if(l > r)
        return result;
    for(int i = l; i <= r; i++) {
        const RrtTriangle &tri = triangles[i];
        HitResult res = hitTriangle(start, dir, tri);
        if(res.isHit)
            result.isHit = true;

        if(res.distance < result.distance)
            result.distance = res.distance;
    }
    return result;
}

HitResult RrtDebugEngine::hitTriangle(const glm::vec3 &start, const glm::vec3 &dir,
                                      const gfx::RrtTriangle &tri)
{
    HitResult res;
    res.distance = FLT_MAX;
    res.isHit = false;

    glm::vec3 p0 = tri.p0;
    glm::vec3 p1 = tri.p1;
    glm::vec3 p2 = tri.p2;

    glm::vec3 s = start;
    glm::vec3 d = dir;
    glm::vec3 N = glm::normalize(glm::cross(p1-p0, p2-p0));

    // camera is placed in front of screen by default
    // N and ray.dir is on the same side
    if (glm::dot(N, d) > 0.0f)
        N = -N;

    // N is parallel to ray.dir
    if (std::abs(glm::dot(N, d)) < 0.00001f)
        return res;

    // by now, ray must hit the plane, compute the distance (t * direction)
    // algorithm referencing to https://blog.csdn.net/weixin_44176696/article/details/119044396?spm=1001.2014.3001.5502
    float t = (glm::dot(N, p0) - glm::dot(s, N)) / glm::dot(d, N);

    // triangle is behind of the eye
    if(t < 0.0005f)
        return res;

    glm::vec3 pt = s + d * t;

    glm::vec3 c1 = glm::cross(p1 - p0, pt - p0);
    glm::vec3 c2 = glm::cross(p2 - p1, pt - p1);
    glm::vec3 c3 = glm::cross(p0 - p2, pt - p2);
    bool hit1 = (glm::dot(c1, N) > 0.0f && glm::dot(c2, N) > 0.0f && glm::dot(c3, N) > 0.0f);
    bool hit2 = (glm::dot(c1, N) < 0.0f && glm::dot(c2, N) < 0.0f && glm::dot(c3, N) < 0.0f);

    if (hit1 || hit2)
    {
        res.isHit = true;
        res.distance = t;
        res.hitPt = pt;
        res.viewDir = d;
    }

    return res;
}

float RrtDebugEngine::hitAABB(const glm::vec3 &start, const glm::vec3 &dir, const glm::vec3 &AA,
                              const glm::vec3 &BB)
{
    if(abs(dir.x) < 0.000001 || abs(dir.y) < 0.000001 || abs(dir.z) < 0.000001) { return -1.f; }

    glm::vec3 invDir = 1.f / dir;
    glm::vec3 far = (BB - start) * invDir;
    glm::vec3 near = (AA - start) * invDir;

    glm::vec3 tMax = glm::max(far, near);
    glm::vec3 tMin = glm::min(far, near);

    float t1 = std::min(tMax.x, std::min(tMax.y, tMax.z));
    float t0 = std::max(tMin.x, std::max(tMin.y, tMin.z));

    if(t0 < t1 && t0 > 0.f) {
        return t0;
    } else {
        return -1.f;
    }
}

