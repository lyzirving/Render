#include "Scene.h"
#include "ViewConv.h"

namespace view
{
    Scene::Scene() : m_layers(), m_conv(new ViewConv)
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

