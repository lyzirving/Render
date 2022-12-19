#ifndef RENDER_GREOBJECT_H
#define RENDER_GREOBJECT_H

#include "Pool.h"

class Sync;

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

    class GreSyncEventArg : public GreEventArg
    {
    public:
        GreSyncEventArg();
        virtual ~GreSyncEventArg();

        GreSyncEventArg(const GreSyncEventArg &other) = delete;
        GreSyncEventArg & operator=(const GreSyncEventArg &other) = delete;

        GreSyncEventArg(GreSyncEventArg &&other) noexcept;
        GreSyncEventArg & operator=(GreSyncEventArg &&other) noexcept;

        Sync *sync;
    };

    using PoolEvtArgType = std::unique_ptr<GreEventArg, Pool<GreEventArg>::Deleter>;
    using PoolSyncEvtArgType = std::unique_ptr<GreSyncEventArg, Pool<GreSyncEventArg>::Deleter>;

    class IEvent
    {
    public:
        IEvent() {}
        virtual ~IEvent() {}

        virtual void fire(PoolEvtArgType &&arg) = 0;
        virtual void fire(PoolSyncEvtArgType &&arg) = 0;
    };

    class GreObject
    {
    public:
        GreObject();

        virtual ~GreObject();
        virtual void slotCb(PoolEvtArgType &&arg);
        virtual void slotCb(PoolSyncEvtArgType &&arg);

        std::shared_ptr<GreContext> getCtx();
        void setWeakCtx(const std::weak_ptr<GreContext> &ctx);

    protected:
        std::weak_ptr<GreContext> m_ctx;
    };
}

#endif //RENDER_GREOBJECT_H
