#ifndef RENDER_BVHSCENE_H
#define RENDER_BVHSCENE_H

#include "Scene.h"

namespace view
{
    class BVHScene : public Scene
    {
    public:
        BVHScene();

        virtual ~BVHScene();
        virtual void update(const std::shared_ptr<ViewConv> &conv) override;
    protected:
        virtual void createLayers() override;
    };
}

#endif //RENDER_BVHSCENE_H
