#include "ObjLayer.h"
#include "ViewConv.h"
#include "Sphere.h"

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
        std::shared_ptr<LayerItem> model = std::make_shared<Model>(objName.c_str());
        ((Model *)model.get())->fitCenter(true);
        ((Model *)model.get())->fitScale(true);
        m_items.push_back(std::move(model));

        /*std::shared_ptr<LayerItem> sphere = std::make_shared<Sphere>("sphere");
        m_items.push_back(std::move(sphere));*/
    }

    void ObjLayer::update(const std::shared_ptr<ViewConv> &conv)
    {
        for(auto &item : m_items)
        {
            item->draw(conv);
        }
    }
}

