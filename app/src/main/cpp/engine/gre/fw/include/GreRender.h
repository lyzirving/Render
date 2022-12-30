#ifndef RENDER_GRERENDER_H
#define RENDER_GRERENDER_H

#include "ViewConv.h"

namespace gre
{
    class GreRender
    {
    public:
        GreRender() : m_conv(new view::ViewConv) {}
        virtual ~GreRender()
        {
            m_conv.reset();
        }

        virtual void update() = 0;

    protected:
        std::shared_ptr<view::ViewConv> m_conv;
    };
}

#endif //RENDER_GRERENDER_H
