#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <limits>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "BVHNode.h"
#include "GfxMesh.h"
#include "GfxShader.h"
#include "RrtStruct.h"
#include "GfxShaderMgr.h"

#include "ViewConv.h"

#include "AssetsMgr.h"
#include "SystemUtil.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHNode"

#define INF 114514.0

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

    BVHBuilder::BVHBuilder(const char *name, bool debug) : m_name(name), m_srcPath(), m_triangles(),
                                                           m_debugFlag(debug), m_debugMesh(),
                                                           m_debugMax(-INF), m_debugMin(INF),
                                                           m_debugModelMt(1.f),
                                                           m_debugMeshId(0)
    {
        load();
    }

    BVHBuilder::~BVHBuilder()
    {
        std::vector<RrtTriangle>().swap(m_triangles);
        if (!m_debugMesh.empty())
        {
            auto itr = m_debugMesh.begin();
            while(itr != m_debugMesh.end())
            {
                (*itr).reset();
                itr = m_debugMesh.erase(itr);
            }
        }
    }

    std::shared_ptr<BVHNode> BVHBuilder::build()
    {
        if(m_triangles.empty())
        {
            LOG_ERR("triangle is empty");
            return nullptr;
        }
        LOG_DEBUG("start build BVH[%s]", m_name.c_str());
        int64_t start = systemTimeMs();
        std::shared_ptr<BVHNode> node = buildWithSAH(m_triangles, 0, m_triangles.size() - 1, 8);
        LOG_DEBUG("finish build BVH, cost[%.3f]s", (systemTimeMs() - start) / 1000.f);
        return node;
    }

    std::shared_ptr<BVHNode> BVHBuilder::buildWithSAH(std::vector<RrtTriangle> &triangles,
                                                      int l, int r, int limit)
    {
        if (l > r)
            return nullptr;

        std::shared_ptr<BVHNode> node = std::make_shared<BVHNode>();
        node->m_AA = glm::vec3(INF);
        node->m_BB = glm::vec3(-INF);

        // find AABB
        for (int i = l; i <= r; ++i) {
            RrtTriangle& item = triangles[i];
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

        float Cost = INF;
        int Axis = 0;
        int Split = (l + r) / 2;

        // iterate each axis
        for (int axisInd = 0; axisInd < 3; ++axisInd)
        {
            // sort the triangles by certain axis
            if(axisInd == 0)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpX);
            if(axisInd == 1)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpY);
            if(axisInd == 2)  std::sort(&triangles[0] + l, &triangles[0] + r + 1, cmpZ);

            std::vector<glm::vec3> leftMin(r - l + 1, glm::vec3(INF));
            std::vector<glm::vec3> leftMax(r - l + 1, glm::vec3(-INF));
            for (int i = l; i <= r; ++i)
            {
                RrtTriangle& tri = triangles[i];
                int bias = ((i == l) ? 0 : 1);

                leftMin[i - l] = glm::min(leftMin[i - l - bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
                leftMax[i - l] = glm::max(leftMax[i - l - bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
            }

            std::vector<glm::vec3> rightMin(r - l + 1, glm::vec3(INF));
            std::vector<glm::vec3> rightMax(r - l + 1, glm::vec3(-INF));
            for (int i = r; i >= l; --i)
            {
                RrtTriangle& tri = triangles[i];
                int bias = ((i == r) ? 0 : 1);

                rightMin[i - l] = glm::min(rightMin[i - l + bias], glm::min(tri.p0, glm::min(tri.p1, tri.p2)));
                rightMax[i - l] = glm::max(rightMax[i - l + bias], glm::max(tri.p0, glm::max(tri.p1, tri.p2)));
            }

            int split = l;
            float cost = INF;
            // split from l to r - 1 to calculate cost and split position
            for (int i = l; i <= r - 1; ++i)
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

    bool BVHBuilder::cmpX(const RrtTriangle &lhs, const RrtTriangle &rhs)
    {
        glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
        glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
        return lhsCenter.x < rhsCenter.x;
    }

    bool BVHBuilder::cmpY(const RrtTriangle &lhs, const RrtTriangle &rhs)
    {
        glm::vec3 lhsCenter = (lhs.p0 + lhs.p1 + lhs.p2) / 3.f;
        glm::vec3 rhsCenter = (rhs.p0 + rhs.p1 + rhs.p2) / 3.f;
        return lhsCenter.y < rhsCenter.y;
    }

    bool BVHBuilder::cmpZ(const RrtTriangle &lhs, const RrtTriangle &rhs)
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
        adjDebugMesh();
        LOG_DEBUG("finish parsing, rrt triangle size[%lu]", m_triangles.size());
    }

    void BVHBuilder::processNode(aiNode *node, const aiScene *scene)
    {
        for(int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }

        for(int i = 0; i < node->mNumChildren; ++i)
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

        for(int i = 0; i < mesh->mNumFaces; ++i)
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

        dealDebugMesh(mesh, scene);
    }

    void BVHBuilder::adjDebugMesh()
    {
        if (m_debugFlag)
        {
            glm::mat4 mtTrans{1.f}, mtScale{1.f};

            glm::vec3 center = (m_debugMax + m_debugMin) * 0.5f;
            mtTrans = glm::translate(mtTrans, -center);

            glm::vec3 interval = m_debugMax - m_debugMin;
            float scale = 2.f / std::max(std::max(interval.x, interval.y), interval.z);
            mtScale = glm::scale(mtScale, glm::vec3(scale));

            m_debugModelMt = mtScale * mtTrans;
        }
    }

    void BVHBuilder::drawDebug(const std::shared_ptr<view::ViewConv> &conv)
    {
        if(m_debugFlag)
        {
            const glm::mat4 &viewMt = conv->getViewMat();
            const glm::mat4 &prjMt = conv->getProjectMat();

            const std::shared_ptr<gfx::GfxShader> &shader = GfxShaderMgr::get()->getShader(ShaderType::COLOR_OBJ);
            shader->use(true);
            shader->setMat4(U_MT_VIEW, viewMt);
            shader->setMat4(U_MT_PROJ, prjMt);
            shader->setMat4(U_MT_MODEL, m_debugModelMt);
            shader->setVec4(U_COLOR, 1.f, 0.f, 0.f, 1.f);

            for(auto &item : m_debugMesh)
            {
                item->draw(shader);
            }
            shader->use(false);
        }
    }

    void BVHBuilder::dealDebugMesh(aiMesh *mesh, const aiScene *scene)
    {
        if(m_debugFlag && mesh && scene)
        {
            std::string meshName = m_name + "@Mesh@" + std::to_string(m_debugMeshId++);
            std::shared_ptr<GfxMesh> result = std::make_shared<GfxMesh>(meshName.c_str());

            std::vector<Vertex> &vertList = result->getVertex();
            std::vector<uint32_t> &indices = result->getIndices();

            for(int i = 0; i < mesh->mNumVertices; ++i)
            {
                Vertex vert{};
                aiVector3D aiVert = mesh->mVertices[i];
                vert.m_pos = glm::vec3(aiVert.x, aiVert.y, aiVert.z);
                m_debugMax = glm::max(m_debugMax, vert.m_pos);
                m_debugMin = glm::min(m_debugMin, vert.m_pos);

                vertList.push_back(vert);
            }

            for(int i = 0; i < mesh->mNumFaces; ++i)
            {
                aiFace face = mesh->mFaces[i];
                for (int j = 0; j < face.mNumIndices; ++j)
                    indices.push_back(face.mIndices[j]);
            }

            result->bind(false);
            m_debugMesh.push_back(result);
        }
    }
}

