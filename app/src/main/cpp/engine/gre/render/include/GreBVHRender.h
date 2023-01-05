#ifndef RENDER_GREBVHRENDER_H
#define RENDER_GREBVHRENDER_H

#include "GreRender.h"

namespace gre
{
    class GreBVHRender : public GreRender
    {
    public:
        GreBVHRender();

        virtual ~GreBVHRender();
        virtual void preUpd() override;
        virtual void update() override;
        virtual void postUpd() override;

    private:
    };
}

#endif //RENDER_GREBVHRENDER_H
