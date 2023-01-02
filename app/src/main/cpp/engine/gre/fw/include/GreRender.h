#ifndef RENDER_GRERENDER_H
#define RENDER_GRERENDER_H

#include "ViewConv.h"

namespace view
{
    class Scene;
}

namespace gre
{
    class GreRender
    {
    public:
        GreRender() : m_conv(new view::ViewConv), m_scene(nullptr) {}
        virtual ~GreRender()
        {
            m_conv.reset();
            m_scene.reset();
        }

        virtual void preUpd() = 0;
        virtual void update() = 0;
        virtual void postUpd() = 0;

        void setViewport(int32_t x, int32_t y, int32_t width, int32_t height)
        {
            m_conv->setViewport(x, y, width, height);
        }

    protected:
        std::shared_ptr<view::ViewConv> m_conv;
        std::shared_ptr<view::Scene> m_scene;
    };
}

#endif //RENDER_GRERENDER_H
