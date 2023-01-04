#include "RrtScene.h"
#include "RrtCasLayer.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtScene"

namespace view
{
    RrtScene::RrtScene() : Scene()
    {
        RrtScene::createLayers();
    }

    RrtScene::~RrtScene() = default;

    void RrtScene::createLayers()
    {
        auto canvas = std::make_shared<RrtCasLayer>();
        addLayer(canvas);
    }

    void RrtScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

