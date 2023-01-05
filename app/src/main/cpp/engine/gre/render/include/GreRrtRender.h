#ifndef RENDER_GRERRTRENDER_H
#define RENDER_GRERRTRENDER_H

#include "GreRender.h"

namespace gre
{
    class GreRrtRender : public GreRender
    {
    public:
        GreRrtRender();

        virtual ~GreRrtRender();
        virtual void preUpd() override;
        virtual void update() override;
        virtual void postUpd() override;

    private:
    };
}

#endif //RENDER_GRERRTRENDER_H
