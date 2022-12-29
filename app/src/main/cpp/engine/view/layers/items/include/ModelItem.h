#ifndef RENDER_MODELITEM_H
#define RENDER_MODELITEM_H

#include "LayerItem.h"

#include <string>

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

        std::string m_srcPath, m_srcDirectory;
    };
}

#endif //RENDER_MODELITEM_H
