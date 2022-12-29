#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include "ModelItem.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelItem"

namespace view
{
    ModelItem::ModelItem(const char *path) : m_srcPath(path), m_srcDirectory() {}

    ModelItem::~ModelItem() = default;

    void ModelItem::draw(const std::shared_ptr<ViewConv> &conv) {}

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
        m_srcDirectory = m_srcPath.substr(0, m_srcPath.find_last_of('/'));
        //todo make the model meshes;
        return true;
    }
}

