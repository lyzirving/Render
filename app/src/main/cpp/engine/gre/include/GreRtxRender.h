#ifndef RENDER_GRERTXRENDER_H
#define RENDER_GRERTXRENDER_H

#include "GreRender.h"

namespace gre
{
    class GreRtxRender : public GreRender
    {
    public:
        GreRtxRender();

        virtual ~GreRtxRender();
        virtual void preUpd() override;
        virtual void update() override;
        virtual void postUpd() override;

    private:
    };
}

#endif //RENDER_GRERTXRENDER_H
