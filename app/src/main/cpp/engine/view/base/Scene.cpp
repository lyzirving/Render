#include "Scene.h"
#include "ViewConv.h"

#include "LogUtil.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace view
{
    Scene::Scene() : m_layers(), m_existence(), m_conv(new ViewConv)
    {
    }

    Scene::~Scene()
    {
        m_conv.reset();
        auto itr = m_layers.begin();
        while(itr != m_layers.end())
        {
            (*itr).reset();
            itr = m_layers.erase(itr);
        }

        auto mapItr = m_existence.begin();
        while(mapItr != m_existence.end())
        {
            mapItr->second.reset();
            mapItr = m_existence.erase(mapItr);
        }
    }

    void Scene::addLayer(const std::shared_ptr<Layer> &layer)
    {
        if(m_existence.find(layer->key()) == m_existence.end())
        {
            m_layers.push_back(layer);
            m_existence.insert(std::pair<LayerKey, std::shared_ptr<Layer>>(layer->key(), layer));
            sortLayer();
        }
    }

    void Scene::removeLayer(const std::shared_ptr<Layer> &layer)
    {
        if (!layer)
            return;

        bool found{false};
        if(m_existence.find(layer->key()) != m_existence.end())
        {
            auto itr = m_layers.begin();
            while(itr != m_layers.end())
            {
                if ((*itr).get() == layer.get())
                {
                    itr = m_layers.erase(itr);
                    found = true;
                    break;
                }
            }
            if(found)
            {
                auto mapItr = m_existence.find(layer->key());
                if (mapItr != m_existence.end())
                {
                    mapItr->second.reset();
                    mapItr = m_existence.erase(mapItr);
                }
            }
        }
        if(!found)
            LOG_DEBUG("fail to find layer(type[%u], order[%u])", layer->type(), layer->order());
    }

    void Scene::sortLayer()
    {
        if (!m_layers.empty())
        {
            std::sort(m_layers.begin(), m_layers.end(), LayerSorter);
        }
    }

    void Scene::setViewport(int32_t x, int32_t y, int32_t width, int32_t height)
    {
        if (m_conv)
        {
            m_conv->setViewport(x, y, width, height);
        }
    }
}

