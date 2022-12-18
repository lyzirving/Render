#ifndef RENDER_GREOBJECT_H
#define RENDER_GREOBJECT_H

#include "Pool.h"

namespace gre
{
    class GreEventArg
    {
    public:
        GreEventArg();
        virtual ~GreEventArg();

        void* argPtr;
    };

    using PoolEvtArgType = std::unique_ptr<GreEventArg, Pool<GreEventArg>::Deleter>;

    class IEvent
    {
    public:
        IEvent() {}
        virtual ~IEvent() {}

        virtual void fire(PoolEvtArgType &&arg) = 0;
    };

    class GreObject
    {
    public:
        GreObject();

        virtual ~GreObject();
    protected:
        virtual void slotCb(PoolEvtArgType &&arg);
    };
}

#endif //RENDER_GREOBJECT_H
