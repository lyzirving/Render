#include "RtxScene.h"
#include "RtxCasLayer.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RtxScene"

namespace view
{
    RtxScene::RtxScene() : Scene()
    {
        RtxScene::createLayers();
    }

    RtxScene::~RtxScene() = default;

    void RtxScene::createLayers()
    {
        auto canvas = std::make_shared<RtxCasLayer>();
        addLayer(canvas);
    }

    void RtxScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

