#include "include/ObjLayer.h"
#include "ModelItem.h"
#include "ViewConv.h"
#include "ModelItem.h"
#include "AssetsMgr.h"

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
        std::string objName = AssetsMgr::getObj("Marry");
        std::shared_ptr<LayerItem> modelItem = std::make_shared<ModelItem>(objName.c_str());
        m_items.push_back(std::move(modelItem));
    }

    void ObjLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        for(auto &item : m_items)
        {
            item->draw(conv);
        }
    }
}

