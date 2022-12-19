#ifndef RENDER_GREOBJECT_H
#define RENDER_GREOBJECT_H

#include "Pool.h"

namespace gre
{
    class GreObject;
    class GreContext;

    class GreEventArg
    {
    public:
        GreEventArg();
        virtual ~GreEventArg();

        GreEventArg(const GreEventArg &other) = delete;
        GreEventArg & operator=(const GreEventArg &other) = delete;

        GreEventArg(GreEventArg &&other) noexcept;
        GreEventArg & operator=(GreEventArg &&other) noexcept;

        int32_t argInt0, argInt1;
        GreObject* argObj;
        void *argData;
    };

    using PoolEvtArgType = std::unique_ptr<GreEventArg, Pool<GreEventArg>::Deleter>;
    using SlotCbType = void (*)(PoolEvtArgType &&);

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
        virtual void slotCb(PoolEvtArgType &&arg);

        std::shared_ptr<GreContext> getCtx();
        void setWeakCtx(const std::weak_ptr<GreContext> &ctx);

    protected:
        std::weak_ptr<GreContext> m_ctx;
    };
}

#endif //RENDER_GREOBJECT_H
