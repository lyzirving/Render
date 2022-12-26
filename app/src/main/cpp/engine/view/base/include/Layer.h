#ifndef RENDER_LAYER_H
#define RENDER_LAYER_H

#include <vector>
#include <memory>

#include "LayerItem.h"
#include "ViewDef.h"

namespace view
{
    class Layer
    {
    public:
        Layer() : m_type(LayerType::BASIC), m_order(0), m_items() {}
        Layer(LayerType type, uint8_t order) : m_type(type),
                                               m_order(order),
                                               m_items() {}

        virtual ~Layer() {}
        virtual void update() = 0;

        inline LayerType type() { return m_type; }
        inline uint8_t order() { return m_order; }

    protected:
        LayerType m_type;
        uint8_t m_order;

        std::vector<std::shared_ptr<LayerItem>> m_items;
    };
}

#endif //RENDER_LAYER_H
