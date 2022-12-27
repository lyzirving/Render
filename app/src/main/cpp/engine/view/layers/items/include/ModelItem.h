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

    private:
        std::string m_srcPath;
    };
}

#endif //RENDER_MODELITEM_H
