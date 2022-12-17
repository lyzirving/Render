#ifndef RENDER_GREEVENTPOOL_H
#define RENDER_GREEVENTPOOL_H

#include "Pool.h"
#include "GreObject.h"

namespace gre
{
    class GreEventPool
    {
    public:
        static GreEventPool *get();
        static void release(GreEventPool *ptr);

        PoolEvtArgType getEvtArg();

    private:
        GreEventPool();
        ~GreEventPool();

        Pool<GreEventArg> m_poolEvtArg;
    };
}

#endif //RENDER_GREEVENTPOOL_H
