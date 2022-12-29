#ifndef RENDER_MODELITEM_H
#define RENDER_MODELITEM_H

#include <string>
#include <vector>
#include <assimp/material.h>
#include <glm/glm.hpp>

#include "LayerItem.h"

namespace gfx
{
    class Mesh;
    class Texture;
}

class aiScene;
class aiNode;
class aiMesh;

namespace view
{
    class ModelItem : public LayerItem
    {
    public:
        ModelItem(const char *path);
        virtual ~ModelItem();

        virtual void draw(const std::shared_ptr<ViewConv> &conv) override;

    protected:
        bool loadModel();
        std::vector<std::shared_ptr<gfx::Texture>> loadTexture(aiMaterial *mt, aiTextureType type, uint8_t texType);
        void processNode(aiNode *node, const aiScene *scene);
        std::shared_ptr<gfx::Mesh> processMesh(aiMesh *mesh, const aiScene *scene);

        std::vector<std::shared_ptr<gfx::Mesh>> m_mesh;
        std::string m_srcPath, m_srcDirectory;

        glm::vec3 m_minPos, m_maxPos;

    private:
        void updateMinMax(const glm::vec3 &pos);
    };
}

#endif //RENDER_MODELITEM_H
