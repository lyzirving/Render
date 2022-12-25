#ifndef RENDER_GRESCENERENDER_H
#define RENDER_GRESCENERENDER_H

#include <memory>

#include "GreRender.h"

namespace view
{
    class Scene;
}

/**
 * @author  lyzirving
 * @date    2022-12-25
 * @brief   GreSceneRender is an element that collecting all the rendering units.
 *          Rendering units contain Scene, Text and so on. Scene contains all kinds of
 *          Layers that need to be drawn. Text is another module that renders chars, words and digits.
**/

namespace gre
{
    class GreSceneRender : public GreRender
    {
    public:
        GreSceneRender();
        virtual ~GreSceneRender();

        virtual void update() override;

    private:
        std::shared_ptr<view::Scene> m_scene;
    };
}

#endif //RENDER_GRESCENERENDER_H
