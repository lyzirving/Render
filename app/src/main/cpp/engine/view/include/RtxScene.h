#ifndef RENDER_RTXSCENE_H
#define RENDER_RTXSCENE_H

#include "Scene.h"

namespace view
{
    class RtxScene : public Scene
    {
    public:
        RtxScene();

        virtual ~RtxScene();
        virtual void update(const std::shared_ptr<ViewConv> &conv) override;
    protected:
        virtual void createLayers() override;
    };
}

#endif //RENDER_RTXSCENE_H
