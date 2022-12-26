#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>
#include <memory>
#include <algorithm>

#include "Layer.h"

namespace view
{
    class Scene
    {
    public:
        Scene() : m_layers() {}

        virtual ~Scene()
        {
            auto itr = m_layers.begin();
            while(itr != m_layers.end())
            {
                (*itr).reset();
                itr = m_layers.erase(itr);
            }
        }

        virtual void update() = 0;

    protected:

        struct
        {
            bool operator() (const std::shared_ptr<Layer>& lhs, const std::shared_ptr<Layer>& rhs)
            {
                if (!lhs) return false;
                if (!rhs) return true;
                return lhs->order() <= rhs->order();
            }
        } LayerSorter;

        void sortLayer()
        {
            if (!m_layers.empty())
            {
                std::sort(m_layers.begin(), m_layers.end(), LayerSorter);
            }
        }

        std::vector<std::shared_ptr<Layer>> m_layers;
    };
}

#endif //RENDER_SCENE_H
