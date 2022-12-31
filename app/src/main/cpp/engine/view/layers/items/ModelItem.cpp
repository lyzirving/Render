#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

#include "ModelItem.h"
#include "AssetsMgr.h"
#include "GfxMesh.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelItem"

namespace view
{
    using namespace gfx;

    ModelItem::ModelItem(const char *path) : m_mesh(),
                                             m_srcPath(path), m_srcDirectory(), m_name(),
                                             m_minPos(FLT_MAX), m_maxPos(FLT_MIN),
                                             m_meshInd(0)
    {
        loadModel();
    }

    ModelItem::~ModelItem()
    {
        ModelItem::release();
    }

    void ModelItem::draw(const std::shared_ptr<ViewConv> &conv)
    {

    }

    bool ModelItem::loadModel()
    {
        if(m_srcPath.empty())
        {
            LOG_ERR("src path is empty");
            return false;
        }
        // use assimp load model and get aiScene
        Assimp::Importer importer;
        // aiProcess_Triangulate forces all primitive into triangles
        // aiProcess_FlipUVs will flip the y axis of texture coordinate
        // find more post process cmd here: http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        const aiScene *scene =  importer.ReadFile(m_srcPath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG_ERR("fail to load model from [%s], reason[%s]", m_srcPath.c_str(), importer.GetErrorString());
            return false;
        }
        int pos0 = m_srcPath.find_last_of('/');
        int pos1 = m_srcPath.find_last_of('.');
        m_srcDirectory = m_srcPath.substr(0, pos0);
        m_name = m_srcPath.substr(pos0 + 1, std::max(0, pos1 - pos0 - 1));
        LOG_DEBUG("load from[%s], parse", m_srcPath.c_str());
        LOG_DEBUG("********************************");
        processNode(scene->mRootNode, scene);
        LOG_DEBUG("********************************");
        LOG_DEBUG("finish parsing");
        return true;
    }

    std::vector<std::shared_ptr<Texture>> ModelItem::loadTexture(aiMaterial *mt, aiTextureType type,
                                                                      uint8_t texType)
    {
        std::vector<std::shared_ptr<Texture>> result;
        unsigned int count = mt->GetTextureCount(type);
        for(unsigned int i = 0; i < count; i++)
        {
            aiString texName;
            mt->GetTexture(type, i, &texName);
            std::string texPath = m_srcDirectory + '/' + texName.C_Str();
            std::shared_ptr<Texture> tmp = std::make_shared<Texture>(texPath.c_str(), TextureType(texType));
            tmp->createMem();
            result.push_back(tmp);
        }
        return result;
    }

    void ModelItem::processNode(aiNode *node, const aiScene *scene)
    {
        // note that Node only contains index of object
        // Scene contains all the meshes and elements which are needed for rendering
        // so we need to get objects from Scene by their index

        // process mesh for current node if the node has any meshes
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            if(mesh != nullptr)
                m_mesh.push_back(std::move(processMesh(mesh, scene)));
        }

        // process the children for current node
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<GfxMesh> ModelItem::processMesh(aiMesh *mesh, const aiScene *scene)
    {
        std::string meshName = m_name + "@Mesh@" + std::to_string(m_meshInd++);
        std::shared_ptr<GfxMesh> result = std::make_shared<GfxMesh>(meshName.c_str());

        std::vector<Vertex> &vertexList = result->getVertex();
        std::vector<uint32_t> &indices = result->getIndices();
        std::vector<std::shared_ptr<Texture>> &textures = result->getTextures();
        const std::shared_ptr<Material> &material = result->getMaterial();

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.m_pos.x = mesh->mVertices[i].x;
            vertex.m_pos.y = mesh->mVertices[i].y;
            vertex.m_pos.z = mesh->mVertices[i].z;
            updateMinMax(vertex.m_pos);

            vertex.m_normal.x = mesh->mNormals[i].x;
            vertex.m_normal.y = mesh->mNormals[i].y;
            vertex.m_normal.z = mesh->mNormals[i].z;

            if(mesh->mTextureCoords[0])
            {
                vertex.m_tex.x = mesh->mTextureCoords[0][i].x;
                vertex.m_tex.y = mesh->mTextureCoords[0][i].y;
            }
            vertexList.push_back(vertex);
        }

        // process the indices
        // every face represent a primitive
        // we use flag aiProcess_Triangulate to import the model, so a single primitive is a triangle
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        // every mesh only has one Material
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial *mt = scene->mMaterials[mesh->mMaterialIndex];
            if(mt != nullptr)
            {
                std::vector<std::shared_ptr<Texture>> diffuse = loadTexture(mt, aiTextureType_DIFFUSE,
                                                                           TextureType::DIFFUSE);
                textures.insert(textures.end(), diffuse.begin(), diffuse.end());

                std::vector<std::shared_ptr<Texture>> specular = loadTexture(mt, aiTextureType_SPECULAR,
                                                                            TextureType::SPECULAR);
                textures.insert(textures.end(), specular.begin(), specular.end());

                aiReturn ret{aiReturn::aiReturn_SUCCESS};
                aiColor3D color;
                ret = mt->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                if(ret == aiReturn::aiReturn_SUCCESS)
                    material->setKd(color.r, color.g, color.b);

                ret = mt->Get(AI_MATKEY_COLOR_SPECULAR, color);
                if(ret == aiReturn::aiReturn_SUCCESS)
                    material->setKs(color.r, color.g, color.b);

                ret = mt->Get(AI_MATKEY_COLOR_AMBIENT, color);
                if(ret == aiReturn::aiReturn_SUCCESS)
                    material->setKa(color.r, color.g, color.b);
            }
        }
        LOG_DEBUG("vertex size[%lu], indices size[%lu], texture size[%lu]",
                  vertexList.size(), indices.size(), textures.size());

        result->bind(false);

        return result;
    }

    void ModelItem::release()
    {
        auto itr = m_mesh.begin();
        while(itr != m_mesh.end())
        {
            (*itr).reset();
            itr = m_mesh.erase(itr);
        }
    }

    void ModelItem::updateMinMax(const glm::vec3 &pos)
    {
        m_maxPos = glm::max(pos, m_maxPos);
        m_minPos = glm::min(pos, m_minPos);
    }
}

