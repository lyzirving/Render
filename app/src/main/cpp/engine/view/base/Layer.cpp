#include "Layer.h"
#include "LayerItem.h"

namespace view
{
    Layer::Layer() : m_type(LayerType::BASIC), m_order(0), m_items() {}

    Layer::Layer(LayerType type, uint8_t order) : m_type(type), m_order(order),
                                                  m_items() {}

    Layer::~Layer()
    {
        auto itr = m_items.begin();
        while(itr != m_items.end())
        {
            (*itr).reset();
            itr = m_items.erase(itr);
        }
    }
}
