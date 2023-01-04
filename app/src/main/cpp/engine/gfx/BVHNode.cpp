#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    BVHBuilder::BVHBuilder(const char *path) : m_srcPath(path), m_srcDirectory(), m_name(),
                                               m_triangles()
    {
        load();
    }

    BVHBuilder::~BVHBuilder()
    {
        std::vector<RrtTriangle>().swap(m_triangles);
    }

    void BVHBuilder::load()
    {
        if(m_srcPath.empty())
        {
            LOG_ERR("src path is empty");
            return;
        }
        m_srcPath = AssetsMgr::getObj(m_srcPath);
        Assimp::Importer importer;
        const aiScene *scene =  importer.ReadFile(m_srcPath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERR("fail to load model from [%s], reason[%s]", m_srcPath.c_str(), importer.GetErrorString());
            return;
        }
        int pos0 = m_srcPath.find_last_of('/');
        int pos1 = m_srcPath.find_last_of('.');
        m_srcDirectory = m_srcPath.substr(0, pos0);
        m_name = m_srcPath.substr(pos0 + 1, std::max(0, pos1 - pos0 - 1));
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

