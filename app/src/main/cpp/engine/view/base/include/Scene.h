#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>
#include <memory>
#include <algorithm>

#include "Layer.h"

namespace view
{
    class ViewConv;

    class Scene
    {
    public:
        Scene();

        virtual ~Scene();
        virtual void update() = 0;

        void setViewport(int32_t x, int32_t y, int32_t width, int32_t height);

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

        virtual void createLayers() = 0;
        void sortLayer();

        std::vector<std::shared_ptr<Layer>> m_layers;
        std::shared_ptr<ViewConv> m_conv;
    };
}

#endif //RENDER_SCENE_H
