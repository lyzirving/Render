#include "BVHScene.h"
#include "BVHLayer.h"
#include "ClearLayer.h"

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
        std::shared_ptr<Layer> clearLayer = std::make_shared<ClearLayer>(LayerOrder::LOW);
        std::shared_ptr<Layer> bvhLayer = std::make_shared<BVHLayer>(LayerOrder::MID);

        addLayer(clearLayer);
        addLayer(bvhLayer);
    }

    void BVHScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

