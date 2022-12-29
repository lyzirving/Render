#include "include/ObjLayer.h"
#include "LayerItem.h"
#include "ViewConv.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ObjLayer"

namespace view
{
    ObjLayer::ObjLayer(LayerOrder order) : Layer(LayerType::OBJ_LAYER, order)
    {
        ObjLayer::createItems();
    }

    ObjLayer::~ObjLayer() = default;

    void ObjLayer::createItems()
    {

    }

    void ObjLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        for(auto &item : m_items)
        {
            item->draw(conv);
        }
    }
}

