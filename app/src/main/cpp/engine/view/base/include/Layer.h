#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <vector>
#include <memory>

#include "ViewDef.h"

namespace view
{
    class ViewConv;
    class LayerItem;

    class Layer
    {
    public:
        Layer();
        Layer(LayerType type, uint8_t order);

        virtual ~Layer();
        virtual void update(const std::shared_ptr<ViewConv> &conv) = 0;

        inline LayerType type() { return m_type; }
        inline uint8_t order() { return m_order; }

    protected:
        virtual void createItems() = 0;

        LayerType m_type;
        uint8_t m_order;

        std::vector<std::shared_ptr<LayerItem>> m_items;
    };
}

#endif //RENDER_LAYER_H
