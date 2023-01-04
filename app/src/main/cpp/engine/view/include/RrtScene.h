#ifndef RENDER_RRTSCENE_H
#define RENDER_RRTSCENE_H

#include "Scene.h"

namespace view
{
    class RrtScene : public Scene
    {
    public:
        RrtScene();

        virtual ~RrtScene();
        virtual void update(const std::shared_ptr<ViewConv> &conv) override;
    protected:
        virtual void createLayers() override;
    };
}

#endif //RENDER_RRTSCENE_H
