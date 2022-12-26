#include "SightScene.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SightScene"

namespace view
{
    SightScene::SightScene() : Scene()
    {
        createLayers();
        sortLayer();
    }

    SightScene::~SightScene() = default;

    void SightScene::createLayers()
    {

    }

    void SightScene::update()
    {
        for (auto &layer : m_layers)
        {
            layer->update();
        }
    }
}

