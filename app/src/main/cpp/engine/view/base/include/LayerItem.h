#ifndef RENDER_LAYERITEM_H
#define RENDER_LAYERITEM_H

namespace view
{
    class LayerItem
    {
    public:
        LayerItem() {}
        virtual ~LayerItem() = default;

        virtual void draw() = 0;
    };
}

#endif //RENDER_LAYERITEM_H
