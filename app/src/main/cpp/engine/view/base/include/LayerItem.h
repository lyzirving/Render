#ifndef RENDER_LAYERITEM_H
#define RENDER_LAYERITEM_H

#include <memory>

namespace view
{
    class ViewConv;

    class LayerItem
    {
    public:
        LayerItem() {}
        virtual ~LayerItem() = default;

        virtual void draw(const std::shared_ptr<ViewConv> &conv) = 0;
    };
}

#endif //RENDER_LAYERITEM_H
