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
        PoolSyncEvtArgType getSyncEvtArg();

    private:
        GreEventPool();
        ~GreEventPool();

        Pool<GreEventArg> m_poolEvtArg;
        Pool<GreSyncEventArg> m_poolSyncArg;
    };
}

#endif //RENDER_GREEVENTPOOL_H
