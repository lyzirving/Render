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

    private:
        void createLayers();

    };
}

#endif //RENDER_SIGHTSCENE_H
