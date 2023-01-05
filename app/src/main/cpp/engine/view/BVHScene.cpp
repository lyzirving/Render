#include "BVHScene.h"
#include "BVHLayer.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BVHScene"

namespace view
{
    BVHScene::BVHScene() : Scene()
    {
        BVHScene::createLayers();
    }

    BVHScene::~BVHScene() = default;

    void BVHScene::createLayers()
    {
        auto canvas = std::make_shared<BVHLayer>();
        addLayer(canvas);
    }

    void BVHScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

