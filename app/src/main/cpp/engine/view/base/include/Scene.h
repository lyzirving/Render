#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <map>
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
        virtual void update(const std::shared_ptr<ViewConv> &conv) = 0;

        void addLayer(const std::shared_ptr<Layer> &layer);
        void removeLayer(const std::shared_ptr<Layer> &layer);

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
        std::map<LayerKey, std::shared_ptr<Layer>> m_existence;
    };
}

#endif //RENDER_SCENE_H
