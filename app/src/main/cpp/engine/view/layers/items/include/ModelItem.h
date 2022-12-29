#ifndef RENDER_MODELITEM_H
#define RENDER_MODELITEM_H

#include <string>

#include "LayerItem.h"
#include "Mesh.h"

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
        void processNode(aiNode *node, const aiScene *scene);
        //gfx::Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        std::string m_srcPath, m_srcDirectory;
    };
}

#endif //RENDER_MODELITEM_H
