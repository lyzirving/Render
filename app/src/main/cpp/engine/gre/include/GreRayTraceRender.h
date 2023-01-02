#ifndef RENDER_GRERAYTRACERENDER_H
#define RENDER_GRERAYTRACERENDER_H

#include "GreRender.h"

namespace gre
{
    class GreRayTraceRender : public GreRender
    {
    public:
        GreRayTraceRender();

        virtual ~GreRayTraceRender();
        virtual void preUpd() override;
        virtual void update() override;
        virtual void postUpd() override;

    private:
    };
}

#endif //RENDER_GRERAYTRACERENDER_H
