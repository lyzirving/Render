#include "SightScene.h"
#include "ClearLayer.h"
#include "ObjLayer.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SightScene"

namespace view
{
    SightScene::SightScene() : Scene()
    {
        SightScene::createLayers();
    }

    SightScene::~SightScene() = default;

    void SightScene::createLayers()
    {
        std::shared_ptr<Layer> clearLayer = std::make_shared<ClearLayer>(LayerOrder::LOW);
        std::shared_ptr<Layer> objLayer = std::make_shared<ObjLayer>(LayerOrder::MID);

        addLayer(clearLayer);
        addLayer(objLayer);
    }

    void SightScene::update(const std::shared_ptr<ViewConv> &conv)
    {
        for (auto &layer : m_layers)
        {
            layer->update(conv);
        }
    }
}

