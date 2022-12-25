#ifndef RENDER_GRERENDER_H
#define RENDER_GRERENDER_H

namespace gre
{
    class GreRender
    {
    public:
        GreRender() {}
        virtual ~GreRender() = default;

        virtual void update() = 0;
    };
}

#endif //RENDER_GRERENDER_H
