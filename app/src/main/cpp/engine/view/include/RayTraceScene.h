#ifndef RENDER_RAYTRACESCENE_H
#define RENDER_RAYTRACESCENE_H

#include "Scene.h"

namespace view
{
    class RayTraceScene : public Scene
    {
    public:
        RayTraceScene();

        virtual ~RayTraceScene();
        virtual void update(const std::shared_ptr<ViewConv> &conv) override;
    protected:
        virtual void createLayers() override;
    };
}

#endif //RENDER_RAYTRACESCENE_H
