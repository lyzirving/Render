#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <limits>
#include <algorithm>

#include "BVHNode.h"

#include "RrtStruct.h"

#include "AssetsMgr.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHNode"

using namespace view;

namespace gfx
{
    BVHNode::BVHNode() : m_left(nullptr), m_right(nullptr), m_itemCnt(0), m_startInd(-1),
                         m_AA(0.f), m_BB(0.f)
    {
    }

    BVHNode::~BVHNode()
    {
        m_left.reset();
        m_right.reset();
    }

    BVHBuilder::BVHBuilder(const char *name) : m_name(name), m_srcPath(),
                                               m_triangles()
    {
        load();
    }

    BVHBuilder::~BVHBuilder()
    {
        std::vector<RrtTriangle>().swap(m_triangles);
    }

    std::shared_ptr<BVHNode> BVHBuilder::build()
    {
        if(m_triangles.empty())
        {
            LOG_ERR("triangle is empty");
            return nullptr;
        }
        return buildWithSAH(m_triangles, 0, m_triangles.size() - 1, 8);
    }

    std::shared_ptr<BVHNode> BVHBuilder::buildWithSAH(std::vector<view::RrtTriangle> &triangles,
                                                      uint32_t l, uint32_t r, uint32_t limit)
    {
        if (l > r)
            return nullptr;

        std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
        node->m_AA = glm::vec3(FLT_MAX);
        node->m_BB = glm::vec3(FLT_MIN);

        // find AABB
        for(auto &item : triangles)
        {
            glm::vec3 min = glm::min(item.p0, glm::min(item.p1, item.p2));
            glm::vec3 max = glm::max(item.p0, glm::max(item.p1, item.p2));
            node->m_AA = glm::min(node->m_AA, min);
            node->m_BB = glm::max(node->m_BB, max);
        }

        //  leaf node
        if((r - l + 1) <= limit)
        {
            node->m_itemCnt = r - l + 1;
            node->m_startInd = l;
            return node;
        }

        float Cost = FLT_MAX;
        uint32_t Axis = 0;
        uint32_t Split = l;

        // iterate each axis
        for (uint32_t axisInd = 0; axisInd < 3; ++axisInd)
        {
            // sort the triangles by certain axis
            if(axisInd == 0)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpX);
            if(axisInd == 1)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpY);
            if(axisInd == 2)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpZ);

            std::vector<glm::vec3> leftMin(r - l + 1, glm::vec3(FLT_MAX));
            std::vector<glm::vec3> leftMax(r - l + 1, glm::vec3(FLT_MIN));
            for (uint32_t i = l; i <= r; i++)
            {
                auto &tri = triangles[i];
                uint32_t bias = (i == l) ? 0 : 1;

                leftMin[i - l] = glm::min(leftMin[i - l - bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
                leftMax[i - l] = glm::max(leftMax[i - l - bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
            }

            std::vector<glm::vec3> rightMin(r - l + 1, glm::vec3(FLT_MAX));
            std::vector<glm::vec3> rightMax(r - l + 1, glm::vec3(FLT_MIN));
            for (uint32_t i = r; i >= l; i--)
            {
                auto &tri = triangles[i];
                uint32_t bias = (i == r) ? 0 : 1;

                rightMin[i - l] = glm::min(rightMin[i - l + bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
                rightMax[i - l] = glm::max(rightMax[i - l + bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
            }

            uint32_t split = l;
            float cost = FLT_MAX;
            // split from l to r - 1 to calculate cost and split position
            for (uint32_t i = l; i <= r - 1; ++i)
            {
                // left side[l, i]
               glm::vec3 leftAA = leftMin[i - l];
               glm::vec3 leftBB = leftMax[i - l];
               glm::vec3 diff = leftBB - leftAA;
               float leftArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
               float leftCost = leftArea * float(i - l + 1);

               // right side[i + 1, r]
                glm::vec3 rightAA = rightMin[i - l + 1];
                glm::vec3 rightBB = rightMax[i - l + 1];
                diff = rightBB - rightAA;
                float rightArea = 2 * (diff.x * diff.y + diff.x * diff.z + diff.y * diff.z);
                float rightCost = rightArea * float(r - i);

                float totalCost = leftCost + rightCost;
                if(totalCost < cost)
                {
                    cost = totalCost;
                    split = i;
                }
            }

            if(cost < Cost)
            {
                Cost = cost;
                Split = split;
                Axis = axisInd;
            }
        }

        // sort by the axis with the lowest cost
        if(Axis == 0) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpX);
        if(Axis == 1) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpY);
        if(Axis == 2) std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpZ);

        node->m_left = buildWithSAH(triangles, l, Split, limit);
        node->m_right = buildWithSAH(triangles, Split + 1, r, limit);

        return node;
    }

    bool BVHBuilder::cmpX(const view::RrtTriangle &lhs, const view::RrtTriangle &rhs)
    {
        glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
        glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
        return lhsCenter.x < rhsCenter.x;
    }

    bool BVHBuilder::cmpY(const view::RrtTriangle &lhs, const view::RrtTriangle &rhs)
    {
        glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
        glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
        return lhsCenter.y < rhsCenter.y;
    }

    bool BVHBuilder::cmpZ(const view::RrtTriangle &lhs, const view::RrtTriangle &rhs)
    {
        glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
        glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
        return lhsCenter.z < rhsCenter.z;
    }

    void BVHBuilder::load()
    {
        if(m_name.empty())
        {
            LOG_ERR("src name is empty");
            return;
        }
        m_srcPath = AssetsMgr::getObj(m_name);
        Assimp::Importer importer;
        const aiScene *scene =  importer.ReadFile(m_srcPath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERR("fail to load model from [%s], reason[%s]", m_srcPath.c_str(), importer.GetErrorString());
            return;
        }
        LOG_DEBUG("load from[%s], begin to parse", m_srcPath.c_str());
        processNode(scene->mRootNode, scene);
        LOG_DEBUG("finish parsing, rrt triangle size[%lu]", m_triangles.size());
    }

    void BVHBuilder::processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void BVHBuilder::processMesh(aiMesh *mesh, const aiScene *scene)
    {
        if(!mesh || !scene)
            return;

        if(mesh->mNumVertices == 0)
            return;

        aiVector3D aiVert, aiNormal;
        RrtTriangle tri{};
        bool normalExist = (mesh->mNormals != nullptr);

        for(uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            if(face.mNumIndices != 3)
            {
                LOG_ERR("obj[%s]'s face[%u] index count is not 3", m_name.c_str(), i);
                continue;
            }

            aiVert = mesh->mVertices[face.mIndices[0]];
            tri.color = glm::vec3(1.f, 0.f, 0.f);
            tri.p0.x = aiVert.x;
            tri.p0.y = aiVert.y;
            tri.p0.z = aiVert.z;
            if (normalExist)
            {
                aiNormal = mesh->mNormals[face.mIndices[0]];
                tri.n0.x = aiNormal.x;
                tri.n0.y = aiNormal.y;
                tri.n0.z = aiNormal.z;
            }

            aiVert = mesh->mVertices[face.mIndices[1]];
            tri.p1.x = aiVert.x;
            tri.p1.y = aiVert.y;
            tri.p1.z = aiVert.z;
            if(normalExist)
            {
                aiNormal = mesh->mNormals[face.mIndices[1]];
                tri.n1.x = aiNormal.x;
                tri.n1.y = aiNormal.y;
                tri.n1.z = aiNormal.z;
            }

            aiVert = mesh->mVertices[face.mIndices[2]];
            tri.p2.x = aiVert.x;
            tri.p2.y = aiVert.y;
            tri.p2.z = aiVert.z;
            if(normalExist)
            {
                aiNormal = mesh->mNormals[face.mIndices[2]];
                tri.n2.x = aiNormal.x;
                tri.n2.y = aiNormal.y;
                tri.n2.z = aiNormal.z;
            }

            m_triangles.push_back(tri);
        }
    }
}

