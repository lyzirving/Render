#include <algorithm>

#include "Scene.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace view
{
    Scene::Scene() : m_layers()
    {
        createLayers();
    }

    Scene::~Scene()
    {
        auto itr = m_layers.begin();
        while(itr != m_layers.end())
        {
            (*itr).reset();
            itr = m_layers.erase(itr);
        }
    }

    void Scene::createLayers()
    {
        sortLayers();
    }

    void Scene::sortLayers()
    {
        if (!m_layers.empty())
        {
            std::sort(m_layers.begin(), m_layers.end(), LayerSortObj);
        }
    }

    void Scene::update()
    {
        for (auto &layer : m_layers)
        {
            layer->update();
        }
    }
}

