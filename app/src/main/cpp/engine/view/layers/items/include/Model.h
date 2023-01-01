#ifndef RENDER_MODEL_H
#define RENDER_MODEL_H

#include <string>
#include <vector>
#include <assimp/material.h>

#include "LayerItem.h"

namespace gfx
{
    class GfxMesh;
    class Texture;
}

class aiScene;
class aiNode;
class aiMesh;

namespace view
{
    class Model : public LayerItem
    {
    public:
        Model();
        Model(const char *path);
        virtual ~Model();

        float bottom();
        void fitCenter(bool set);
        void fitScale(bool set);

        virtual void draw(const std::shared_ptr<ViewConv> &conv) override;
        virtual void release() override;

        void translate(float x, float y, float z);

    protected:
        void calcAdj();
        void calcMtModel();
        bool loadModel();
        std::vector<std::shared_ptr<gfx::Texture>> loadTexture(aiMaterial *mt, aiTextureType type, uint8_t texType);
        void processNode(aiNode *node, const aiScene *scene);
        std::shared_ptr<gfx::GfxMesh> processMesh(aiMesh *mesh, const aiScene *scene);
        void updateMinMax(const glm::vec3 &pos);

        std::vector<std::shared_ptr<gfx::GfxMesh>> m_mesh;
        std::string m_srcPath, m_srcDirectory;
        std::string m_name;

        glm::vec3 m_minPos, m_maxPos;

        uint32_t m_meshInd;

        uint8_t m_adjFlag;
    };
}

#endif //RENDER_MODEL_H
