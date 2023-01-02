#include "RayTraceScene.h"
#include "RayTraceCanvasLayer.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RayTraceScene"

namespace view
{
    RayTraceScene::RayTraceScene() : Scene()
    {
        RayTraceScene::createLayers();
    }

    RayTraceScene::~RayTraceScene() = default;

    void RayTraceScene::createLayers()
    {
        auto canvas = std::make_shared<RayTraceCanvasLayer>();
        addLayer(canvas);
    }

    void RayTraceScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

