#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <vector>
#include <memory>

#include "Layer.h"

namespace view
{
    class Scene : public Layer
    {
    public:
        Scene();
        virtual ~Scene();

        virtual void update() override;

    protected:
        std::vector<std::shared_ptr<Layer>> m_layers;

    private:
        struct
        {
            bool operator() (const std::shared_ptr<Layer>& lhs, const std::shared_ptr<Layer>& rhs)
            {
                if (!lhs) return false;
                if (!rhs) return true;
                return lhs->order() <= rhs->order();
            }
        } LayerSortObj;

        void createLayers();
        void sortLayers();
    };
}

#endif //RENDER_SCENE_H
