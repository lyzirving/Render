#ifndef RENDER_SIGHTSCENE_H
#define RENDER_SIGHTSCENE_H

#include "Scene.h"

namespace view
{
    class SightScene : public Scene
    {
    public:
        SightScene();

        virtual ~SightScene();
        virtual void update() override;

    protected:
        virtual void createLayers() override;

    };
}

#endif //RENDER_SIGHTSCENE_H
