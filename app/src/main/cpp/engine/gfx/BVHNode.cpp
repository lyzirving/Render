#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "BVHNode.h"

#include "AssetsMgr.h"
#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHNode"

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

    BVNBuilder::BVNBuilder(const char *path) : m_srcPath(path), m_srcDirectory(), m_name()
    {
        load();
    }

    BVNBuilder::~BVNBuilder() = default;

    void BVNBuilder::load()
    {
        if(m_srcPath.empty())
        {
            LOG_ERR("src path is empty");
            return;
        }
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
        LOG_DEBUG("finish parsing");
    }

    void BVNBuilder::processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // todo turn mesh into triangle
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            /*if(mesh != nullptr)
                m_mesh.push_back(std::move(processMesh(mesh, scene)));*/
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
}

